#include <napi.h>
#include "MgxParser.h"


static Napi::String Method(const Napi::CallbackInfo& info) {
  // Napi::Env is the opaque data structure containing the environment in which the request is being run. 
  // We will need this env when we want to create any new objects inside of the node.js environment
  Napi::Env env = info.Env();
  
  // Retrieve input filename
  std::string recfile = info[0].IsString() ? info[0].ToString().Utf8Value() : "-";

  std::string retval = MgxParser::parse(recfile);
  
  // Return a new javascript string that we copy-construct inside of the node.js environment
  return Napi::String::New(env, retval);
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "parse"),
              Napi::Function::New(env, Method));
  return exports;
}

NODE_API_MODULE(mgxnode, Init)
