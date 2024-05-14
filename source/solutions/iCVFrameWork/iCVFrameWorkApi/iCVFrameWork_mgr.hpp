#pragma once
#include "iCVFrameWork_mgr_unit.h"
class iCVFrameWorkInstMgr : public iCVBaseInstMgr {
  protected:
    int32_t register_res_info() {
        int32_t ret = ICVBASE_NO_ERROR;
        res_info_umap_.clear();
        iCVBaseResInfo res_info;
        res_info.res_path =
            G_CFG_ICVFRAMEWORK()->get_iCVFrameWork_FaceDetectResPath();
        res_info_umap_.insert(
            std::make_pair(RESTYPE_FACE_DETECT_CPU, res_info));
        return ret;
    }
    int32_t register_res_creator() {
        int32_t ret = ICVBASE_NO_ERROR;
        factory_.Register(RESTYPE_FACE_DETECT_CPU, creatorFaceDetectInst);
        return ret;
    }
};

#define G_iCVFrameWorkInstMgr SingletonClass<iCVFrameWorkInstMgr>::get_inst