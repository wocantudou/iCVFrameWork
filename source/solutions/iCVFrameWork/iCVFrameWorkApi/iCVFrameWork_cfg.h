#pragma once
#define CFG_FILE_NAME "./test.cfg"
#define SECTION_NAME iCVFrameWork
#define CFG_NAME CFG_iCVFrameWork
// #define G_CFG_ICVFRAMEWORK SingleonClass<CFG_iCVFrameWork>::get_inst()
#include "iCVConfigTemplate.h"
#define G_CFG_ICVFRAMEWORK CUST_SINGLETON_INST(CFG_iCVFrameWork)
#undef CFG_NAME
#undef SECTION_NAME
#undef CFG_FILE_NAME