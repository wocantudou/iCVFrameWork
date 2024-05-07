#pragma once
#include "dnn_wrapper/dnn_wrapper.hpp"
#include "iCVBase_errcode.h"
#include "iCVBase_types.h"
#include "iCVLoggerApi.h"
#include "iCVProfiler.hpp"

#define LOG_PROF_TAG "LogProf"

namespace DNN_WRAPPER {

typedef struct _DNNModel {
    char *res_buffer_;
    uint64_t res_buffer_len_;
    uint64_t input_count_;
    // Dims input_shape_;
    uint64_t output_count_;
    // Dims output_shape_;
    int32_t model_type_;
    // void *model_handl;
} DNNModel;
typedef DNNModel *DNNModelHandle;

typedef struct _DNNExecute {
    void *exec_handle_;
    char *forward_data_ptr_;
    int64_t forward_data_size_;
} DNNExecute;
typedef DNNExecute *DNNExecuteHandle;

class MNNWrapper
    : public DNNWrapper<RESTYPE, DNNModelHandle, DNNExecuteHandle> {
  public:
    explicit MNNWrapper() : inited_(false) {}
    ~MNNWrapper() { inited_ = false; }

  public:
    int32_t init(const char *workdir = NULL) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(!inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");

        return ret;
    }
    int32_t fini() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        return ret;
    }
    int32_t add_resource(const RESTYPE &res_type, const std::string &res_path) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        return ret;
    }

    int32_t add_resource(const RESTYPE &res_type, const void *data,
                         const int32_t len) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        return ret;
    }

    int32_t del_resource(const RESTYPE res_type) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        return ret;
    }

    int32_t get_input_shape(const RESTYPE rest_type,
                            std::vector<Dims> &input_dims) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        return ret;
    }

    int32_t get_output_shape(const RESTYPE rest_type,
                             std::vector<Dims> &output_dims) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        return ret;
    }

    int32_t inference(const RESTYPE res_type, const void *input,
                      const Dims &input_dims, std::vector<float> &output,
                      Dims &output_dims, int32_t batch_num = 1) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        return ret;
    }

    int32_t inference(const RESTYPE res_type,
                      const std::vector<std::vector<float>> &input,
                      const std::vector<Dims> &input_dims,
                      std::vector<std::vector<float>> &output,
                      std::vector<Dims> output_dims, int32_t batch_num = 1) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        return ret;
    }

    int32_t create_inst(const RESTYPE res_type) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        return ret;
    }

    int32_t destroy_inst(const RESTYPE res_type) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        return ret;
    }

  private:
    bool inited_;

  public:
    static const std::unordered_map<RESTYPE, const std::string>
        restype_name_map_;
    static const std::unordered_map<RESTYPE, std::vector<RESTYPE>>
        modtype_restypes_map_;
};

} // namespace DNN_WRAPPER

#define DnnWrapperClass DNN_WRAPPER::MNNWrapper
#define DnnWrapperInst SingletonClass<DnnWrapperClass>::get_inst