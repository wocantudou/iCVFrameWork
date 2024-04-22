#pragma once
#include "dp_factory.hpp"
#include "dp_singleton.hpp"
#include "iCVBase_errcode.h"
#include "iCVBase_types.h"
#include "onnxrt_wrapper.h" //onnx runtime
#include <string>
#include <unordered_map>

#define INST_INITIAL (1)

class iCVBaseInstMgr {
  public:
    typedef struct _iCVBaseResInfo {
        std::string res_path;
    } iCVBaseResInfo;

  public:
    explicit iCVBaseInstMgr()
        : inited_(false), inst_id_(INST_INITIAL), cfg_path_("") {}

    virtual ~iCVBaseInstMgr() {
        reset();
        inited_ = false;
    }

  public:
    int init(const std::string &cfg_path) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(!inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        reset();
        cfg_path_ = cfg_path;
        ret = register_res_info();
        srlog_error_return(!ret, ("register_res_info error!"), ret);
        ret = register_res_creator();
        srlog_error_return(!ret, ("register_res_creator error!"), ret);

        ret = DnnWrapperInst()->init();
        srlog_error_return(!ret, ("DnnWrapperInst()->init() error!"), ret);
        inited_ = true;
        return ret;
    }
    int fini() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        ret = DnnWrapperInst()->fini();
        srlog_error_return(!ret, ("DnnWrapperInst()->fini() error!"), ret);
        reset();
        inited_ = false;
        return ret;
    }

    int setup(const RESTYPE res_type, const char *res_path) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_error_return(
            (res_info_umap_.find(res_type) != res_info_umap_.end()),
            ("setup error, ResType : {} not registered!",
             static_cast<int>(res_type)),
            ICVBASE_REGISTER_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        iCVBaseResInfo res_info = res_info_umap_.at(res_type);
        std::string real_res_path;
        if(NULL == res_path){
            real_res_path = res_info.res_path;
        }else{
            real_res_path = res_path;
        }
        return ret;
    }
    int shutdown() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        return ret;
    }

    int create_inst() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        return ret;
    }
    int destroy_inst() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        return ret;
    }
    int reset_inst() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        return ret;
    }
    int run_inst() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        return ret;
    }
    int set_param() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        return ret;
    }
    int get_param() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        return ret;
    }

  protected:
    virtual int register_res_info() = 0;
    virtual int register_res_creator() = 0;

  private:
    void reset() {
        inst_umap_.clear();
        res_info_umap_.clear();
        inst_id_ = INST_INITIAL;
        cfg_path_ = "";
    }

  private:
    bool inited_;
    std::string cfg_path_;
    long inst_id_;
    FactoryClass<iCVBaseInstMgr> factory_;
    std::unordered_map<long, std::unique_ptr<iCVBaseInstMgr>> inst_umap_;
    std::unordered_map<RESTYPE, iCVBaseResInfo> res_info_umap_;
};