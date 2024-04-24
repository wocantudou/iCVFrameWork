#pragma once
#include "ort_wrapper.h" //onnx runtime

class iCVBaseEngine {
  public:
    virtual ~iCVBaseEngine() {}

  public:
    virtual int32_t set_param(const char *para, const char *value) = 0;
    virtual int32_t get_param(const char *para, char *value, int32_t len) = 0;

  protected:
    virtual int32_t preprocess_impl(const RESTYPE res_type,
                                    const void *input_mat, void *target_data,
                                    void *reserved) = 0;
    virtual int32_t postprocess_impl(const RESTYPE res_type, void *target_data,
                                     void *reserved) = 0;
    int32_t forward_impl(const RESTYPE res_type, void *reserved) {
        int32_t ret = ICVBASE_NO_ERROR;
        ret = DnnWrapperInst()->inference(
            res_type, input_data_buffer_, input_data_shape_,
            output_data_buffer_, output_data_shape_);
        return ret;
    }

    int32_t preprocess(const RESTYPE res_type, const void *input_mat,
                       void *target_data, void *reserverd = NULL) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_error_return(
            (0 != DnnWrapperInst()->restype_name_map_.count(res_type)),
            ("preprocess( {} ) failed.", static_cast<int>(res_type)),
            ICVBASE_INPUT_ERROR);
        std::string TAG = DnnWrapperInst()->restype_name_map_.at(res_type) +
                          "_" + __FUNCTION__;
        srlog_perf(LOG_PROF_TAG, TAG);
        iCVProfilerProfiler(TAG);
        ret = preprocess_impl(res_type, input_mat, target_data, reserverd);
        return ret;
    }

    int32_t forward(const RESTYPE res_type, void *reserverd = NULL) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_error_return(
            (0 != DnnWrapperInst()->restype_name_map_.count(res_type)),
            ("forward( {} ) failed.", static_cast<int>(res_type)),
            ICVBASE_INPUT_ERROR);
        std::string TAG = DnnWrapperInst()->restype_name_map_.at(res_type) +
                          "_" + __FUNCTION__;
        srlog_perf(LOG_PROF_TAG, TAG);
        iCVProfilerProfiler(TAG);
        ret = forward_impl(res_type, reserverd);
        return ret;
    }

    int32_t postprocess(const RESTYPE res_type, void *target_data,
                        void *reserverd = NULL) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_error_return(
            (0 != DnnWrapperInst()->restype_name_map_.count(res_type)),
            ("postprocess( {} ) failed.", static_cast<int>(res_type)),
            ICVBASE_INPUT_ERROR);
        std::string TAG = DnnWrapperInst()->restype_name_map_.at(res_type) +
                          "_" + __FUNCTION__;
        srlog_perf(LOG_PROF_TAG, TAG);
        iCVProfilerProfiler(TAG);
        ret = postprocess_impl(res_type, target_data, reserverd);
        return ret;
    }

  public:
    using DataBuf = std::vector<std::vector<float>>;
    using DataShape = std::vector<DnnWrapperClass::Dims>;
    DataBuf input_data_buffer_;
    DataBuf output_data_buffer_;
    DataBuf input_output_data_buffer_;
    DataShape input_data_shape_;
    DataShape output_data_shape_;
};

class iCVBaseDef {
  public:
    virtual ~iCVBaseDef() {}

  public:
    virtual int32_t init(const char *workdir) = 0;
    virtual int32_t fini() = 0;
    virtual int32_t set_param(const char *para, const char *value) = 0;
    virtual int32_t get_param(const char *para, char *value, int32_t len) = 0;
    virtual int32_t process(const void *in, void *out) = 0;
    virtual int32_t get_result(void *out) = 0;

  public:
    virtual int32_t reset_inst() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_info("Not Implement ...");
        return ret;
    }

    virtual int32_t set_input(const void *in) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_info("Not Implement ...");
        return ret;
    }

  public:
    RESTYPE res_type_;
};
