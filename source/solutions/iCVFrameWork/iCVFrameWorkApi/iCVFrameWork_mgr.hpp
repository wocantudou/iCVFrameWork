#pragma once
#include "iCVBase_inst_mgr.hpp"
#include "iCVFaceDetect_def.h"

class iCVFrameWorkInstMgr : public iCVBaseInstMgr {
  protected:
    int32_t res_inst_register() {
        int32_t ret = ICVBASE_NO_ERROR;
        res_info_map_.clear();
        iCVBaseResInfo res_info;
        // Unit FaceDetect{
        res_info.res_path =
            G_CFG_ICVFRAMEWORK()->get_iCVFrameWork_FaceDetectResPath();
        res_info.res_name = NAME_STR(RESTYPE_FACE_DETECT_CPU);
        res_info_map_.insert(std::make_pair(RESTYPE_FACE_DETECT_CPU, res_info));
        factory_.Register(RESTYPE_FACE_DETECT_CPU, creatorFaceDetectInst);
        // }
        // New Unit Add ... TODO {}
        return ret;
    }
};

#define G_iCVFrameWorkInstMgr SingletonClass<iCVFrameWorkInstMgr>::get_inst