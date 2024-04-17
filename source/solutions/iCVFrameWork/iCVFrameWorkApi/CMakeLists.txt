

set(ENGINE_NAME iCVFrameWork)
set(ENGINE_VERSION_MAJOR 1.0)
set(ENGINE_VERSION_MINOR 1000)
set(ENGINE_VERSION_PATCH 0)

include(${PROJECT_SOURCE_DIR}/cmake/depends.cmake)
GetEngineNameWithVersion("${PROJECT_SOURCE_DIR}/source/solutions/iCVFrameWork/version.txt" 
                         "${PROJECT_SOURCE_DIR}/source/solutions/iCVFrameWork/iCVFrameWorkApi/iCVFrameWork_config.h.in" 
                         "${PROJECT_BINARY_DIR}/iCVFrameWork_config.h")