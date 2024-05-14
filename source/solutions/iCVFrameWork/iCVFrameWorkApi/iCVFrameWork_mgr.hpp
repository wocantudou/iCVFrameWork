#pragma once
#include "iCVFrameWork_mgr_unit.h"
class iCVFrameWorkInstMgr : public iCVBaseInstMgr {
  protected:
    int32_t register_res_info() {
        int32_t ret = ICVBASE_NO_ERROR;
        return ret;
    }
    int32_t register_res_creator() {
        int32_t ret = ICVBASE_NO_ERROR;
        factory_.Register(RESTYPE_FACE_DETECT_CPU, creatorFaceDetectInst);
        return ret;
    }
};

#define G_iCVFrameWorkInstMgr SingletonClass<iCVFrameWorkInstMgr>::get_inst