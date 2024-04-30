//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: message.fbe
// FBE version: 1.14.5.0
//------------------------------------------------------------------------------

#include "proto.h"

namespace proto {

OriginMessage::OriginMessage()
    : id(FBE::uuid_t::sequential())
    , mask((int32_t)0ll)
    , json_msg()
{}

OriginMessage::OriginMessage(const FBE::uuid_t& arg_id, int32_t arg_mask, const std::string& arg_json_msg)
    : id(arg_id)
    , mask(arg_mask)
    , json_msg(arg_json_msg)
{}

bool OriginMessage::operator==(const OriginMessage& other) const noexcept
{
    return (
        (id == other.id)
        );
}

bool OriginMessage::operator<(const OriginMessage& other) const noexcept
{
    if (id < other.id)
        return true;
    if (other.id < id)
        return false;
    return false;
}

void OriginMessage::swap(OriginMessage& other) noexcept
{
    using std::swap;
    swap(id, other.id);
    swap(mask, other.mask);
    swap(json_msg, other.json_msg);
}

std::ostream& operator<<(std::ostream& stream, const OriginMessage& value)
{
    stream << "OriginMessage(";
    stream << "id="; stream << "\"" << value.id << "\"";
    stream << ",mask="; stream << value.mask;
    stream << ",json_msg="; stream << "\"" << value.json_msg << "\"";
    stream << ")";
    return stream;
}

MessageReject::MessageReject()
    : id(FBE::uuid_t::sequential())
    , error()
{}

MessageReject::MessageReject(const FBE::uuid_t& arg_id, const std::string& arg_error)
    : id(arg_id)
    , error(arg_error)
{}

bool MessageReject::operator==(const MessageReject& other) const noexcept
{
    return (
        (id == other.id)
        );
}

bool MessageReject::operator<(const MessageReject& other) const noexcept
{
    if (id < other.id)
        return true;
    if (other.id < id)
        return false;
    return false;
}

void MessageReject::swap(MessageReject& other) noexcept
{
    using std::swap;
    swap(id, other.id);
    swap(error, other.error);
}

std::ostream& operator<<(std::ostream& stream, const MessageReject& value)
{
    stream << "MessageReject(";
    stream << "id="; stream << "\"" << value.id << "\"";
    stream << ",error="; stream << "\"" << value.error << "\"";
    stream << ")";
    return stream;
}

MessageNotify::MessageNotify()
    : notification()
{}

MessageNotify::MessageNotify(const std::string& arg_notification)
    : notification(arg_notification)
{}

bool MessageNotify::operator==(const MessageNotify& other) const noexcept
{
    return (
        true
        );
}

bool MessageNotify::operator<(const MessageNotify& other) const noexcept
{
    return false;
}

void MessageNotify::swap(MessageNotify& other) noexcept
{
    using std::swap;
    swap(notification, other.notification);
}

std::ostream& operator<<(std::ostream& stream, const MessageNotify& value)
{
    stream << "MessageNotify(";
    stream << "notification="; stream << "\"" << value.notification << "\"";
    stream << ")";
    return stream;
}

DisconnectRequest::DisconnectRequest()
    : id(FBE::uuid_t::sequential())
{}

DisconnectRequest::DisconnectRequest(const FBE::uuid_t& arg_id)
    : id(arg_id)
{}

bool DisconnectRequest::operator==(const DisconnectRequest& other) const noexcept
{
    return (
        (id == other.id)
        );
}

bool DisconnectRequest::operator<(const DisconnectRequest& other) const noexcept
{
    if (id < other.id)
        return true;
    if (other.id < id)
        return false;
    return false;
}

void DisconnectRequest::swap(DisconnectRequest& other) noexcept
{
    using std::swap;
    swap(id, other.id);
}

std::ostream& operator<<(std::ostream& stream, const DisconnectRequest& value)
{
    stream << "DisconnectRequest(";
    stream << "id="; stream << "\"" << value.id << "\"";
    stream << ")";
    return stream;
}

} // namespace proto