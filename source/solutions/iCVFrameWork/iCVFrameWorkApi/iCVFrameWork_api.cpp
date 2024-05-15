#include "iCVFrameWork_api.h"
#include "iCVFrameWork_cfg.h"
#include "iCVFrameWork_config.h"
#include "iCVFrameWork_mgr.hpp"

static bool g_iCVFrameWork_init = false;
static bool g_iCVFrameWork_permission = false;
static std::string g_iCVFrameWork_engine_version = "";

#ifdef SMOKING_VERSION
#define VERSION_TAG "Test"
#else
#define VERSION_TAG ""
#endif

int32_t ICVBASEAPI iCVFrameWorkInitialize(const char *cfg_path,
                                          void *reserved) {
    int32_t ret = ICVBASE_NO_ERROR;
    G_CFG_ICVFRAMEWORK()->init(cfg_path);
    srlog_open(cfg_path,
               G_CFG_ICVFRAMEWORK()->get_iCVFrameWork_LogPath().c_str());
    srlog_perf(LOG_PROF_TAG, "API");
    srlog_verify_init(!g_iCVFrameWork_init, ICVBASE_INIT_ERROR);
    std::string engine_name = ENGINE_NAME;
    std::string engine_version_majar = ENGINE_VERSION_MAJOR;
    std::string engine_version_minor = ENGINE_VERSION_MINOR;
    std::string engine_version_patch = ENGINE_VERSION_PATCH;
    std::string engine_version =
        engine_name + VERSION_TAG + engine_version_majar + "." +
        engine_version_minor + "." + engine_version_patch;
    std::cout << "Engine Version : " << engine_version << std::endl;
    srlog_info("Engine Version : {}", engine_version);
    g_iCVFrameWork_engine_version = engine_version;
    g_iCVFrameWork_permission = true; // TODO
    ret = G_iCVFrameWorkInstMgr()->init(cfg_path);
    srlog_error_return(!ret, ("G_iCVFrameWorkInstMgr()->init( {} )", cfg_path),
                       ret);
    g_iCVFrameWork_init = true;
    std::cout << "Initialize successful ..." << std::endl;
    srlog_info("Initialize successful ...");
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
                                             int *version_num) {
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