#include "SendTransfer.h"

#include <fstream>

#include <fmt/core.h>

#include <QDir>
#include <QFile>
#include <QTcpSocket>
#include <QHostAddress>

#include "utils/message_helper.h"

#include "protocol/message.pb.h"

namespace fs = std::filesystem;

class ObjectSendTransfer {
public:
    ObjectSendTransfer(QTcpSocket *conn, const fs::path &base, const fs::path &relPath)
        : m_conn(conn)
        , m_base(base)
        , m_relPath(relPath)
        , m_path(m_base / m_relPath) {}
    virtual ~ObjectSendTransfer() = default;

    virtual void handleMessage(const Message &msg) = 0;
    virtual bool done() = 0;
    virtual void sendRequest() = 0;

protected:
    QTcpSocket *m_conn;
    const fs::path m_base;
    const fs::path m_relPath;
    const fs::path m_path;
};

class FileSendTransfer : public ObjectSendTransfer {
public:
    FileSendTransfer(QTcpSocket *conn, const fs::path &base, const fs::path &relPath)
        : ObjectSendTransfer(conn, base, relPath)
        , m_stream(m_path, std::ios::binary)
        , m_remainingSize(fs::file_size(m_path)) {}

    virtual void handleMessage(const Message &msg) override {
        switch (msg.payload_case()) {
        case Message::PayloadCase::kSendFileResponse: // 创建文件成功，开始发送文件
        case Message::PayloadCase::kSendFileChunkResponse: // 上个 chunk 处理成功，发送下一个 chunk
        {
            if (done()) {
                sendDone();
            } else {
                sendNextChunk();
            }
            break;
        }
        default:
            m_conn->close();
        }
    }

    virtual bool done() override { return m_stream.peek() == EOF; }

    virtual void sendRequest() override {
        Message msg;
        auto *sendFileRequest = msg.mutable_sendfilerequest();
        sendFileRequest->set_relpath(m_relPath);

        m_conn->write(MessageHelper::genMessage(msg));
    }

    void sendNextChunk() {
        Message msg;
        auto *sendFileChunkRequest = msg.mutable_sendfilechunkrequest();
        sendFileChunkRequest->set_relpath(m_relPath);
        sendFileChunkRequest->set_offset(m_stream.tellg());
        std::string *data = sendFileChunkRequest->mutable_data();

        if (m_remainingSize >= MAX_CHUNK_SIZE) {
            data->resize(MAX_CHUNK_SIZE);
        } else {
            data->resize(m_remainingSize);
        }

        if (!m_stream.read(data->data(), data->size())) {
            return;
        }

        m_remainingSize -= data->size();
        m_conn->write(MessageHelper::genMessage(msg));
    }

    void sendDone() {
        Message msg;
        auto *stopSendFileRequest = msg.mutable_stopsendfilerequest();
        stopSendFileRequest->set_relpath(m_relPath);
        // stopSendFileRequest->set_sha256();

        m_conn->write(MessageHelper::genMessage(msg));
    }

private:
    static const size_t MAX_CHUNK_SIZE = 1024 * 1024;
    std::ifstream m_stream;
    uintmax_t m_remainingSize;
};

class DirectorySendTransfer : public ObjectSendTransfer {
public:
    DirectorySendTransfer(QTcpSocket *conn, const fs::path &base, const fs::path &relPath)
        : ObjectSendTransfer(conn, base, relPath)
        , m_dirIter(m_path) {}

    virtual void handleMessage(const Message &msg) override {
        if (m_child) {
            // 文件还未发送完成，交由它处理
            m_child->handleMessage(msg);
            if (m_child->done()) {
                m_child.reset();
                return;
            }
        }

        switch (msg.payload_case()) {
        case Message::PayloadCase::kStopSendFileResponse: // 上个文件发送结束
        case Message::PayloadCase::kSendDirResponse:      // 目录创建结束
        {
            sendNextEntry();
            break;
        }
        default: {
            m_conn->close();
        }
        }
    }

    virtual bool done() override { return m_dirIter == fs::end(m_dirIter); }

