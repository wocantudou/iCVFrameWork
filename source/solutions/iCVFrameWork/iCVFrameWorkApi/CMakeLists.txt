

include_directories(
                    ${PROJECT_BINARY_DIR}
                    ./)
set(ENGINE_NAME "" CACHE INTERNAL "")
set(ENGINE_VERSION_MAJOR "" CACHE INTERNAL "")
set(ENGINE_VERSION_MINOR "" CACHE INTERNAL "")
set(ENGINE_VERSION_PATCH "" CACHE INTERNAL "")

include(${PROJECT_SOURCE_DIR}/cmake/depends.cmake)
GetEngineNameWithVersion("${PROJECT_SOURCE_DIR}/source/solutions/iCVFrameWork/version.txt" 
                         "${PROJECT_SOURCE_DIR}/source/solutions/iCVFrameWork/iCVFrameWorkApi/iCVFrameWork_config.h.in" 
                         "${PROJECT_BINARY_DIR}/iCVFrameWork_config.h")

# set(ENGINE_NAME ${ENGINE_NAME}_shared)

string(CONCAT ENGINE_NAME_WITH_VERSION ${ENGINE_NAME} "_" ${ENGINE_VERSION_MAJOR} "." ${ENGINE_VERSION_MINOR} "." ${ENGINE_VERSION_PATCH})
message(STATUS "###############ENGINE_NAME_WITH_VERSION = ${ENGINE_NAME_WITH_VERSION}" )


# for static
aux_source_directory(. iCVFrameWork_RT_API_STATIC)
aux_source_directory(${source_dir}/ai_engine_agent iCVFrameWork_RT_API_STATIC)
# for shared
aux_source_directory(. iCVFrameWork_RT_API_SHARED)
aux_source_directory(${source_dir}/ai_engine_agent iCVFrameWork_RT_API_SHARED)

unset(tmp_thirdparty_lib)
set(tmp_thirdparty_lib "")
ModuleDependLibs(tmp_thirdparty_lib)

# unset(thirdparty_lib)
set(thirdparty_lib "")
# linker lib from modules
list(APPEND thirdparty_lib "iCVFaceDetect_static")

list(APPEND thirdparty_lib "${tmp_thirdparty_lib}")

# unset(thirdparty_lib_dir)
set(thirdparty_lib_dir "")
list(APPEND thirdparty_lib_dir ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
list(APPEND thirdparty_lib_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
link_directories(${thirdparty_lib_dir})

if(MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(iCVFrameWork_DEF_FILE "./iCVFrameWork_debug.def")
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        set(iCVFrameWork_DEF_FILE "./iCVFrameWork_release.def")
    endif()
    # # for static
    # add_library(${ENGINE_NAME_WITH_VERSION} STATIC ${iCVFrameWork_RT_API_STATIC} ${iCVFrameWork_DEF_FILE})
    # for shared
    add_library(${ENGINE_NAME_WITH_VERSION} SHARED ${iCVFrameWork_RT_API_SHARED} ${iCVFrameWork_DEF_FILE})
    target_link_libraries(${ENGINE_NAME_WITH_VERSION} ${thirdparty_lib})

    set_property(TARGET ${ENGINE_NAME_WITH_VERSION} PROPERTY FOLDER "iCVFrameWorkApi")
else()
    # # for static
    # add_library(${ENGINE_NAME_WITH_VERSION} STATIC ${iCVFrameWork_RT_API_STATIC})
    # set_target_properties(${ENGINE_NAME_WITH_VERSION} PROPERTIES LINK_FLAGS "-Wl,--no-undefined,--retain-symbols-file,${CMAKE_CURRENT_SOURCE_DIR}/iCVFrameWork_static.sym")
    # for shared
    if (CMAKE_SYSTEM_NAME STREQUAL "Android")
        add_library(${ENGINE_NAME_WITH_VERSION} SHARED ${iCVFrameWork_RT_API_SHARED})
        target_link_libraries(${ENGINE_NAME_WITH_VERSION} ${thirdparty_lib})
    else()
        set(ENGINE_NAME_WITH_VERSION ${ENGINE_NAME} CACHE STRING "" FORCE)
        add_library(${ENGINE_NAME_WITH_VERSION} SHARED ${iCVFrameWork_RT_API_SHARED})
        target_link_libraries(${ENGINE_NAME_WITH_VERSION} ${thirdparty_lib})
        set_target_properties(${ENGINE_NAME_WITH_VERSION} PROPERTIES VERSION ${ENGINE_VERSION_MAJOR}.${ENGINE_VERSION_MINOR}.${ENGINE_VERSION_PATCH} SOVERSION ${ENGINE_VERSION_MAJOR})
    endif()
    if (NOT APPLE AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        add_custom_command(TARGET ${ENGINE_NAME_WITH_VERSION} POST_BUILD
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            COMMAND ${CMAKE_STRIP} -s "lib${ENGINE_NAME_WITH_VERSION}.so"
        )
        set_target_properties(${ENGINE_NAME_WITH_VERSION} PROPERTIES LINK_FLAGS "-Wl,--no-undefined,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/iCVFrameWork_shared.map")
    endif()
endif()


