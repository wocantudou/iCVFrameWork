#pragma once
#include "dp_factory.hpp"
#include "dp_singleton.hpp"

#include "iCVBase_def.h"
#include <string>
#include <unordered_map>

#define INST_ID_INITIAL (1)

class iCVBaseInstMgr {
  public:
    typedef struct _iCVBaseResInfo {
        std::string res_path;
    } iCVBaseResInfo;

  public:
    explicit iCVBaseInstMgr()
        : inited_(false), inst_id_(INST_ID_INITIAL), cfg_path_("") {}

    virtual ~iCVBaseInstMgr() {
        reset();
        inited_ = false;
    }

  public:
    int32_t init(const std::string &cfg_path) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        srlog_verify_init(!inited_, ICVBASE_INIT_ERROR);
        reset();
        cfg_path_ = cfg_path;
        ret = res_inst_register();
        srlog_error_return(!ret, ("res_inst_register error!"), ret);
        ret = DnnWrapperInst()->init();
        srlog_error_return(!ret, ("DnnWrapperInst()->init() error!"), ret);
        inited_ = true;
        return ret;
    }
    int32_t fini() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        ret = DnnWrapperInst()->fini();
        srlog_error_return(!ret, ("DnnWrapperInst()->fini() error!"), ret);
        reset();
        inited_ = false;
        return ret;
    }

    int32_t setup(const RESTYPE res_type, const char *res_path) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_error_return(
            (0 != res_info_umap_.count(res_type) ||
             0 != DnnWrapperInst()->modtype_restypes_map_.count(res_type)),
            ("setup error, ResType : {} not registered!",
             static_cast<int32_t>(res_type)),
            ICVBASE_REGISTER_ERROR);
        iCVBaseResInfo res_info = res_info_umap_.at(res_type);
        std::string real_res_path;
        if (NULL == res_path) {
            real_res_path = res_info.res_path;
        } else {
            real_res_path = res_path;
        }
        if (0 == DnnWrapperInst()->modtype_restypes_map_.count(res_type)) {
            ret = DnnWrapperInst()->add_resource(res_type, real_res_path);
            srlog_error_return(
                !ret,
                ("DnnWrapperInst()->add_resource( {}, {} ) error!",
                 (static_cast<int32_t>(res_type), real_res_path)),
                ret);
        }
        return ret;
    }

    int32_t setup(const RESTYPE res_type, const void *data, const int32_t len) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_error_return(
            (0 != res_info_umap_.count(res_type) ||
             0 != DnnWrapperInst()->modtype_restypes_map_.count(res_type)),
            ("setup error, ResType : {} not registered!",
             static_cast<int32_t>(res_type)),
            ICVBASE_REGISTER_ERROR);
        if (0 == DnnWrapperInst()->modtype_restypes_map_.count(res_type)) {
            ret = DnnWrapperInst()->add_resource(res_type, data, len);
            srlog_error_return(
                !ret,
                ("DnnWrapperInst()->add_resource( {}, {}, {} ) error!",
                 (static_cast<int32_t>(res_type), data, len)),
                ret);
        }
        return ret;
    }

    int32_t shutdown(const RESTYPE res_type) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_error_return(
            (0 != res_info_umap_.count(res_type) ||
             0 != DnnWrapperInst()->modtype_restypes_map_.count(res_type)),
            ("shutdown error, ResType : {} not registered!",
             static_cast<int32_t>(res_type)),
            ICVBASE_REGISTER_ERROR);
        if (0 == DnnWrapperInst()->modtype_restypes_map_.count(res_type)) {
            ret = DnnWrapperInst()->del_resource(res_type);
            srlog_error_return(!ret,
                               ("DnnWrapperInst()->del_resource( {} ) error!",
                                (static_cast<int32_t>(res_type))),
                               ret);
        }
        return ret;
    }

    int32_t create_inst(const RESTYPE res_type, long &inst_id) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_error_return(
            (0 != res_info_umap_.count(res_type) ||
             0 != DnnWrapperInst()->modtype_restypes_map_.count(res_type)),
            ("create_inst error, ResType : {} not registered!",
             static_cast<int32_t>(res_type)),
            ICVBASE_REGISTER_ERROR);
        {
            std::lock_guard<std::mutex> lock(mtx_);
            inst_umap_.insert(std::pair<long, std::unique_ptr<iCVBaseDef>>(
                inst_id_, factory_.Create(res_type)));
            inst_id = inst_id_++;
        }
        auto &inst = inst_umap_.at(inst_id);
        srlog_verify_inst(inst, ICVBASE_INSTANCE_ERROR);
        inst->res_type_ = res_type;
        ret = inst->init(cfg_path_.c_str());
        srlog_error_return(!ret, ("inst->init( {} ) failed.", cfg_path_), ret);

        if (0 != DnnWrapperInst()->modtype_restypes_map_.count(res_type)) {
            for (const auto &each_res :
                 DnnWrapperInst()->modtype_restypes_map_.at(res_type)) {
                ret = DnnWrapperInst()->create_inst(each_res);
                srlog_error_return(
                    !ret,
                    ("DnnWrapperInst()->create_inst( {} ) failed.",
                     static_cast<int>(each_res)),
                    ret);
            }
        } else {
            ret = DnnWrapperInst()->create_inst(res_type);
            srlog_error_return(!ret,
                               ("DnnWrapperInst()->create_inst( {} ) failed.",
                                static_cast<int>(res_type)),
                               ret);
        }
        return ret;
    }
    int32_t destroy_inst(const long inst_id) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_error_return((0 != inst_umap_.count(inst_id)),
                           ("destroy_inst( {} ) failed.", inst_id),
                           ICVBASE_INPUT_ERROR);
        auto &inst = inst_umap_.at(inst_id);
        srlog_verify_inst(inst, ICVBASE_INSTANCE_ERROR);
        const RESTYPE res_type = inst->res_type_;
        if (0 != DnnWrapperInst()->modtype_restypes_map_.count(res_type)) {
            for (const auto &each_res :
                 DnnWrapperInst()->modtype_restypes_map_.at(res_type)) {
                ret = DnnWrapperInst()->destroy_inst(each_res);
                srlog_error_return(
                    !ret,
                    ("DnnWrapperInst()->destroy_inst( {} ) failed.",
                     static_cast<int>(each_res)),
                    ret);
            }
        } else {
            ret = DnnWrapperInst()->destroy_inst(res_type);
            srlog_error_return(!ret,
                               ("DnnWrapperInst()->destroy_inst( {} ) failed.",
                                static_cast<int>(res_type)),
                               ret);
        }
        ret = inst->fini();
        srlog_error_return(!ret, ("inst->fini() failed."), ret);
        inst_umap_.erase(inst_id);
        return ret;
    }
    int32_t reset_inst(const long inst_id) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_error_return((0 != inst_umap_.count(inst_id)),
                           ("reset_inst( {} ) failed.", inst_id),
                           ICVBASE_INPUT_ERROR);
        auto &inst = inst_umap_.at(inst_id);
        srlog_verify_inst(inst, ICVBASE_INSTANCE_ERROR);
        ret = inst->reset_inst();
        return ret;
    }
    int32_t run_inst(const long inst_id, const void *in_data, void *out_data) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_verify_ptr(in_data, ICVBASE_INPUT_ERROR);
        srlog_verify_ptr(out_data, ICVBASE_INPUT_ERROR);
        srlog_error_return((0 != inst_umap_.count(inst_id)),
                           ("run_inst( {} ) failed.", inst_id),
                           ICVBASE_INPUT_ERROR);
        auto &inst = inst_umap_.at(inst_id);
        srlog_verify_inst(inst, ICVBASE_INSTANCE_ERROR);
        ret = inst->process(in_data, out_data);
        return ret;
    }
    int32_t set_param(const long inst_id, const char *param,
                      const char *value) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_verify_ptr(param, ICVBASE_INPUT_ERROR);
        srlog_verify_ptr(value, ICVBASE_INPUT_ERROR);
        srlog_error_return((0 != inst_umap_.count(inst_id)),
                           ("set_param( {} ) failed.", inst_id),
                           ICVBASE_INPUT_ERROR);
        auto &inst = inst_umap_.at(inst_id);
        srlog_verify_inst(inst, ICVBASE_INSTANCE_ERROR);
        ret = inst->set_param(param, value);
        return ret;
    }
    int32_t get_param(const long inst_id, const char *param, char *value,
                      int32_t len) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        srlog_verify_ptr(param, ICVBASE_INPUT_ERROR);
        srlog_verify_ptr(value, ICVBASE_INPUT_ERROR);
        srlog_verify_para(len > 0, ICVBASE_INPUT_ERROR);
        srlog_error_return((0 != inst_umap_.count(inst_id)),
                           ("get_param( {} ) failed.", inst_id),
                           ICVBASE_INPUT_ERROR);
        auto &inst = inst_umap_.at(inst_id);
        srlog_verify_inst(inst, ICVBASE_INSTANCE_ERROR);
        ret = inst->get_param(param, value, len);
        return ret;
    }
    int32_t get_res_verion(const RESTYPE res_type, int32_t &res_ver_num) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "BASE_MGR");
        srlog_verify_init(inited_, ICVBASE_INIT_ERROR);
        ret = DnnWrapperInst()->get_res_version(res_type, res_ver_num);
        srlog_error_return(!ret,
                           ("get_res_version({},{}) failed",
                            static_cast<int>(res_type), res_ver_num),
                           ret);
        return ret;
    }

  protected:
    virtual int32_t res_inst_register() = 0;

  private:
    void reset() {
        inst_umap_.clear();
        res_info_umap_.clear();
        inst_id_ = INST_ID_INITIAL;
        cfg_path_ = "";
    }

  protected:
    bool inited_;
    std::string cfg_path_;
    long inst_id_;
    FactoryClass<iCVBaseDef> factory_;
    std::mutex mtx_;
    std::unordered_map<long, std::unique_ptr<iCVBaseDef>> inst_umap_;
    std::unordered_map<RESTYPE, iCVBaseResInfo> res_info_umap_;
};