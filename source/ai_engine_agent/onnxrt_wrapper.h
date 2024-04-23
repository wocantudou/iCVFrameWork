#pragma once
#include "iCVBase_errcode.h"
#include "iCVBase_types.h"
#include "iCVLoggerApi.h"

#define LOG_PROF_TAG "LogProf"

namespace DNN_WRAPPER {
class OnnxRTWrapper {
  public:
    explicit OnnxRTWrapper() : inited_(false) {}
    ~OnnxRTWrapper() { inited_ = false; }

  public:
    int32_t init(const char *workdir = NULL) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(!inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "OnnxRTWrapper");
        return ret;
    }
    int32_t fini() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "OnnxRTWrapper");
        return ret;
    }
    int32_t add_resource(const RESTYPE &res_type, const std::string &res_path) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "OnnxRTWrapper");
        return ret;
    }

    int32_t add_resource(const RESTYPE &res_type, const void *data,
                         const int32_t len) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "OnnxRTWrapper");
        return ret;
    }

    int32_t del_resource(const RESTYPE &res_type) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "OnnxRTWrapper");
        return ret;
    }

    // int32_t inference(const RESTYPE res_type, const
    // std::vector<std::vector<float> > &input){}

  private:
    bool inited_;
};
} // namespace DNN_WRAPPER

#define DnnWrapperClass DNN_WRAPPER::OnnxRTWrapper
#define DnnWrapperInst SingletonClass<DnnWrapperClass>::get_inst