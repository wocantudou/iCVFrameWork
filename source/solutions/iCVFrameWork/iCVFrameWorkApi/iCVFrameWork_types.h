#pragma once
#include "iCVBase_types.h"
#define MAX_DETECT_OBJECT_NUM (20)
typedef struct _ICVFrameWorkFace {
    ICVBaseRect2fWithInfo face_rect_with_info;
    // TO ADD LATER
} ICVFrameWorkFace;

typedef struct _ICVFrameWorkFaces {
    ICVFrameWorkFace faces[MAX_DETECT_OBJECT_NUM];
    int32_t face_num;
    int32_t face_idx;
} ICVFrameWorkFaces;