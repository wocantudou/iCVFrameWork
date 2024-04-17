#pragma once
#include "errcode_module.h"
#include "iCVFrameWork_types.h"

#if defined(_MSC_VER) /* Microsoft Visual C++ */
#if defined(iCVFrameWorkAPI)
#undef iCVFrameWorkAPI
#undef iCVFrameWorkAPITYPE
#endif
#define iCVFrameWorkAPI __stdcall
#define iCVFrameWorkAPITYPE __stdcall
#pragma pack(push, 8)
#else /* Any other including Unix */
#if defined(iCVFrameWorkAPI)
#undef iCVFrameWorkAPI
#undef iCVFrameWorkAPITYPE
#endif
#define iCVFrameWorkAPI __attribute__((visibility("default")))
#define iCVFrameWorkAPITYPE
#endif

typedef void *IFLYDSM_INST;

#define API_LIST_DSM(func)                                                     \
    func(iCVFrameWorkInitialize) func(iCVFrameWorkUninitialize)                \
        func(iCVFrameWorkCreateInst) func(iCVFrameWorkDestroyInst)             \
            func(iCVFrameWorkMalloc) func(iCVFrameWorkFree) func(              \
                iCVFrameWorkSetParameter) func(iCVFrameWorkGetParameter)       \
                func(iCVFrameWorkGetResult) func(iCVFrameWorkResourceAdd)      \
                    func(iCVFrameWorkResourceAddFromMem) func(                 \
                        iCVFrameWorkResourceDelete)                            \
                        func(iCVFrameWorkGetResVersion) func(                  \
                            iCVFrameWorkGetExpiredTime)                        \
                            func(iCVFrameWorkGetEngineVerion) func(            \
                                iCVFrameWorkGetLensOcclusionDetectionResult)   \
                                func(iCVFrameWorkResetInst)

#ifdef __cplusplus
extern "C" {
#endif

// common DSM Engine API
int32_t iCVFrameWorkAPI iCVFrameWorkInitialize(const char *init_params);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkInitialize)(
    const char *init_params);

int32_t iCVFrameWorkAPI iCVFrameWorkUninitialize();
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkUninitialize)();

int32_t iCVFrameWorkAPI iCVFrameWorkCreateInst(IFLYDSM_INST *ifly_dsm_inst,
                                               const RESTYPE res_type);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkCreateInst)(
    IFLYDSM_INST *ifly_dsm_inst, const RESTYPE res_type);

int32_t iCVFrameWorkAPI iCVFrameWorkDestroyInst(IFLYDSM_INST *ifly_dsm_inst);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkDestroyInst)(
    IFLYDSM_INST *ifly_dsm_inst);

int32_t iCVFrameWorkAPI iCVFrameWorkResetInst(IFLYDSM_INST ifly_dsm_inst);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkResetInst)(
    IFLYDSM_INST ifly_dsm_inst);

int32_t iCVFrameWorkAPI iCVFrameWorkMalloc(void **output_data);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkMalloc)(
    void **output_data);

// just for encrypt version
int32_t iCVFrameWorkAPI iCVFrameWorkFree(void **output_data);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkFree)(void **output_data);

int32_t iCVFrameWorkAPI iCVFrameWorkSetParameter(IFLYDSM_INST ifly_dsm_inst,
                                                 const char *param,
                                                 const char *value);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkSetParameter)(
    IFLYDSM_INST ifly_dsm_inst, const char *param, const char *value);

int32_t iCVFrameWorkAPI iCVFrameWorkGetParameter(IFLYDSM_INST ifly_dsm_inst,
                                                 const char *param, char *value,
                                                 int32_t nLen);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkGetParameter)(
    IFLYDSM_INST ifly_dsm_inst, const char *param, char *value, int32_t nLen);

int32_t iCVFrameWorkAPI iCVFrameWorkGetResult(IFLYDSM_INST ifly_dsm_inst,
                                              const void *in_data,
                                              void *out_data);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkGetResult)(
    IFLYDSM_INST wFDInst, const void *in_data, void *out_data);

int32_t iCVFrameWorkAPI iCVFrameWorkResourceAdd(const RES_SET *res_set,
                                                const char *szResURI);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkResourceAdd)(
    const RES_SET *res_set, const char *szResURI);

int32_t iCVFrameWorkAPI iCVFrameWorkResourceAddFromMem(const RES_SET *res_set,
                                                       const void *data,
                                                       const int32_t len);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkResourceAddFromMem)(
    const RES_SET *res_set, const void *data, const int32_t len);

int32_t iCVFrameWorkAPI iCVFrameWorkResourceDelete(const RES_SET *res_set);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkResourceDelete)(
    const RES_SET *res_set);

int32_t iCVFrameWorkAPI iCVFrameWorkGetResVersion(const RES_SET *res_set,
                                                  int *version);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkGetResVersion)(
    const RES_SET *res_set, int *version);

int32_t iCVFrameWorkAPI iCVFrameWorkGetExpiredTime(const char **expired_time);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkGetExpiredTime)(
    const char **expired_time);

int32_t iCVFrameWorkAPI iCVFrameWorkGetEngineVerion(char *engine_version,
                                                    int32_t len);
typedef int32_t(iCVFrameWorkAPITYPE *Proc_iCVFrameWorkGetEngineVerion)(
    char *engine_version, int32_t len);

int32_t iCVFrameWorkAPI iCVFrameWorkGetLensOcclusionDetectionResult(
    const void *in_data, float *occlusion_ratio);
typedef int32_t(
    iCVFrameWorkAPITYPE *Proc_iCVFrameWorkGetLensOcclusionDetectionResult)(
    const void *in_data, float *occlusion_ratio);
#ifdef __cplusplus
};
#endif

/* Reset the structure packing alignments for different compilers. */
#if defined(_MSC_VER) /* Microsoft Visual C++ */
#pragma pack(pop)
#endif