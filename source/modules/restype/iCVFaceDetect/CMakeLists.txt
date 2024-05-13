#generate library
include(${PROJECT_SOURCE_DIR}/cmake/depends.cmake)

include_directories(
		./
		)

message(STATUS "iflyFaceDetect hhhhh = ${source_dir}")

set(thirdparty_libs "")
ModuleDependLibs(thirdparty_libs)
message(STATUS "iflyFaceDetect thirdparty_libs = ${thirdparty_libs}")
set(thirdparty_lib "")
# list(APPEND thirdparty_lib "wFaceReid_static")
list(APPEND thirdparty_lib "${thirdparty_libs}")
ModuleLinker("iflyFaceDetect" "${thirdparty_libs}")