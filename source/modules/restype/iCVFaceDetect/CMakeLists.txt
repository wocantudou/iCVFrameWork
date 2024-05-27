#generate library
include(${PROJECT_SOURCE_DIR}/cmake/depends.cmake)

include_directories(
		./
		${PROJECT_SOURCE_DIR}/source/solutions/iCVFrameWork/iCVFrameWorkApi
		)

message(STATUS "iCVFaceDetect hhhhh = ${source_dir}")

set(thirdparty_libs "")
ModuleDependLibs(thirdparty_libs)
message(STATUS "iCVFaceDetect thirdparty_libs = ${thirdparty_libs}")
set(thirdparty_lib "")
# list(APPEND thirdparty_lib "wFaceReid_static")
list(APPEND thirdparty_lib "${thirdparty_libs}")
ModuleLinker("iCVFaceDetect" "${thirdparty_libs}")