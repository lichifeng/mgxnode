#include <napi.h>

#include <cstdio>

#include "MgxParser.h"

static Napi::Object Method(const Napi::CallbackInfo &info) {
    // Napi::Env is the opaque data structure containing the environment in which the request is being run.
    // We will need this env when we want to create any new objects inside of the node.js environment
    Napi::Env env = info.Env();
    Napi::Object config;
    MgxParser::Settings _;

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "No record or config object specified.").ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    } else {
        if (info[0].IsObject()) {
            config = info[0].As<Napi::Object>();
        } else {
            config = Napi::Object::New(env);
        }
    }

    // extract parsing settings
    if (config.Has("input")) {
        if (config.Get("input").IsBuffer()) {
            auto rawbuf = config.Get("input").As<Napi::Buffer<uint8_t>>();
            _.input_stream = rawbuf.Data();
            _.input_size = rawbuf.Length();
        } else if (config.Get("input").IsString()) {
            _.input_path = config.Get("input").ToString().Utf8Value();
        }
    }
    if (config.Has("filepath") && config.Get("filepath").IsString()) {
        _.input_path = config.Get("filepath").ToString().Utf8Value();
    }
    if (config.Has("map") && config.Get("map").IsBoolean() && config.Get("map").ToBoolean().Value()) {
        if (config.Has("hd") && config.Get("hd").IsBoolean()) {
            _.map_type = HD_MAP;
        } else {
            _.map_type = NORMAL_MAP;
        }
        if (config.Has("mapWidth") && config.Get("mapWidth").IsNumber()) {
            _.map_width = config.Get("mapWidth").ToNumber();
        }
        if (config.Has("mapHeight") && config.Get("mapHeight").IsNumber()) {
            _.map_height = config.Get("mapHeight").ToNumber();
        }
    }
    if (config.Has("mapName") && config.Get("mapName").IsString()) {
        _.map_name = config.Get("mapName").ToString().Utf8Value();
    }
    if (config.Has("header") && config.Get("header").IsString()) {
        _.header_path = config.Get("header").ToString().Utf8Value();
    }
    if (config.Has("body") && config.Get("body").IsString()) {
        _.body_path = config.Get("body").ToString().Utf8Value();
    }
    if (config.Has("fullParse") && config.Get("fullParse").IsBoolean()) {
        _.full_parse = config.Get("fullParse").ToBoolean().Value();
    }
    if (config.Has("md5") && config.Get("md5").IsBoolean()) {
        _.md5 = config.Get("md5").ToBoolean().Value();
    }

    // check if map buffer requested
    char *out_buffer = nullptr;
    std::size_t map_size = 0;
    if (_.map_type && _.map_name.empty()) {
        _.map_dest = open_memstream(&out_buffer, &map_size);
    }

    // check if need unzip
    char *unzip_dest = nullptr;
    std::size_t unzip_size = 0;
    if (config.Has("unzip") && config.Get("unzip").IsString()) {
        std::string unzip_type = config.Get("unzip").ToString().Utf8Value();
        if ("buffer" == unzip_type) {
            _.unzip_buffer = &unzip_dest;
            _.unzip_size_ptr = &unzip_size;
        } else {
            _.unzip = unzip_type;
        }
    }

    // do parsing
    auto recinfo = MgxParser::parse(_);
    if (_.map_dest)
        fclose(_.map_dest);

    // Return a new javascript string that we copy-construct inside of the node.js environment
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("result", recinfo);

    // populate map buffer if exists
    if (map_size > 0 && out_buffer) {
        auto map_buffer = Napi::Buffer<char>::New(env, out_buffer, map_size, [](napi_env e, char *b) {
            if (b)
                free(b);
        });
        obj.Set("mapbuffer", map_buffer);
    }

    // populate unzip buffer if required
    if (unzip_dest && unzip_size) {
            auto unzip_buffer = Napi::Buffer<char>::New(env, unzip_dest, unzip_size, [](napi_env e, char *b) {
                if (b)
                    free(b);
            });
            obj.Set("unzipbuffer", unzip_buffer);
    }

    return obj;
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("parse", Napi::Function::New(env, Method));
    return exports;
}

NODE_API_MODULE(mgxnode, Init)
