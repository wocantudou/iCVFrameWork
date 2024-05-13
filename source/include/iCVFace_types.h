#pragma once
#include "iCVBase_types.h"
#define MAX_DETECT_OBJECT_NUM (20)
typedef struct _ICVFace {
    ICVBaseRect2fWithInfo face_rect_with_info;
    // TO ADD LATER
} ICVFace;

typedef struct _ICVFaces {
    ICVFace face[MAX_DETECT_OBJECT_NUM];
    int32_t face_num;
} ICVFaces;