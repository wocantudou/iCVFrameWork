#include "iCVBase_inst_mgr.hpp"
#include "iCVFrameWork_api.h"
#include "iCVFrameWork_cfg.h"
#include "iCVFrameWork_config.h"

int32_t ICVBASEAPI iCVFrameWorkInitialize(const char *cfg_path,
                                          void *reserved) {
    int32_t ret = ICVBASE_NO_ERROR;
    G_CFG_ICVFRAMEWORK->init(cfg_path);
    srlog_open(cfg_path,
               G_CFG_ICVFRAMEWORK->get_iCVFrameWork_LogPath().c_str());
    srlog_perf(LOG_PROF_TAG, "API");
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkUninitialize() {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkCreateInst(iCVFW_INST *inst,
                                          const RESTYPE res_type) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkDestroyInst(iCVFW_INST *inst) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkResetInst(iCVFW_INST inst) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkSetParameter(iCVFW_INST inst, const char *param,
                                            const char *value) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkGetParameter(iCVFW_INST inst, const char *param,
                                            char *value, int32_t len) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkGetResult(iCVFW_INST inst, const void *in_data,
                                         void *out_data) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkResourceAdd(const RES_SET *res_set,
                                           const char *res_path) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkResourceAddFromMem(const RES_SET *res_set,
                                                  const void *data,
                                                  const int32_t len) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkResourceDelete(const RES_SET *res_set) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkGetResVersion(const RES_SET *res_set,
                                             int *version) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkGetExpiredTime(const char **expired_time) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkGetEngineVerion(char *engine_version,
                                               int32_t len) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    return ret;
}