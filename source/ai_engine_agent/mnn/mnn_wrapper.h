#pragma once
#include "dnn_wrapper/dnn_wrapper.hpp"
#include "iCVBase_errcode.h"
#include "iCVBase_types.h"
#include "iCVLoggerApi.h"
#include "iCVProfiler.hpp"
// MNN
#include "MNN/Interpreter.hpp"
#include "MNN/MNNForwardType.h"

#define LOG_PROF_TAG "LogProf"

#define RES_MEM_IDX 0
#define RES_DESC_IDX 1
#define RES_IDX_COUNT 2

namespace DNN_WRAPPER {

typedef struct _DNNModel {
    char *res_buffer_[RES_IDX_COUNT];
    uint64_t res_buffer_len_[RES_IDX_COUNT];
    std::shared_ptr<MNN::Interpreter> net_;
    // uint64_t input_count_;
    // // Dims input_shape_;
    // uint64_t output_count_;
    // // Dims output_shape_;
    // int32_t model_type_;
} DNNModel;
typedef DNNModel *DNNModelHandle;

typedef struct _DNNExecute {
    MNN::Session *session_;
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
    int32_t add_resource(const RESTYPE res_type, const std::string &res_path) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        std::string key = "iflytek_cv3";
        ret = add_res_from_file(res_type, res_path, key.c_str());
        srlog_error_return(!ret,
                           ("add_res_from_file( {}, {} ) error!",
                            static_cast<int>(res_type), res_path),
                           ret);
        DNNModelHandle model_handle = new (std::nothrow) DNNModel();
        srlog_verify_ptr(model_handle, ICVBASE_MEMORY_ALLOCATION_ERROR);
        for (int idx = 0; idx < RES_IDX_COUNT; idx++) {
            model_handle->res_buffer_[idx] =
                dnn_res_map_.at(res_type)->get_mem_by_idx(idx);
            model_handle->res_buffer_len_[idx] =
                dnn_res_map_.at(res_type)->get_mem_len_by_idx(idx);
        }
        std::string model_json_info =
            std::string(model_handle->res_buffer_[RES_DESC_IDX],
                        model_handle->res_buffer_len_[RES_DESC_IDX]);
        srlog_trace("{} model info: {}", static_cast<int>(res_type),
                    model_json_info);
        std::shared_ptr<MNN::Interpreter> net =
            std::shared_ptr<MNN::Interpreter>(
                MNN::Interpreter::createFromBuffer(
                    model_handle->res_buffer_[RES_MEM_IDX],
                    model_handle->res_buffer_len_[RES_MEM_IDX]));
        ret = push_model_handle(res_type, model_handle);
        srlog_error_return(
            !ret,
            ("push_model_handle( {} ) error!", static_cast<int>(res_type)),
            ret);
        return ret;
    }

    int32_t add_resource(const RESTYPE res_type, const void *data,
                         const int32_t len) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        std::string key = "iflytek_cv3";
        ret = add_res_from_mem(res_type, data, len, key.c_str());
        srlog_error_return(
            !ret, ("add_res_from_mem( {} ) error!", static_cast<int>(res_type)),
            ret);
        DNNModelHandle model_handle = new (std::nothrow) DNNModel();
        srlog_verify_ptr(model_handle, ICVBASE_MEMORY_ALLOCATION_ERROR);
        for (int idx = 0; idx < RES_IDX_COUNT; idx++) {
            model_handle->res_buffer_[idx] =
                dnn_res_map_.at(res_type)->get_mem_by_idx(idx);
            model_handle->res_buffer_len_[idx] =
                dnn_res_map_.at(res_type)->get_mem_len_by_idx(idx);
        }
        std::string model_json_info =
            std::string(model_handle->res_buffer_[RES_DESC_IDX],
                        model_handle->res_buffer_len_[RES_DESC_IDX]);
        srlog_trace("{} model info: {}", static_cast<int>(res_type),
                    model_json_info);
        std::shared_ptr<MNN::Interpreter> net =
            std::shared_ptr<MNN::Interpreter>(
                MNN::Interpreter::createFromBuffer(
                    model_handle->res_buffer_[RES_MEM_IDX],
                    model_handle->res_buffer_len_[RES_MEM_IDX]));
        ret = push_model_handle(res_type, model_handle);
        srlog_error_return(
            !ret,
            ("push_model_handle( {} ) error!", static_cast<int>(res_type)),
            ret);
        return ret;
    }

    int32_t del_resource(const RESTYPE res_type) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        DNNModelHandle model_handle;
        ret = pull_model_handle(res_type, model_handle);
        srlog_error_return(
            !ret,
            ("pull_model_handle( {} ) error!", static_cast<int>(res_type)),
            ret);

        if (model_handle) {
            model_handle->net_->releaseModel();
            delete model_handle;
            model_handle = NULL;
        }
        ret = del_res_from_mem(res_type);
        srlog_error_return(
            !ret, ("del_res_from_mem( {} ) error!", static_cast<int>(res_type)),
            ret);
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

    int32_t inference(const RESTYPE res_type,
                      const std::vector<std::vector<float>> &inputs,
                      const std::vector<Dims> &input_dims,
                      std::vector<std::vector<float>> &outputs,
                      std::vector<Dims> &output_dims, int32_t batch_num = 1) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        DNNModelHandle model_handle = dnn_res_map_.at(res_type)->model_handle_;
        std::shared_ptr<DnnInst> inst;
        ret = pop_instpool_map_inst_by_rt(res_type, inst);
        auto input_tensors = model_handle->net_->getSessionInputAll(
            inst->execute_handle_->session_);

        ret = push_instpool_map_inst_by_rt(res_type, inst);
        return ret;
    }

    int32_t create_inst(const RESTYPE res_type) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        DNNModelHandle model_handle = dnn_res_map_.at(res_type)->model_handle_;
        DNNExecuteHandle execute_handle = new (std::nothrow) DNNExecute();
        srlog_verify_ptr(execute_handle, ICVBASE_MEMORY_ALLOCATION_ERROR);
        MNN::ScheduleConfig config;
        config.numThread = 1;
        config.backupType = MNN_FORWARD_CPU;
        MNN::BackendConfig backendConfig;
        backendConfig.memory =
            MNN::BackendConfig::MemoryMode::Memory_Normal; // 内存
        backendConfig.power =
            MNN::BackendConfig::PowerMode::Power_Normal; // 功耗
        backendConfig.precision =
            MNN::BackendConfig::PrecisionMode::Precision_Normal; // 精度
        config.backendConfig = &backendConfig;
        execute_handle->session_ = model_handle->net_->createSession(config);
        ret = create_instpool_map_inst_by_rt(res_type, execute_handle);
        srlog_error_return(!ret,
                           ("create_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);
        return ret;
    }

    int32_t destroy_inst(const RESTYPE res_type) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        DNNModelHandle model_handle = dnn_res_map_.at(res_type)->model_handle_;
        DNNExecuteHandle execute_handle;
        ret = destroy_instpool_map_inst_by_rt(res_type, execute_handle);
        srlog_error_return(!ret,
                           ("destroy_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);

        if (execute_handle) {
            bool flag =
                model_handle->net_->releaseSession(execute_handle->session_);
            srlog_error_return(!ret, ("releaseSession() error!"),
                               ICVBASE_INSTANCE_ERROR);
            delete execute_handle;
            execute_handle = NULL;
        }
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