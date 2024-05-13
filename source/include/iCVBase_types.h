#pragma once

#if defined(_MSC_VER) /* Microsoft Visual C++ */
#if defined(ICVBASEAPI)
#undef ICVBASEAPI
#undef ICVBASEAPITYPE
#endif
#define ICVBASEAPI __stdcall
#define ICVBASEAPITYPE __stdcall
#pragma pack(push, 8)
#else /* Any other including Unix */
#if defined(ICVBASEAPI)
#undef ICVBASEAPI
#undef ICVBASEAPITYPE
#endif
#define ICVBASEAPI __attribute__((visibility("default")))
#define ICVBASEAPITYPE
#endif

typedef int int32_t;
typedef void *ICV_BASE_INST;

#define ENUM_TO_STRING(value) #value

#define ENGINE_API(RetType, FunName, ParamList)                                \
    RetType ICVBASEAPI FunName ParamList;                                      \
    typedef RetType(ICVBASEAPITYPE *Proc_##FunName) ParamList;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BOOL
#define BOOL int32_t
#define TRUE 1
#define FALSE 0
#endif

// Enumerations In order for the conversion string to be traversed, the
// definition of the enumeration must be contiguous.
enum RESTYPE {
    RESTYPE_NONE,
    RESTYPE_FACE_DETECT_CPU,
    RESTYPE_FACE_DETECT_NPU,
    RESTYPE_FACE_ALIGNMENT_CPU,
    RESTYPE_FACE_ALIGNMENT_NPU,
    MODTYPE_FACE_SINGLETRACK_CPU,
    MODTYPE_FACE_SINGLETRACK_NPU,
    RESTYPE_COUNT,
};

struct RES_SET {
    RESTYPE res_type;
    void *reserved; // reserved parameters
};

enum ICV_BASE_IMG_FMT {
    ICV_BASE_IMG_FMT_NONE,
    ICV_BASE_IMG_FMT_GRAY = 1,
    ICV_BASE_IMG_FMT_RGB = 2,
    ICV_BASE_IMG_FMT_BGR = 3,
    ICV_BASE_IMG_FMT_BGRA = 4,
    ICV_BASE_IMG_FMT_YUV420_NV12 = 5,
    ICV_BASE_IMG_FMT_YUV420_NV21 = 6,
    ICV_BASE_IMG_FMT_COUNT,
};

typedef struct _ICVBaseImgDataIn {
    char *data;
    int32_t w;
    int32_t h;
    int32_t data_byte_size;
    ICV_BASE_IMG_FMT fmt;
    _ICVBaseImgDataIn *next;
} ICVBaseImgDataIn;

typedef struct _ICVBaseAudDataIn {
    char *data;
    int32_t data_byte_size;
    int32_t sample_rate;
    int32_t sample_bits;
    int32_t channel_num;
    _ICVBaseAudDataIn *next;
} ICVBaseAudDataIn;

typedef struct _ICVBaseTxtDataIn {
    char *data;
    int32_t data_byte_size;
    _ICVBaseTxtDataIn *next;
} ICVBaseTxtDataIn;

enum ICV_BASE_DATA_STAT {
    ICV_BASE_DATA_STAT_NONE,
    ICV_BASE_DATA_STAT_IMG = 0b001,
    ICV_BASE_DATA_STAT_AUD = 0b010,
    ICV_BASE_DATA_STAT_TXT = 0b100,
    ICV_BASE_DATA_STAT_IMG_AUD = 0b011,
    ICV_BASE_DATA_STAT_IMG_TXT = 0b101,
    ICV_BASE_DATA_STAT_IMG_AUD_TXT = 0b111,
    ICV_BASE_DATA_STAT_COUNT,
};

typedef struct _ICVBaseDataIn {
    ICV_BASE_DATA_STAT data_stat;
    ICVBaseImgDataIn img;
    ICVBaseAudDataIn aud;
    ICVBaseTxtDataIn txt;
} ICVBaseDataIn;

typedef struct _ICVBaseNumericFloat {
    BOOL valid;
    float val;
} ICVBaseNumericFloat;

typedef struct _ICVBaseRect2f {
    float x;
    float y;
    float w;
    float h;
} ICVBaseRect2f;

typedef struct _ICVBaseRect2fWithInfo {
    ICVBaseRect2f rect;
    float conf_score;
    int32_t id;
} ICVBaseRect2fWithInfo;

typedef struct _ICVBasePoint2f {
    float x;
    float y;
} ICVBasePoint2f;

typedef struct _ICVBasePoint2fWithInfo {
    ICVBasePoint2f point;
    ICVBaseNumericFloat visiable_info;
    float conf_score;
} ICVBasePoint2fWithInfo;

typedef struct _ICVBasePoint3f {
    float x;
    float y;
    float z;
} ICVBasePoint3f;

typedef struct _ICVBaseAngle2d {
    float pitch;
    float yaw;
} ICVBaseAngle2d;

typedef struct _ICVBaseAngle3d {
    float pitch;
    float yaw;
    float roll;
} ICVBaseAngle3d;

#ifdef __cplusplus
};
#endif