    virtual void sendRequest() override {
        Message msg;
        auto *sendDirRequest = msg.mutable_senddirrequest();
        sendDirRequest->set_relpath(m_relPath);

        m_conn->write(MessageHelper::genMessage(msg));
    }

    void sendNextEntry() {
        fs::directory_entry entry = *m_dirIter;
        m_dirIter++;
        auto relPath = entry.path().lexically_relative(m_base);

        if (entry.is_directory()) {
            Message msg;
            auto *sendDirRequest = msg.mutable_senddirrequest();
            sendDirRequest->set_relpath(relPath);

            m_conn->write(MessageHelper::genMessage(msg));
            return;
        }

        m_child = std::make_shared<FileSendTransfer>(m_conn, m_base, entry.path());
        m_child->sendRequest();
    }

private:
    std::filesystem::recursive_directory_iterator m_dirIter;
    std::shared_ptr<ObjectSendTransfer> m_child;
};

SendTransfer::SendTransfer(const QStringList &filePaths)
    : m_conn(nullptr)
    , m_filePaths(filePaths) {
}

void SendTransfer::send(const std::string &ip, uint16_t port) {
    m_conn = new QTcpSocket(this);

    connect(m_conn, &QTcpSocket::connected, [this] { sendNextObject(); });
    connect(m_conn, &QTcpSocket::errorOccurred, [this](QAbstractSocket::SocketError err) {
        qWarning() << "transfer connection failed:" << err;
        deleteLater();
    });
    connect(m_conn, &QTcpSocket::readyRead, this, &SendTransfer::dispatcher);
    connect(m_conn, &QTcpSocket::disconnected, this, &SendTransfer::handleDisconnected);
    m_conn->connectToHost(QHostAddress(QString::fromStdString(ip)), port);
}

void SendTransfer::dispatcher() {
    while (m_conn->size() >= header_size) {
        QByteArray buffer = m_conn->peek(header_size);
        auto header = MessageHelper::parseMessageHeader(buffer);
        if (!header.legal()) {
            qWarning() << "illegal message from " << m_conn->peerAddress().toString();
            return;
        }

        if (m_conn->size() < static_cast<qint64>(header_size + header.size())) {
            qDebug() << "partial content";
            return;
        }

        m_conn->read(header_size);
        auto size = header.size();
        buffer = m_conn->read(size);

        Message msg = MessageHelper::parseMessageBody<Message>(buffer.data(), buffer.size());
        qDebug() << fmt::format("message type: {}", msg.payload_case()).data();

        switch (msg.payload_case()) {
        case Message::PayloadCase::kSendFileResponse:
        case Message::PayloadCase::kSendFileChunkResponse:
        case Message::PayloadCase::kStopSendFileResponse:
        case Message::PayloadCase::kSendDirResponse: {
            if (m_objectSendTransfer) {
                m_objectSendTransfer->handleMessage(msg);
                if (m_objectSendTransfer->done()) {
                    m_objectSendTransfer.reset();
                }
            } else {
                sendNextObject();
            }

            break;
        }
        case Message::PayloadCase::kStopTransferResponse: {
            m_conn->close();
        }
        default: {
        }
        }
    }
}

void SendTransfer::sendNextObject() {
    if (m_filePaths.empty()) {
        Message msg;
        [[maybe_unused]] auto *stopSendTransferRequest = msg.mutable_stoptransferrequest();

        m_conn->write(MessageHelper::genMessage(msg));
        return;
    }

    QString qpath = m_filePaths[m_filePaths.size() - 1];
    m_filePaths.pop_back();
    fs::path path(qpath.toStdString());
    if (!fs::is_directory(path)) {
        m_objectSendTransfer = std::make_shared<FileSendTransfer>(m_conn,
                                                                  path.parent_path(),
                                                                  path.filename());
    } else {
        m_objectSendTransfer = std::make_shared<DirectorySendTransfer>(m_conn,
                                                                       path.parent_path(),
                                                                       path.filename());
    }

    m_objectSendTransfer->sendRequest();
}

void SendTransfer::handleDisconnected() {
    deleteLater();
}