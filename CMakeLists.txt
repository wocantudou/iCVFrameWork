cmake_minimum_required(VERSION 3.6)

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

option(GIT_SUBMODULE "Check submodules within CMake." ON)
option(BUILD_WITH_EASY_PROFILER "Build with easy profiler for engine." OFF)
option(ENGINE_DATA_DUMP "Dump engine input/output data for engine." OFF)

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

include(${PROJECT_SOURCE_DIR}/cmake/platform.cmake)

add_subdirectory(${source_dir}/solutions/iCVFrameWork/iCVFrameWorkApi)

if(BUILD_WITH_EASY_PROFILER)
    add_definitions(-DBUILD_WITH_EASY_PROFILER)
    message(STATUS "add_definitions(-DBUILD_WITH_EASY_PROFILER)")
    add_subdirectory(${source_dir}/submodules/iCVBaseUtilsLib)
endif()
