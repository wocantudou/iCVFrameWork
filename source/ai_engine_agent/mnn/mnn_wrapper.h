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

namespace DNN_WRAPPER {

typedef struct _DNNModel {
    char *res_buffer_[RES_IDX_COUNT];
    uint64_t res_buffer_len_[RES_IDX_COUNT];
    std::shared_ptr<MNN::Interpreter> net_;
} DNNModelHandle;

typedef struct _DNNExecute {
    MNN::Session *session_;
} DNNExecuteHandle;

class MNNSchedule : public AIEngineSchedule<DNNModelHandle, DNNExecuteHandle> {
  public:
    MNNSchedule() {}
    ~MNNSchedule() {}

  public:
    int32_t ai_engine_create_impl(DNNModelHandle *&model_handle) {
        int32_t ret = ICVBASE_NO_ERROR;
        std::string model_json_info =
            std::string(model_handle->res_buffer_[RES_DESC_IDX],
                        model_handle->res_buffer_len_[RES_DESC_IDX]);
        srlog_trace("model info: {}", model_json_info);
        model_handle->net_ = std::shared_ptr<MNN::Interpreter>(
            MNN::Interpreter::createFromBuffer(
                model_handle->res_buffer_[RES_MEM_IDX],
                model_handle->res_buffer_len_[RES_MEM_IDX]));
        return ret;
    }
    int32_t ai_engine_destory_impl(const DNNModelHandle *&model_handle) {
        int32_t ret = ICVBASE_NO_ERROR;
        if (model_handle) {
            model_handle->net_->releaseModel();
            delete model_handle;
            model_handle = NULL;
        }
        return ret;
    }
    int32_t ai_inst_create_impl(const DNNModelHandle *&model_handle,
                                DNNExecuteHandle *&execute_handle) {
        int32_t ret = ICVBASE_NO_ERROR;
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
        return ret;
    }
    int32_t ai_inst_destroy_impl(const DNNModelHandle *&model_handle,
                                 const DNNExecuteHandle *&execute_handle) {
        int32_t ret = ICVBASE_NO_ERROR;
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
    int32_t ai_io_data_prepares_impl(const DNNModelHandle *&model_handle,
                                     const DNNExecuteHandle *&execute_handle,
                                     DnnDataIO &data_io) {
        int32_t ret = ICVBASE_NO_ERROR;
        // input prepares{
        data_io.inputs_.clear();
        auto input_tensors =
            model_handle->net_->getSessionInputAll(execute_handle->session_);
        for (auto &input_tensor_kv : input_tensors) {
            DnnDataInfo tmp;
            tmp.dims_.nbDims = input_tensor_kv.second->dimensions();
            for (int i = 0; i < input_tensor_kv.second->dimensions(); i++) {
                tmp.dims_.d[i] = input_tensor_kv.second->shape()[i];
            }
            tmp.data_.resize(tmp.dims_.count());
            data_io.inputs_.insert(std::make_pair(input_tensor_kv.first, tmp));
        }
        // input prepares}

        // output prepares{
        data_io.outputs_.clear();
        auto output_tensors =
            model_handle->net_->getSessionOutputAll(execute_handle->session_);
        for (auto &output_tensor_kv : output_tensors) {
            DnnDataInfo tmp;
            tmp.dims_.nbDims = output_tensor_kv.second->dimensions();
            for (int i = 0; i < output_tensor_kv.second->dimensions(); i++) {
                tmp.dims_.d[i] = output_tensor_kv.second->shape()[i];
            }
            tmp.data_.resize(tmp.dims_.count());
            data_io.outputs_.insert(
                std::make_pair(output_tensor_kv.first, tmp));
        }
        // output prepares}
        return ret;
    }
    int32_t ai_run_impl(const DNNModelHandle *&model_handle,
                        const DNNExecuteHandle *&execute_handle,
                        DnnDataIO &data_io) {
        int32_t ret = ICVBASE_NO_ERROR;
        // set input
        auto input_tensors =
            model_handle->net_->getSessionInputAll(execute_handle->session_);
        for (auto &input_kv : data_io.inputs_) {
            auto nchw_input = std::make_shared<MNN::Tensor>(
                input_tensors.at(input_kv.first), MNN::Tensor::CAFFE);
            auto nchw_input_host = nchw_input->host<float>();
            ::memcpy(nchw_input_host, input_kv.second.data_.data(),
                     input_kv.second.data_.size() * sizeof(float));
            bool flag = input_tensors.at(input_kv.first)
                            ->copyFromHostTensor(nchw_input.get());
            srlog_error_return(
                flag, ("copyFromHostTensor( {} ) error!", input_kv.first),
                ICVBASE_MEMORY_ERROR);
        }

        // run
        ret = model_handle->net_->runSession(execute_handle->session_);
        srlog_error_return(!ret, ("runSession() error!"),
                           ICVBASE_ALGORITHM_EXECUTION_ERROR);

        // get output
        auto output_tensors =
            model_handle->net_->getSessionOutputAll(execute_handle->session_);

        for (auto &output_tensor_kv : output_tensors) {
            auto nchw_output = std::make_shared<MNN::Tensor>(
                output_tensor_kv.second, MNN::Tensor::CAFFE);
            output_tensor_kv.second->copyToHostTensor(nchw_output.get());
            // get output data
            data_io.outputs_.at(output_tensor_kv.first)
                .data_.resize(nchw_output->elementSize());
            ::memcpy(data_io.outputs_.at(output_tensor_kv.first).data_.data(),
                     nchw_output->host<float>(), nchw_output->size());
            // get output shape
            data_io.outputs_.at(output_tensor_kv.first).dims_.nbDims =
                nchw_output->dimensions();
            for (int i = 0; i < nchw_output->dimensions(); i++) {
                data_io.outputs_.at(output_tensor_kv.first).dims_.d[i] =
                    nchw_output->shape()[i];
            }
        }
        return ret;
    }
    int32_t ai_run_batch_impl(const DNNModelHandle *&model_handle,
                              const DNNExecuteHandle *&execute_handle) {
        int32_t ret = ICVBASE_NO_ERROR;
        // TODO
        return ret;
    }
};

class MNNWrapper
    : public DNNWrapper<RESTYPE, DNNModelHandle, DNNExecuteHandle> {
  public:
    explicit MNNWrapper() { ai_engine_schedule_ = new MNNSchedule(); }
    ~MNNWrapper() {
        if (ai_engine_schedule_) {
            delete ai_engine_schedule_;
            ai_engine_schedule_ = NULL;
        }
    }

  public:
    static const std::map<RESTYPE, const std::string> restype_name_map_;
    static const std::map<RESTYPE, std::vector<RESTYPE>> modtype_restypes_map_;
};

} // namespace DNN_WRAPPER

#define DnnWrapperClass DNN_WRAPPER::MNNWrapper
#define DnnWrapperInst SingletonClass<DnnWrapperClass>::get_inst