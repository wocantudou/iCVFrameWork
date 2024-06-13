#ifndef __ICV_FACE_DETECT_DEF_H__
#define __ICV_FACE_DETECT_DEF_H__

#include "iCVFaceDetect_cfg.h"
#include "iCVFaceDetect_engine.h"

class iCVFaceDetectInst : public iCVBaseDef {
  public:
    iCVFaceDetectInst();
    ~iCVFaceDetectInst();
    int32_t init(const char *workdir);
    int32_t fini();
    int32_t set_param(const char *para, const char *value);
    int32_t get_param(const char *para, char *value, int32_t len);
    int32_t process(const void *in, void *out);
    int32_t get_result(void *out);

  private:
    FaceDetectWithEngine *engine_;
};

std::unique_ptr<iCVBaseDef> creatorFaceDetectInst();

#endif