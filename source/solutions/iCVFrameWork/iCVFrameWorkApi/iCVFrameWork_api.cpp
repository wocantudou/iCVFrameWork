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
    srlog_error_return(
        !ret, ("G_iCVFrameWorkInstMgr()->init( {} ) error!", cfg_path), ret);
    g_iCVFrameWork_init = true;
    g_iCVFrameWork_init = true;
    std::cout << "Initialize successful ..." << std::endl;
    srlog_info("Initialize successful ...");
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkUninitialize() {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    srlog_verify_init(g_iCVFrameWork_init, ICVBASE_INIT_ERROR);
    srlog_verify_para(g_iCVFrameWork_permission, ICVBASE_PERMISSION_ERROR);
    ret = G_iCVFrameWorkInstMgr()->fini();
    srlog_error_return(!ret, ("G_iCVFrameWorkInstMgr()->fini() error!"), ret);
    std::cout << "Uninitialize successful ..." << std::endl;
    srlog_info("Uninitialize successful ...");
    srlog_close();
    g_iCVFrameWork_init = false;
    g_iCVFrameWork_permission = false;
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkCreateInst(iCVFrameWork_INST *inst,
                                          const RESTYPE res_type) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    srlog_verify_init(g_iCVFrameWork_init, ICVBASE_INIT_ERROR);
    srlog_verify_para(g_iCVFrameWork_permission, ICVBASE_PERMISSION_ERROR);
    srlog_verify_ptr(inst, ICVBASE_MEMORY_ERROR);
    srlog_verify_para((res_type > RESTYPE_NONE) && (res_type < RESTYPE_COUNT),
                      ICVBASE_INPUT_ERROR);
    long inst_id = -1;
    DNN_WRAPPER::DnnScheduleConfig scfg;
    scfg.hardware_type_ = static_cast<DNN_WRAPPER::DNNHardWareType>(
        G_CFG_ICVFRAMEWORK()->get_iCVFrameWork_AIEngineHardWareType());
    scfg.num_thread_ =
        G_CFG_ICVFRAMEWORK()->get_iCVFrameWork_AIEngineNumThread();
    scfg.max_batch_size_ =
        G_CFG_ICVFRAMEWORK()->get_iCVFrameWork_AIEngineMaxBatchSize();
    scfg.device_id = G_CFG_ICVFRAMEWORK()->get_iCVFrameWork_AIEngineDeviceID();

    ret = G_iCVFrameWorkInstMgr()->create_inst(res_type, scfg, inst_id);
    srlog_error_return(!ret,
                       ("G_iCVFrameWorkInstMgr()->create_inst( {}, {} ) error!",
                        static_cast<int>(res_type), inst_id),
                       ret);
    *inst = reinterpret_cast<iCVFrameWork_INST>(inst_id);
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkDestroyInst(iCVFrameWork_INST *inst) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    srlog_verify_init(g_iCVFrameWork_init, ICVBASE_INIT_ERROR);
    srlog_verify_para(g_iCVFrameWork_permission, ICVBASE_PERMISSION_ERROR);
    srlog_verify_ptr(inst, ICVBASE_MEMORY_ERROR);
    long inst_id = *(reinterpret_cast<long *>(inst));
    ret = G_iCVFrameWorkInstMgr()->destroy_inst(inst_id);
    srlog_error_return(
        !ret, ("G_iCVFrameWorkInstMgr()->destroy_inst( {} ) error!", inst_id),
        ret);
    *inst = NULL;
    inst = NULL;
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkResetInst(iCVFrameWork_INST inst) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    srlog_verify_init(g_iCVFrameWork_init, ICVBASE_INIT_ERROR);
    srlog_verify_para(g_iCVFrameWork_permission, ICVBASE_PERMISSION_ERROR);
    srlog_verify_ptr(inst, ICVBASE_MEMORY_ERROR);
    long inst_id = reinterpret_cast<long>(inst);
    ret = G_iCVFrameWorkInstMgr()->reset_inst(inst_id);
    srlog_error_return(
        !ret, ("G_iCVFrameWorkInstMgr()->reset_inst( {} ) error!", inst_id),
        ret);
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkSetParameter(iCVFrameWork_INST inst,
                                            const char *param,
                                            const char *value) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    srlog_verify_init(g_iCVFrameWork_init, ICVBASE_INIT_ERROR);
    srlog_verify_para(g_iCVFrameWork_permission, ICVBASE_PERMISSION_ERROR);
    srlog_verify_ptr(inst, ICVBASE_MEMORY_ERROR);
    srlog_verify_ptr(param, ICVBASE_MEMORY_ERROR);
    srlog_verify_ptr(value, ICVBASE_MEMORY_ERROR);
    long inst_id = reinterpret_cast<long>(inst);
    ret = G_iCVFrameWorkInstMgr()->set_param(inst_id, param, value);
    srlog_error_return(!ret,
                       ("G_iCVFrameWorkInstMgr()->set_param( {}, {}, {} ) "
                        "error!",
                        inst_id, param, value),
                       ret);
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkGetParameter(iCVFrameWork_INST inst,
                                            const char *param, char *value,
                                            int32_t len) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    srlog_verify_init(g_iCVFrameWork_init, ICVBASE_INIT_ERROR);
    srlog_verify_para(g_iCVFrameWork_permission, ICVBASE_PERMISSION_ERROR);
    srlog_verify_ptr(inst, ICVBASE_MEMORY_ERROR);
    srlog_verify_ptr(param, ICVBASE_MEMORY_ERROR);
    srlog_verify_ptr(value, ICVBASE_MEMORY_ERROR);
    srlog_verify_para(len > 0, ICVBASE_INPUT_ERROR);
    long inst_id = reinterpret_cast<long>(inst);
    ret = G_iCVFrameWorkInstMgr()->get_param(inst_id, param, value, len);
    srlog_error_return(!ret,
                       ("G_iCVFrameWorkInstMgr()->get_param( {}, {}, {}, {} ) "
                        "error!",
                        inst_id, param, value, len),
                       ret);
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkGetResult(iCVFrameWork_INST inst,
                                         const void *in_data, void *out_data) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    srlog_verify_init(g_iCVFrameWork_init, ICVBASE_INIT_ERROR);
    srlog_verify_para(g_iCVFrameWork_permission, ICVBASE_PERMISSION_ERROR);
    srlog_verify_ptr(inst, ICVBASE_MEMORY_ERROR);
    srlog_verify_ptr(in_data, ICVBASE_MEMORY_ERROR);
    srlog_verify_ptr(out_data, ICVBASE_MEMORY_ERROR);
    long inst_id = reinterpret_cast<long>(inst);
    ret = G_iCVFrameWorkInstMgr()->run_inst(inst_id, in_data, out_data);
    srlog_error_return(
        !ret, ("G_iCVFrameWorkInstMgr()->run_inst( {} ) error!", inst_id), ret);
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkResourceAdd(const RESTYPE res_type,
                                           const char *res_path) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    srlog_verify_init(g_iCVFrameWork_init, ICVBASE_INIT_ERROR);
    srlog_verify_para(g_iCVFrameWork_permission, ICVBASE_PERMISSION_ERROR);
    srlog_verify_para((res_type > RESTYPE_NONE) && (res_type < RESTYPE_COUNT),
                      ICVBASE_INPUT_ERROR);
    ret = G_iCVFrameWorkInstMgr()->setup(res_type, res_path);
    srlog_error_return(!ret,
                       ("G_iCVFrameWorkInstMgr()->setup( {}, {} ) error!",
                        static_cast<int>(res_type), res_path),
                       ret);

    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkResourceAddFromMem(const RESTYPE res_type,
                                                  const void *data,
                                                  const int32_t len) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    srlog_verify_init(g_iCVFrameWork_init, ICVBASE_INIT_ERROR);
    srlog_verify_para(g_iCVFrameWork_permission, ICVBASE_PERMISSION_ERROR);
    srlog_verify_para((res_type > RESTYPE_NONE) && (res_type < RESTYPE_COUNT),
                      ICVBASE_INPUT_ERROR);
    ret = G_iCVFrameWorkInstMgr()->setup(res_type, data, len);
    srlog_error_return(!ret,
                       ("G_iCVFrameWorkInstMgr()->setup( {}, {}, {} ) error!",
                        static_cast<int>(res_type), data, len),
                       ret);
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkResourceDelete(const RESTYPE res_type) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    srlog_verify_init(g_iCVFrameWork_init, ICVBASE_INIT_ERROR);
    srlog_verify_para(g_iCVFrameWork_permission, ICVBASE_PERMISSION_ERROR);
    srlog_verify_para((res_type > RESTYPE_NONE) && (res_type < RESTYPE_COUNT),
                      ICVBASE_INPUT_ERROR);
    ret = G_iCVFrameWorkInstMgr()->shutdown(res_type);
    srlog_error_return(!ret,
                       ("G_iCVFrameWorkInstMgr()->shutdown( {} ) error!",
                        static_cast<int>(res_type)),
                       ret);
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkGetResVersion(const RESTYPE res_type,
                                             int32_t *version_num) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    srlog_verify_init(g_iCVFrameWork_init, ICVBASE_INIT_ERROR);
    srlog_verify_para(g_iCVFrameWork_permission, ICVBASE_PERMISSION_ERROR);
    srlog_verify_para((res_type > RESTYPE_NONE) && (res_type < RESTYPE_COUNT),
                      ICVBASE_INPUT_ERROR);
    srlog_verify_ptr(version_num, ICVBASE_MEMORY_ERROR);
    ret = G_iCVFrameWorkInstMgr()->get_res_verion(res_type, *version_num);
    srlog_error_return(
        !ret,
        ("G_iCVFrameWorkInstMgr()->get_res_verion( {}, {} ) error!",
         static_cast<int>(res_type), *version_num),
        ret);

    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkGetEngineVersion(const char **engine_version) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    // srlog_verify_init(g_iCVFrameWork_init, ICVBASE_INIT_ERROR);
    // srlog_verify_para(g_iCVFrameWork_permission, ICVBASE_PERMISSION_ERROR);
    srlog_verify_ptr(engine_version, ICVBASE_MEMORY_ERROR);
    *engine_version = g_iCVFrameWork_engine_version.c_str();
    return ret;
}

int32_t ICVBASEAPI iCVFrameWorkGetExpiredTime(const char **expired_time) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_perf(LOG_PROF_TAG, "API");
    srlog_verify_init(g_iCVFrameWork_init, ICVBASE_INIT_ERROR);
    srlog_verify_para(g_iCVFrameWork_permission, ICVBASE_PERMISSION_ERROR);
    std::cerr << "Supported Soon." << std::endl;
    srlog_info("Supported Soon.");
    return ret;
}