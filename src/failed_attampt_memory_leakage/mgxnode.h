#ifndef MGXNODE_MGXNODE_H_
#define MGXNODE_MGXNODE_H_

#include <string>
#include <memory>

#include "analyzers/default/analyzer.h"
#include "napi.h"

class MgxNode : public Napi::ObjectWrap<MgxNode> {
   public:
    MgxNode(const Napi::CallbackInfo &);
    inline ~MgxNode() { if(mapbuffer_) free(mapbuffer_); }
    Napi::Value GetResult(const Napi::CallbackInfo &);
    Napi::Value GetMapBuffer(const Napi::CallbackInfo& info);
    // Napi::Buffer<uint8_t> GetMapBuffer(Napi::Object& map_settings);
    // Napi::String ExtractHeader(const std::string& filename);
    // Napi::String ExtractBody(const std::string& filename);

    static Napi::Function GetClass(Napi::Env);

   private:
    std::unique_ptr<DefaultAnalyzer> analyzer_;
    std::string filepath_;
    Napi::Object config_;
    bool tryfix_ = false;
    std::string rawencoding_;
    std::string result_;
    Napi::Env env_;
    char* mapbuffer_ = nullptr;
};

#endif  // MGXNODE_MGXNODE_H_