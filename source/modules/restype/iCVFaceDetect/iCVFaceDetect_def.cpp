#include <math.h>

#include "iCVFaceDetect_cfg.h"
#include "iCVFaceDetect_def.h"
std::unique_ptr<iCVBaseDef> creatorFaceDetectInst() {
    return make_unique_cust<iCVFaceDetectInst>();
}
iCVFaceDetectInst::iCVFaceDetectInst() { face_detector_with_engine_ = NULL; }

iCVFaceDetectInst::~iCVFaceDetectInst() {}

int32_t iCVFaceDetectInst::init(const char *workdir) {
    int ret = ICVBASE_NO_ERROR;
    face_detector_with_engine_ =
        new (std::nothrow) MmyoloFaceDetectWithEngine(workdir);
    srlog_error_return((face_detector_with_engine_), ("mem alloc error!"),
                       ICVBASE_MEMORY_ERROR);
    return ret;
}

int32_t iCVFaceDetectInst::fini() {
    int ret = ICVBASE_NO_ERROR;
    if (NULL != face_detector_with_engine_) {
        delete face_detector_with_engine_;
        face_detector_with_engine_ = NULL;
    }
    return ret;
}

int32_t iCVFaceDetectInst::set_param(const char *para, const char *value) {
    int ret = face_detector_with_engine_->set_param(para, value);
    return ret;
}

int32_t iCVFaceDetectInst::get_param(const char *para, char *value,
                                     int32_t len) {
    int ret = face_detector_with_engine_->get_param(para, value, len);
    return ret;
}

int32_t iCVFaceDetectInst::process(const void *in, void *out) {
    int ret = ICVBASE_NO_ERROR;
    ret = set_input(in, input_mat_);
    srlog_error_return(
        !ret, ("iCVFaceDetectInst::process function | set_input error !"), ret);
    ret = get_result(out);
    srlog_error_return(
        !ret, ("iCVFaceDetectInst::process function | get_result error !"),
        ret);

    return ret;
}

int32_t iCVFaceDetectInst::get_result(void *out) {
    int ret = ICVBASE_NO_ERROR;
    srlog_verify_para(out, ICVBASE_INPUT_ERROR);
    ICVFrameWorkFaces &face_targets_in = *(ICVFrameWorkFaces *)(out);
    face_targets_in.face_num = 0;
    ret = face_detector_with_engine_->detect_face(res_type_, input_mat_,
                                                  face_targets_in);
    srlog_error_return(
        !ret, ("iCVFaceDetectInst::process function | detect_face error !"),
        ret);
    return ret;
}
