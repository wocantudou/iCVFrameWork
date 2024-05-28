#pragma once
#include "dnn_wrapper/dnn_wrapper.hpp"
#include "iCVBase_errcode.h"
#include "iCVBase_types.h"
#include "iCVLoggerApi.h"
#include "iCVProfiler.hpp"
// ORT Header{

// ORT Header}

#define LOG_PROF_TAG "LogProf"

#define RES_MEM_IDX 0
#define RES_DESC_IDX 1
#define RES_IDX_COUNT 2

namespace DNN_WRAPPER {

typedef struct _DNNModel {
    char *res_buffer_[RES_IDX_COUNT];
    uint64_t res_buffer_len_[RES_IDX_COUNT];
    // ORT ModelHandle define

} DNNModel;
typedef DNNModel *DNNModelHandle;

typedef struct _DNNExecute {
    // ORT ExecuteHandle define

} DNNExecute;
typedef DNNExecute *DNNExecuteHandle;

class ORTWrapper
    : public DNNWrapper<RESTYPE, DNNModelHandle, DNNExecuteHandle> {
  public:
    explicit ORTWrapper() : inited_(false) {}
    ~ORTWrapper() { inited_ = false; }

  public:
    int32_t init(const char *workdir = NULL) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "ORTWrapper");
        srlog_verify_init(!inited_, ICVBASE_INIT_ERROR);
        inited_ = true;
        return ret;
    }
    int32_t fini() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "ORTWrapper");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        inited_ = false;
        return ret;
    }
    int32_t add_resource(const RESTYPE res_type, const std::string &res_path) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "ORTWrapper");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
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
        // ModelHandle Create{

        // ModelHandle Create}
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
        srlog_perf(LOG_PROF_TAG, "ORTWrapper");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
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
        // ModelHandle Create{

        // ModelHandle Create}
        ret = push_model_handle(res_type, model_handle);
        srlog_error_return(
            !ret,
            ("push_model_handle( {} ) error!", static_cast<int>(res_type)),
            ret);
        return ret;
    }

    int32_t del_resource(const RESTYPE res_type) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "ORTWrapper");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        DNNModelHandle model_handle;
        ret = pull_model_handle(res_type, model_handle);
        srlog_error_return(
            !ret,
            ("pull_model_handle( {} ) error!", static_cast<int>(res_type)),
            ret);

        if (model_handle) {
            // ModelHandle Destory{

            // ModelHandle Destory}
            delete model_handle;
            model_handle = NULL;
        }
        ret = del_res_from_mem(res_type);
        srlog_error_return(
            !ret, ("del_res_from_mem( {} ) error!", static_cast<int>(res_type)),
            ret);
        return ret;
    }

    int32_t prepare_inputs(const RESTYPE res_type,
                           std::map<std::string, DnnDataInfo> &inputs) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "ORTWrapper");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        inputs.clear();
        DNNModelHandle model_handle = dnn_res_map_.at(res_type)->model_handle_;
        std::shared_ptr<DnnInst> inst;
        ret = pop_instpool_map_inst_by_rt(res_type, inst);
        srlog_error_return(!ret,
                           ("pop_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);
        // Prepare inputs{

        // Prepare inputs}
        ret = push_instpool_map_inst_by_rt(res_type, inst);
        srlog_error_return(!ret,
                           ("push_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);
        return ret;
    }

    int32_t inference(const RESTYPE res_type,
                      const std::map<std::string, DnnDataInfo> &inputs,
                      std::map<std::string, DnnDataInfo> &outputs,
                      int32_t batch_num = 1) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "ORTWrapper");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        DNNModelHandle model_handle = dnn_res_map_.at(res_type)->model_handle_;
        std::shared_ptr<DnnInst> inst;
        ret = pop_instpool_map_inst_by_rt(res_type, inst);
        srlog_error_return(!ret,
                           ("pop_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);

        // set input

        // run

        // get output

        ret = push_instpool_map_inst_by_rt(res_type, inst);
        srlog_error_return(!ret,
                           ("push_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);
        return ret;
    }

    int32_t create_inst(const RESTYPE res_type) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "ORTWrapper");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        DNNModelHandle model_handle = dnn_res_map_.at(res_type)->model_handle_;
        DNNExecuteHandle execute_handle = new (std::nothrow) DNNExecute();
        srlog_verify_ptr(execute_handle, ICVBASE_MEMORY_ERROR);
        // ExecuteHandle Create{

        // ExecuteHandle Create}
        ret = create_instpool_map_inst_by_rt(res_type, execute_handle);
        srlog_error_return(!ret,
                           ("create_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);
        return ret;
    }

    int32_t destroy_inst(const RESTYPE res_type) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "ORTWrapper");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        DNNModelHandle model_handle = dnn_res_map_.at(res_type)->model_handle_;
        DNNExecuteHandle execute_handle;
        ret = destroy_instpool_map_inst_by_rt(res_type, execute_handle);
        srlog_error_return(!ret,
                           ("destroy_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);

        if (execute_handle) {
            // ExecuteHandle Destory{

            // ExecuteHandle Destory}
            delete execute_handle;
            execute_handle = NULL;
        }
        return ret;
    }

  private:
    bool inited_;

  public:
    static const std::map<RESTYPE, const std::string> restype_name_map_;
    static const std::map<RESTYPE, std::vector<RESTYPE>> modtype_restypes_map_;
};

} // namespace DNN_WRAPPER

#define DnnWrapperClass DNN_WRAPPER::ORTWrapper
#define DnnWrapperInst SingletonClass<DnnWrapperClass>::get_inst