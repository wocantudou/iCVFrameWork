#pragma once
#include "dp_factory.hpp"
#include "dp_singleton.hpp"
#include "iCVBase_errcode.h"
#include <string>
#include <unordered_map>

#define INST_INITIAL (1)

class iCVBaseInstMgr {
  public:
    explicit iCVBaseInstMgr()
        : inited_(false), inst_id_(INST_INITIAL), cfg_path_("") {}

    virtual ~iCVBaseInstMgr() {
        reset();
        inited_ = false;
    }

  public:
    int init() {
        int32_t ret = ICVBASE_NO_ERROR;
        inited_ = true;
        return ret;
    }
    int fini() {
        int32_t ret = ICVBASE_NO_ERROR;
        return ret;
    }

    int setup() {
        int32_t ret = ICVBASE_NO_ERROR;
        return ret;
    }
    int shutdown() {
        int32_t ret = ICVBASE_NO_ERROR;
        return ret;
    }

    int create_inst() {
        int32_t ret = ICVBASE_NO_ERROR;
        return ret;
    }
    int destroy_inst() {
        int32_t ret = ICVBASE_NO_ERROR;
        return ret;
    }
    int reset_inst() {
        int32_t ret = ICVBASE_NO_ERROR;
        return ret;
    }
    int run_inst() {
        int32_t ret = ICVBASE_NO_ERROR;
        return ret;
    }
    int set_param() {
        int32_t ret = ICVBASE_NO_ERROR;
        return ret;
    }
    int get_param() {
        int32_t ret = ICVBASE_NO_ERROR;
        return ret;
    }

  private:
    void reset() {
        inst_umap_.clear();
        inst_id_ = INST_INITIAL;
        cfg_path_ = "";
    }

  private:
    bool inited_;
    std::string cfg_path_;
    long inst_id_;
    std::unordered_map<long, std::unique_ptr<iCVBaseInstMgr>> inst_umap_;
    FactoryClass<iCVBaseInstMgr> factory_;
};