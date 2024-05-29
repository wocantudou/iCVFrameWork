#pragma once
#include "dnn_wrapper/dnn_wrapper.hpp"
#include "iCVProfiler.hpp"
#include "mnn_schedule.hpp"

#define LOG_PROF_TAG "LogProf"

namespace DNN_WRAPPER {

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
        ret = ai_engine_schedule_.ai_engine_create_impl(model_handle);
        srlog_error_return(
            !ret,
            ("ai_engine_create_impl( {} ) error!", static_cast<int>(res_type)),
            ret);
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
        ret = ai_engine_schedule_.ai_engine_create_impl(model_handle);
        srlog_error_return(
            !ret,
            ("ai_engine_create_impl( {} ) error!", static_cast<int>(res_type)),
            ret);
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
        ret = ai_engine_schedule_.ai_engine_destory_impl(model_handle);
        srlog_error_return(
            !ret,
            ("ai_engine_destory_impl( {} ) error!", static_cast<int>(res_type)),
            ret);
        ret = del_res_from_mem(res_type);
        srlog_error_return(
            !ret, ("del_res_from_mem( {} ) error!", static_cast<int>(res_type)),
            ret);
        return ret;
    }

    int32_t data_io_prepares(const RESTYPE res_type, DnnDataIO &data_io) {
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
        ret = ai_engine_schedule_.ai_io_data_prepares_impl(
            model_handle, inst->execute_handle_, data_io);
        srlog_error_return(!ret,
                           ("ai_io_data_prepares_impl( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);
        ret = push_instpool_map_inst_by_rt(res_type, inst);
        srlog_error_return(!ret,
                           ("push_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);
        return ret;
    }
    int32_t inference(const RESTYPE res_type, DnnDataIO &data_io,
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
        ret = ai_engine_schedule_.ai_run_impl(model_handle,
                                              inst->execute_handle_, data_io);
        srlog_error_return(
            !ret, ("ai_run_impl( {} ) error!", static_cast<int>(res_type)),
            ret);
        ret = push_instpool_map_inst_by_rt(res_type, inst);
        srlog_error_return(!ret,
                           ("push_instpool_map_inst_by_rt( {} ) error!",
                            static_cast<int>(res_type)),
                           ret);
        return ret;
    }

    int32_t create_inst(const RESTYPE res_type, const DnnScheduleConfig &scfg,
                        bool batch_mode = false) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "MNNWrapper");
        srlog_verify_init(is_init_, ICVBASE_INIT_ERROR);
        DNNModelHandle model_handle = dnn_res_map_.at(res_type)->model_handle_;
        DNNExecuteHandle execute_handle = new (std::nothrow) DNNExecute();
        srlog_verify_ptr(execute_handle, ICVBASE_MEMORY_ERROR);
        ret = ai_engine_schedule_.ai_inst_create_impl(model_handle,
                                                      execute_handle);
        srlog_error_return(
            !ret,
            ("ai_inst_create_impl( {} ) error!", static_cast<int>(res_type)),
            ret);
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
        ret = ai_engine_schedule_.ai_inst_destroy_impl(model_handle,
                                                       execute_handle);
        srlog_error_return(
            !ret,
            ("ai_inst_destroy_impl( {} ) error!", static_cast<int>(res_type)),
            ret);
        return ret;
    }

  private:
    AIEngineSchedule ai_engine_schedule_;

  public:
    static const std::map<RESTYPE, const std::string> restype_name_map_;
    static const std::map<RESTYPE, std::vector<RESTYPE>> modtype_restypes_map_;
};

} // namespace DNN_WRAPPER

#define DnnWrapperClass DNN_WRAPPER::MNNWrapper
#define DnnWrapperInst SingletonClass<DnnWrapperClass>::get_inst