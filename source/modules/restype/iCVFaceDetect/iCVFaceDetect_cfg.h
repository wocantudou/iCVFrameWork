#pragma once
#define CFG_FILE_NAME "./test.cfg"
#define SECTION_NAME iCVFaceDetect
#define CFG_NAME CFG_iCVFaceDetect
#include "iCVConfigTemplate.h"
#define G_CFG_ICVFACEDETECT CUST_SINGLETON_INST(CFG_iCVFaceDetect)
#undef CFG_NAME
#undef SECTION_NAME
#undef CFG_FILE_NAME