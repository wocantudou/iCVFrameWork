# cmake_minimum_required(VERSION 3.5)
cmake_minimum_required(VERSION 3.4...3.18)
if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
set(CMAKE_CONFIGURATION_TYPES
    ${CMAKE_BUILD_TYPE}
    CACHE STRING "" FORCE)

project(iCVFrameWork C CXX)

set(source_dir ${PROJECT_SOURCE_DIR}/source)

if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

if(NOT DEFINED CMAKE_RELEASE_POSTFIX)
    set(CMAKE_RELEASE_POSTFIX "")
endif()

option(BUILD_API_ICVFRAMEWORK "Build iCVFrameWork API." ON)
option(BUILD_PYTHON_API_ICVFRAMEWORK "Build iCVFrameWork python API." ON)
option(GIT_SUBMODULE "Check submodules within CMake." ON)
option(BUILD_WITH_EASY_PROFILER "Build with easy profiler for engine." OFF)
option(BUILD_WITH_OPENCV "Build with OpenCV." ON)
option(USE_FFMPEG "Build OpenCV with FFMpeg." OFF)
option(ENGINE_DATA_DUMP "Dump engine input/output data for engine." OFF)
option(USE_AI_ENGINE_MNN "Use mnn for AI engine." ON)
option(SMOKING_VERSION "Smoking Verson for Engine." ON)

find_package(Git QUIET)
if(GIT_FOUND AND NOT EXISTS "${source_dir}/submodules/iCVBaseUtilsLib/.git")
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update ...")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(
                FATAL_ERROR
                    "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules"
            )
        endif()
    endif()
endif()

if(NOT EXISTS "${source_dir}/submodules/iCVBaseUtilsLib/CMakeLists.txt")
    message(
        FATAL_ERROR
            "The submodules were not update! GIT_SUBMODULE was turned off or failed. Please update submodules and try again."
    )
endif()

if(ENGINE_DATA_DUMP)
    add_definitions(-DENGINE_DATA_DUMP)
    message(STATUS "add_definitions(-DENGINE_DATA_DUMP)")
endif()

if(SMOKING_VERSION)
    add_definitions(-DSMOKING_VERSION)
    message(STATUS "add_definitions(-DSMOKING_VERSION)")
endif()

include(${PROJECT_SOURCE_DIR}/cmake/platform.cmake)
# *********************** FOR DEPENDS Begin ***********************
# AI Engine
if(USE_AI_ENGINE_MNN)
    add_definitions(-DUSE_AI_ENGINE_MNN)
    message(STATUS "add_definitions(-DUSE_AI_ENGINE_MNN)")
endif()
# DnnWrapper
add_subdirectory(${source_dir}/submodules/DnnWrapper)

if(BUILD_WITH_EASY_PROFILER)
    add_definitions(-DBUILD_WITH_EASY_PROFILER)
    message(STATUS "add_definitions(-DBUILD_WITH_EASY_PROFILER)")
    add_subdirectory(${source_dir}/submodules/iCVBaseUtilsLib)
endif()

# iCVIPLib
add_subdirectory(${source_dir}/submodules/iCVIPLib)


# *********************** FOR DEPENDS End ***********************
# API
if(BUILD_API_ICVFRAMEWORK)
    add_subdirectory(${source_dir}/solutions/iCVFrameWork/iCVFrameWorkApi)    
    if(BUILD_PYTHON_API_ICVFRAMEWORK)
        add_subdirectory(${source_dir}/submodules/pybind11-2.13.1)
        add_subdirectory(${source_dir}/solutions/iCVFrameWork/iCVFrameWorkPythonApi)
    endif()
    # ResType and ModType
    add_subdirectory(${source_dir}/modules/restype/iCVFaceDetect)
    add_subdirectory(${source_dir}/solutions/iCVFrameWork/iCVFrameWorkTest)
endif()
