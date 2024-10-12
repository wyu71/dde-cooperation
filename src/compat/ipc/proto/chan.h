﻿// Autogenerated.
// DO NOT EDIT. All changes will be undone.
#pragma once

#include "co/json.h"

struct UserLoginResult {
    fastring appname;
    fastring uuid;
    fastring ip;
    bool result;

    void from_json(const co::Json& _x_) {
        appname = _x_.get("appname").as_c_str();
        uuid = _x_.get("uuid").as_c_str();
        ip = _x_.get("ip").as_c_str();
        result = _x_.get("result").as_bool();
    }

    co::Json as_json() const {
        co::Json _x_;
        _x_.add_member("appname", appname);
        _x_.add_member("uuid", uuid);
        _x_.add_member("ip", ip);
        _x_.add_member("result", result);
        return _x_;
    }
};

struct LoginConfirm {
    fastring user_name;
    fastring session_id;
    fastring host_ip;

    void from_json(const co::Json& _x_) {
        user_name = _x_.get("user_name").as_c_str();
        session_id = _x_.get("session_id").as_c_str();
        host_ip = _x_.get("host_ip").as_c_str();
    }

    co::Json as_json() const {
        co::Json _x_;
        _x_.add_member("user_name", user_name);
        _x_.add_member("session_id", session_id);
        _x_.add_member("host_ip", host_ip);
        return _x_;
    }
};

struct ConfirmResult {
    int32 session_id;
    bool result;
    fastring reason;

    void from_json(const co::Json& _x_) {
        session_id = (int32)_x_.get("session_id").as_int64();
        result = _x_.get("result").as_bool();
        reason = _x_.get("reason").as_c_str();
    }

    co::Json as_json() const {
        co::Json _x_;
        _x_.add_member("session_id", session_id);
        _x_.add_member("result", result);
        _x_.add_member("reason", reason);
        return _x_;
    }
};

struct FSResult {
    int32 job_id;
    bool result;
    fastring reason;

    void from_json(const co::Json& _x_) {
        job_id = (int32)_x_.get("job_id").as_int64();
        result = _x_.get("result").as_bool();
        reason = _x_.get("reason").as_c_str();
    }

    co::Json as_json() const {
        co::Json _x_;
        _x_.add_member("job_id", job_id);
        _x_.add_member("result", result);
        _x_.add_member("reason", reason);
        return _x_;
    }
};

struct FSDataBlock {
    int32 job_id{0};
    int32 file_id{0};
    fastring rootdir;
    fastring filename;
    int64 blk_id{0};
    int32 flags{0};
    int64 data_size{0};
    fastring data;

    void from_json(const co::Json& _x_) {
        job_id = (int32)_x_.get("job_id").as_int64();
        file_id = (int32)_x_.get("file_id").as_int64();
        rootdir = _x_.get("rootdir").as_c_str();
        filename = _x_.get("filename").as_c_str();
        blk_id = (int64)_x_.get("blk_id").as_int64();
        flags = (int32)_x_.get("flags").as_int64();
        data_size = _x_.get("data_size").as_int64();
        data = _x_.get("data").as_c_str();
    }

    co::Json as_json() const {
        co::Json _x_;
        _x_.add_member("job_id", job_id);
        _x_.add_member("file_id", file_id);
        _x_.add_member("rootdir", rootdir);
        _x_.add_member("filename", filename);
        _x_.add_member("blk_id", blk_id);
        _x_.add_member("flags", flags);
        _x_.add_member("data_size", data_size);
        _x_.add_member("data", data);
        return _x_;
    }
};

struct FileInfo {
    int32 job_id;
    int32 file_id;
    fastring name;
    int64 total_size;
    int64 current_size;
    int64 time_spended;

    void from_json(const co::Json& _x_) {
        job_id = (int32)_x_.get("job_id").as_int64();
        file_id = (int32)_x_.get("file_id").as_int64();
        name = _x_.get("name").as_c_str();
        total_size = (int64)_x_.get("total_size").as_int64();
        current_size = (int64)_x_.get("current_size").as_int64();
        time_spended = (int64)_x_.get("time_spended").as_int64();
    }

    co::Json as_json() const {
        co::Json _x_;
        _x_.add_member("job_id", job_id);
        _x_.add_member("file_id", file_id);
        _x_.add_member("name", name);
        _x_.add_member("total_size", total_size);
        _x_.add_member("current_size", current_size);
        _x_.add_member("time_spended", time_spended);
        return _x_;
    }
};

struct FSJobCancel {
    int32 job_id;
    fastring path;

    void from_json(const co::Json& _x_) {
        job_id = (int32)_x_.get("job_id").as_int64();
        path = _x_.get("path").as_c_str();
    }

    co::Json as_json() const {
        co::Json _x_;
        _x_.add_member("job_id", job_id);
        _x_.add_member("path", path);
        return _x_;
    }
};

struct FSReport {
    int32 job_id;
    fastring path;
    int32 result { -1 };
    fastring error;

    void from_json(const co::Json& _x_) {
        job_id = (int32)_x_.get("job_id").as_int64();
        path = _x_.get("path").as_c_str();
        result = (int32)_x_.get("result").as_int64();
        error = _x_.get("error").as_c_str();
    }

    co::Json as_json() const {
        co::Json _x_;
        _x_.add_member("job_id", job_id);
        _x_.add_member("path", path);
        _x_.add_member("result", result);
        _x_.add_member("error", error);
        return _x_;
    }
};

struct SendStatus {
    int32 type{0};
    int32 status{0};
    int32 curstatus{0};
    fastring msg;

    void from_json(const co::Json& _x_) {
        type = (int32)_x_.get("type").as_int64();
        status = (int32)_x_.get("status").as_int64();
        curstatus = (int32)_x_.get("curstatus").as_int64();
        msg = _x_.get("msg").as_c_str();
    }

    co::Json as_json() const {
        co::Json _x_;
        _x_.add_member("type", type);
        _x_.add_member("status", status);
        _x_.add_member("curstatus", curstatus);
        _x_.add_member("msg", msg);
        return _x_;
    }
};

struct LoginResultStruct {
    bool result;
    fastring appName;

    void from_json(const co::Json& _x_) {
        result = _x_.get("result").as_bool();
        appName = _x_.get("appName").as_c_str();
    }

    co::Json as_json() const {
        co::Json _x_;
        _x_.add_member("result", result);
        _x_.add_member("appName", appName);
        return _x_;
    }
};
