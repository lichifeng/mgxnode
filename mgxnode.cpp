#include <napi.h>
#include "libs/MgxParser/src/include/MgxParser.h"

static Napi::String Method(const Napi::CallbackInfo &info)
{
    // Napi::Env is the opaque data structure containing the environment in which the request is being run.
    // We will need this env when we want to create any new objects inside of the node.js environment
    Napi::Env env = info.Env();

    std::string retval;
    if (info[0].IsBuffer())
    {
        Napi::Buffer<uint8_t> rawBuf =  info[0].As<Napi::Buffer<uint8_t>>();
        retval = MgxParser::parse(rawBuf.Data(), rawBuf.ByteLength());
    }
    else
    {
        std::string recfile = info[0].IsString() ? info[0].ToString().Utf8Value() : "-";
        retval = MgxParser::parse(recfile);
    }

    // Return a new javascript string that we copy-construct inside of the node.js environment
    return Napi::String::New(env, retval);
}

static Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "parse"),
                Napi::Function::New(env, Method));
    return exports;
}

NODE_API_MODULE(mgxnode, Init)
