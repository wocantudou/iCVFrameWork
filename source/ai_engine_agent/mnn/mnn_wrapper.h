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
} DNNModel;
typedef DNNModel *DNNModelHandle;

typedef struct _DNNExecute {
    MNN::Session *session_;
} DNNExecute;
typedef DNNExecute *DNNExecuteHandle;

class MNNWrapper
    : public DNNWrapper<RESTYPE, DNNModelHandle, DNNExecuteHandle> {
  public:
    explicit MNNWrapper() {}
    ~MNNWrapper() {}

  public:
    int32_t init(const char *workdir = NULL) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        srlog_verify_init(!is_init_, ICVBASE_INIT_ERROR);
        is_init_ = true;
        return ret;
    }
    int32_t fini() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        srlog_verify_init(is_init_, ICVBASE_INIT_ERROR);
        is_init_ = false;
        return ret;
    }
    int32_t add_resource(const RESTYPE res_type, const std::string &res_path) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        srlog_verify_init(is_init_, ICVBASE_INIT_ERROR);
        std::string key = "iflytek_cv3";
        ret = add_res_from_file(res_type, res_path, key.c_str());
        srlog_error_return(!ret,
                           ("add_res_from_file( {}, {} ) error!",
                            static_cast<int>(res_type), res_path),
                           ret);
        DNNModelHandle model_handle = new (std::nothrow) DNNModel();
        srlog_verify_ptr(model_handle, ICVBASE_MEMORY_ERROR);
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
        model_handle->net_ = std::shared_ptr<MNN::Interpreter>(
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
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        srlog_verify_init(is_init_, ICVBASE_INIT_ERROR);
        std::string key = "iflytek_cv3";
        ret = add_res_from_mem(res_type, data, len, key.c_str());
        srlog_error_return(
            !ret, ("add_res_from_mem( {} ) error!", static_cast<int>(res_type)),
            ret);
        DNNModelHandle model_handle = new (std::nothrow) DNNModel();
        srlog_verify_ptr(model_handle, ICVBASE_MEMORY_ERROR);
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
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        srlog_verify_init(is_init_, ICVBASE_INIT_ERROR);
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

    int32_t data_io_prepares(const RESTYPE res_type, DnnDataIO &io_inst) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        srlog_verify_init(is_init_, ICVBASE_INIT_ERROR);
        io_inst.inputs_.clear();
        io_inst.outputs_.clear();

        DNNModelHandle model_handle = dnn_res_map_.at(res_type)->model_handle_;
        std::shared_ptr<DnnInst> inst;
        ret = pop_instpool_map_inst_by_rt(res_type, inst);
        srlog_error_return(!ret,
                           ("pop_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);
        // input prepares{
        auto input_tensors = model_handle->net_->getSessionInputAll(
            inst->execute_handle_->session_);
        for (auto &input_tensor_kv : input_tensors) {
            DnnDataInfo tmp;
            tmp.dims_.nbDims = input_tensor_kv.second->dimensions();
            for (int i = 0; i < input_tensor_kv.second->dimensions(); i++) {
                tmp.dims_.d[i] = input_tensor_kv.second->shape()[i];
            }
            tmp.data_.resize(tmp.dims_.count());
            io_inst.inputs_.insert(std::make_pair(input_tensor_kv.first, tmp));
        }
        // input prepares}

        // output prepares{
        auto output_tensors = model_handle->net_->getSessionOutputAll(
            inst->execute_handle_->session_);
        for (auto &output_tensor_kv : output_tensors) {
            DnnDataInfo tmp;
            tmp.dims_.nbDims = output_tensor_kv.second->dimensions();
            for (int i = 0; i < output_tensor_kv.second->dimensions(); i++) {
                tmp.dims_.d[i] = output_tensor_kv.second->shape()[i];
            }
            tmp.data_.resize(tmp.dims_.count());
            io_inst.outputs_.insert(
                std::make_pair(output_tensor_kv.first, tmp));
        }
        // output prepares}
        ret = push_instpool_map_inst_by_rt(res_type, inst);
        srlog_error_return(!ret,
                           ("push_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);
        return ret;
    }
    int32_t inference(const RESTYPE res_type, DnnDataIO &io_inst,
                      bool batch_mode = false) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        srlog_verify_init(is_init_, ICVBASE_INIT_ERROR);
        DNNModelHandle model_handle = dnn_res_map_.at(res_type)->model_handle_;
        std::shared_ptr<DnnInst> inst;
        ret = pop_instpool_map_inst_by_rt(res_type, inst);
        srlog_error_return(!ret,
                           ("pop_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);
        // set input
        auto input_tensors = model_handle->net_->getSessionInputAll(
            inst->execute_handle_->session_);
        for (auto &input_kv : io_inst.inputs_) {
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
        ret = model_handle->net_->runSession(inst->execute_handle_->session_);
        srlog_error_return(
            !ret, ("runSession( {} ) error!", static_cast<int>(res_type)),
            ICVBASE_ALGORITHM_EXECUTION_ERROR);

        // get output
        auto output_tensors = model_handle->net_->getSessionOutputAll(
            inst->execute_handle_->session_);

        for (auto &output_tensor_kv : output_tensors) {
            auto nchw_output = std::make_shared<MNN::Tensor>(
                output_tensor_kv.second, MNN::Tensor::CAFFE);
            output_tensor_kv.second->copyToHostTensor(nchw_output.get());
            // get output data
            io_inst.outputs_.at(output_tensor_kv.first)
                .data_.resize(nchw_output->elementSize());
            ::memcpy(io_inst.outputs_.at(output_tensor_kv.first).data_.data(),
                     nchw_output->host<float>(), nchw_output->size());
            // get output shape
            io_inst.outputs_.at(output_tensor_kv.first).dims_.nbDims =
                nchw_output->dimensions();
            for (int i = 0; i < nchw_output->dimensions(); i++) {
                io_inst.outputs_.at(output_tensor_kv.first).dims_.d[i] =
                    nchw_output->shape()[i];
            }
        }

        ret = push_instpool_map_inst_by_rt(res_type, inst);
        srlog_error_return(!ret,
                           ("push_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);
        return ret;
    }

    int32_t create_inst(const RESTYPE res_type, DnnScheduleConfig scfg,
                        bool batch_mode = false) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        srlog_verify_init(is_init_, ICVBASE_INIT_ERROR);
        DNNModelHandle model_handle = dnn_res_map_.at(res_type)->model_handle_;
        DNNExecuteHandle execute_handle = new (std::nothrow) DNNExecute();
        srlog_verify_ptr(execute_handle, ICVBASE_MEMORY_ERROR);
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
        ret = create_instpool_map_inst_by_rt(res_type, execute_handle,
                                             batch_mode);
        srlog_error_return(!ret,
                           ("create_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);
        return ret;
    }

    int32_t destroy_inst(const RESTYPE res_type) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        srlog_verify_init(is_init_, ICVBASE_INIT_ERROR);
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

  public:
    static const std::map<RESTYPE, const std::string> restype_name_map_;
    static const std::map<RESTYPE, std::vector<RESTYPE>> modtype_restypes_map_;
};

} // namespace DNN_WRAPPER

#define DnnWrapperClass DNN_WRAPPER::MNNWrapper
#define DnnWrapperInst SingletonClass<DnnWrapperClass>::get_inst