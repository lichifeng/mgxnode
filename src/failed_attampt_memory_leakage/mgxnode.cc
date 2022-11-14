#define _GUN_SOURCE
#include "mgxnode.h"

#include <cstdio>

// \todo ugly solution, fix this later
std::string _parse(DefaultAnalyzer& a, int map_type, std::string&& map_name, bool extract);

MgxNode::MgxNode(const Napi::CallbackInfo& info)
    : ObjectWrap<MgxNode>(info), env_(info.Env()) {
    // get input parameters
    if (info.Length() < 1) {
        Napi::TypeError::New(env_, "No record or config object specified.").ThrowAsJavaScriptException();
        return;
    } else {
        config_ = info[0].As<Napi::Object>();
    }

    // extract valid config parameters
    if (config_ && config_.Has("tryfix") && config_.Get("tryfix").IsBoolean())
        tryfix_ = config_.Get("tryfix").As<Napi::Boolean>();
    if (config_ && config_.Has("file") && config_.Get("file").IsString()) {
        filepath_ = config_.Get("file").ToString().Utf8Value();
    }
    if (config_ && config_.Has("rawencoding") && config_.Get("rawencoding").IsString())
        rawencoding_ = config_.Get("rawencoding").ToString().Utf8Value();

    // load record
    if (config_ && config_.Has("buffer") && config_.Get("buffer").IsBuffer()) {
        auto buffer = config_.Get("buffer").As<Napi::Buffer<uint8_t>>();
        analyzer_ = std::make_unique<DefaultAnalyzer>(
            buffer.Data(), buffer.ByteLength(),
            filepath_.empty() ? "" : filepath_);
    } else {
        analyzer_ = std::make_unique<DefaultAnalyzer>(filepath_.empty() ? "" : filepath_);
    }
}

Napi::Value MgxNode::GetResult(const Napi::CallbackInfo& info) {
    if (result_.empty()) {
        result_ = _parse(*analyzer_, 0, "", false);
    }
    return Napi::String::New(env_, result_);
}

Napi::Value MgxNode::GetMapBuffer(const Napi::CallbackInfo& info) {
    std::size_t buffer_size;
    FILE* tmpout;
    uint32_t width = 300, height = 150;
    bool hd = false;

    tmpout = open_memstream(&mapbuffer_, &buffer_size);
    if (tmpout == NULL)
        return Napi::Buffer<char>();

    if (info.Length() >= 1) {
        auto config = info[0].As<Napi::Object>();
        if (config && config.Has("hd") && config.Get("hd").IsBoolean())
            hd = config.Get("hd").As<Napi::Boolean>();
        if (config && config.Has("width") && config.Get("width").IsNumber())
            width = config.Get("width").As<Napi::Number>();
        if (config && config.Has("height") && config.Get("height").IsNumber())
            height = config.Get("height").As<Napi::Number>();
    }

    analyzer_->DrawMap(tmpout, width, height, hd);
    fclose(tmpout);
    return Napi::Buffer<char>::New(env_, mapbuffer_, buffer_size);
}

Napi::Function MgxNode::GetClass(Napi::Env env) {
    return DefineClass(env, "MgxNode", {InstanceMethod("result", &MgxNode::GetResult), InstanceMethod("map", &MgxNode::GetMapBuffer)});
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("MgxNode", MgxNode::GetClass(env));
    return exports;
}

//NODE_API_MODULE(mgxnode, Init)