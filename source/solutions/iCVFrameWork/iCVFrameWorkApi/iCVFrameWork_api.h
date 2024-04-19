#pragma once
#include "iCVBase_errcode.h"
#include "iCVFrameWork_types.h"

#if defined(_MSC_VER) /* Microsoft Visual C++ */
#pragma pack(push, 8)
#endif

typedef ICV_BASE_INST iCVFW_INST;

#define API_LIST_FW(func)                                                      \
    func(iCVFrameWorkInitialize) func(iCVFrameWorkUninitialize)                \
        func(iCVFrameWorkCreateInst) func(iCVFrameWorkDestroyInst)             \
            func(iCVFrameWorkSetParameter) func(iCVFrameWorkGetParameter)      \
                func(iCVFrameWorkGetResult) func(iCVFrameWorkResourceAdd)      \
                    func(iCVFrameWorkResourceAddFromMem)                       \
                        func(iCVFrameWorkResourceDelete)                       \
                            func(iCVFrameWorkGetResVersion)                    \
                                func(iCVFrameWorkGetExpiredTime)               \
                                    func(iCVFrameWorkGetEngineVerion)          \
                                        func(iCVFrameWorkResetInst)

#ifdef __cplusplus
extern "C" {
#endif

// Engine API
ENGINE_API(int32_t, iCVFrameWorkInitialize,
           (const char *cfg_path, void *reserved));
ENGINE_API(int32_t, iCVFrameWorkUninitialize, ());
ENGINE_API(int32_t, iCVFrameWorkCreateInst,
           (iCVFW_INST * inst, const RESTYPE res_type));
ENGINE_API(int32_t, iCVFrameWorkDestroyInst, (iCVFW_INST * inst));
ENGINE_API(int32_t, iCVFrameWorkResetInst, (iCVFW_INST inst));
ENGINE_API(int32_t, iCVFrameWorkSetParameter,
           (iCVFW_INST inst, const char *param, const char *value));
ENGINE_API(int32_t, iCVFrameWorkGetParameter,
           (iCVFW_INST inst, const char *param, char *value, int32_t len));
ENGINE_API(int32_t, iCVFrameWorkGetResult,
           (iCVFW_INST inst, const void *in_data, void *out_data));
ENGINE_API(int32_t, iCVFrameWorkResourceAdd,
           (const RES_SET *res_set, const char *res_path));
ENGINE_API(int32_t, iCVFrameWorkResourceAddFromMem,
           (const RES_SET *res_set, const void *data, const int32_t len));
ENGINE_API(int32_t, iCVFrameWorkResourceDelete, (const RES_SET *res_set));
ENGINE_API(int32_t, iCVFrameWorkGetResVersion, (const char **expired_time));
ENGINE_API(int32_t, iCVFrameWorkGetEngineVerion,
           (char *engine_version, int32_t len));

#ifdef __cplusplus
};
#endif

/* Reset the structure packing alignments for different compilers. */
#if defined(_MSC_VER) /* Microsoft Visual C++ */
#pragma pack(pop)
#endif