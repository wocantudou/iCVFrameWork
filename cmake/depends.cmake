# opencv 3.4.4
include_directories(
    ${source_dir}/submodules/Opencv3.4.4/include
    ${source_dir}/submodules/Opencv3.4.4/modules/core/include
    ${source_dir}/submodules/Opencv3.4.4/modules/dnn/include
    ${source_dir}/submodules/Opencv3.4.4/modules/features2d/include
    ${source_dir}/submodules/Opencv3.4.4/modules/flann/include
    ${source_dir}/submodules/Opencv3.4.4/modules/highgui/include
    ${source_dir}/submodules/Opencv3.4.4/modules/imgproc/include
    ${source_dir}/submodules/Opencv3.4.4/modules/imgcodecs/include
    ${source_dir}/submodules/Opencv3.4.4/modules/videoio/include
    ${source_dir}/submodules/Opencv3.4.4/modules/calib3d/include
    ${source_dir}/submodules/Opencv3.4.4/modules/ml/include
    ${source_dir}/submodules/Opencv3.4.4/modules/objdetect/include
    ${source_dir}/submodules/Opencv3.4.4/modules/photo/include
    ${source_dir}/submodules/Opencv3.4.4/modules/shape/include
    ${source_dir}/submodules/Opencv3.4.4/modules/videostab/include
    ${source_dir}/submodules/Opencv3.4.4/modules/video/include
    ${source_dir}/submodules/Opencv3.4.4/modules/stitching/include
    ${source_dir}/submodules/Opencv3.4.4/modules/superres/include
    )

if(BUILD_OPENCV)
    include_directories(
                ${PROJECT_BINARY_DIR}
                )
else()
    include_directories(
                ${source_dir}/common/3rdparty
                )
endif()

# simpleini and spdlog
include_directories(
                ${source_dir}/submodules/iCVBaseUtilsLib/include/3rdparty/spdlog-1.11.0/include
                ${source_dir}/submodules/iCVBaseUtilsLib/include/iCVLogger
                ${source_dir}/submodules/iCVBaseUtilsLib/include/3rdparty/simpleini-4.19
                ${source_dir}/submodules/iCVBaseUtilsLib/include/iCVConfig
                )

# rapidjson
include_directories(
                ${source_dir}/submodules/iCVBaseUtilsLib/include/3rdparty/rapidjson-1.1.0/include
                ${source_dir}/submodules/iCVBaseUtilsLib/include/iCVJson
                )

# easy profiler
include_directories(
                ${source_dir}/submodules/iCVBaseUtilsLib/src/3rdparty/easy_profiler-2.1.0/easy_profiler_core/include
                ${source_dir}/submodules/iCVBaseUtilsLib/src/3rdparty/easy_profiler-2.1.0/reader
                ${source_dir}/submodules/iCVBaseUtilsLib/include/iCVProfiler
                )

# base utils
include_directories(${source_dir}/submodules/iCVBaseUtilsLib/include/utils)


# dnnwrapper
include_directories(
                ${source_dir}/submodules/DnnWrapper/source/include
                ${source_dir}/submodules/DnnWrapper/source/submodules/IPEDU/source/include
                )

include_directories(
                ${source_dir}/include
                )

function(ModuleDependLibs thirdparty_libs)
    message(STATUS "begin thirdparty_libs = ${thirdparty_libs}")
    
    # add thirdyparty library
    set(thirdparty_lib_dir "")
    set(thirdparty_lib "")

    set(BEGIN_WHOLE_ARCHIVE -Wl,--whole-archive)
    set(END_WHOLE_ARCHIVE -Wl,--no-whole-archive)

    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        if(BUILD_OPENCV)
            if (MSVC)
				list(APPEND thirdparty_lib "opencv_imgcodecs344;libpng;libtiff;libjpeg-turbo;libjasper;IlmImf;libwebp")
                list(APPEND thirdparty_lib "opencv_calib3d344;opencv_imgproc344;opencv_core344;opencv_highgui344;ittnotify;zlib")
            else()
				list(APPEND thirdparty_lib "opencv_imgcodecs")
                list(APPEND thirdparty_lib "opencv_calib3d;opencv_imgproc;opencv_core;opencv_highgui")
            endif()
        else()
            if (MSVC)
				list(APPEND thirdparty_lib "opencv_imgcodecs344d;libpngd;libtiffd;libjpeg-turbod;libjasperd;IlmImfd;libwebpd")
                list(APPEND thirdparty_lib "opencv_calib3d344d;opencv_imgproc344d;opencv_core344d;opencv_highgui344d;ittnotifyd;zlibd")
            else()
				list(APPEND thirdparty_lib "opencv_imgcodecsd")
                list(APPEND thirdparty_lib "opencv_calib3dd;opencv_imgprocd;opencv_cored;opencv_highguid")
            endif()
        endif()
    else()
        if (MSVC)
		    list(APPEND thirdparty_lib "opencv_imgcodecs344;libpng;libtiff;libjpeg-turbo;libjasper;IlmImf;libwebp")
            list(APPEND thirdparty_lib "opencv_calib3d344;opencv_imgproc344;opencv_core344;opencv_highgui344;ittnotify;zlib")
        else()
			list(APPEND thirdparty_lib "opencv_imgcodecs")
            list(APPEND thirdparty_lib "opencv_calib3d;opencv_imgproc;opencv_core;opencv_highgui")
        endif()
    endif()
    
	list(APPEND thirdparty_lib "ipedu_static")
	list(APPEND thirdparty_lib "ipup_static")
    list(APPEND thirdparty_lib "decrypt")

    list(APPEND thirdparty_lib "xlite")
    if (BUILD_AMBA)
        list(APPEND thirdparty_lib_dir ${XLITE_AMBA_THIRDPARTY_PATH}/amba_cv2x/amba_cv2x_lib/)
        list(APPEND thirdparty_lib "libnnctrl.so;libcavalry_mem.so")
    endif ()

    if (BUILD_QNX700)
        list(APPEND thirdparty_lib_dir ${XLITE_HEXAGON_THIRDPARTY_PATH}/hexagon_NN/qnx)
        list(APPEND thirdparty_lib "libstd;fastrpc_pmem;smmu_client;cdsprpc;hexagon_nn_stub")
    endif () 

    if(MSVC)
        # add thridyparty library
        list(APPEND thirdparty_lib_dir ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
        list(APPEND thirdparty_lib_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})

    else () 
        # add thridyparty library
        list(APPEND thirdparty_lib_dir ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
        list(APPEND thirdparty_lib_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        if (CMAKE_SYSTEM_NAME STREQUAL "Android")
            list(APPEND thirdparty_lib "log;z")
            if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND NOT BUILD_OPENCV)
                list(APPEND thirdparty_lib "tegra_hald;cpufeaturesd")
            else()
                list(APPEND thirdparty_lib "tegra_hal;cpufeatures")
            endif()
        else ()
            if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
                if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND NOT BUILD_OPENCV)
                    list(APPEND thirdparty_lib "tegra_hald;zlibd")
                else()
                    list(APPEND thirdparty_lib "tegra_hal;zlib")
                endif()
            else()
                list(APPEND thirdparty_lib "dl")
                if(BUILD_X86_64_LINUX)
                    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND NOT BUILD_OPENCV)
                        list(APPEND thirdparty_lib "zlibd")
                    else()
                        list(APPEND thirdparty_lib "zlib")
                    endif()
                endif()
                if(BUILD_AARCH64_LINUX)
                    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND NOT BUILD_OPENCV)
                        list(APPEND thirdparty_lib "tegra_hald;zlibd")
                    else()
                        list(APPEND thirdparty_lib "tegra_hal;zlib")
                    endif()
                endif()
                if(BUILD_ARM_LINUX)
                    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND NOT BUILD_OPENCV)
                        list(APPEND thirdparty_lib "tegra_hald;zlibd")
                    else()
                        list(APPEND thirdparty_lib "tegra_hal;zlib")
                    endif()
                endif()
                if(BUILD_IOS64)
                    list(APPEND thirdparty_lib "")
                endif()
                list(APPEND thirdparty_lib "z")
            endif()	
        endif ()
    endif ()
    link_directories(
    ${thirdparty_lib_dir}
    )

    set(${thirdparty_libs} ${thirdparty_lib} PARENT_SCOPE)
endfunction()

function(ModuleLinker module_name thirdparty_libs)
    message(STATUS "ModuleLinker module_name = ${module_name}")
    message(STATUS "ModuleLinker thirdparty_libs = ${thirdparty_libs}")
    #***************static library************
	aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/AffineTransform ${module_name}_STATIC_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/BaseImgProc ${module_name}_STATIC_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/BasePointsProc ${module_name}_STATIC_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/CropPadding ${module_name}_STATIC_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/ObjDetect ${module_name}_STATIC_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/ops ${module_name}_STATIC_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/ScopeExit ${module_name}_STATIC_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/ThreadPool ${module_name}_STATIC_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/Track/HungarianMatch  ${module_name}_STATIC_LIB)
    aux_source_directory(${source_dir}/common/default_utils/encrypt  ${module_name}_STATIC_LIB)
    aux_source_directory(.  ${module_name}_STATIC_LIB)
    aux_source_directory(${source_dir}/common/Xlite_wrapper ${module_name}_STATIC_LIB)

    #***************shared library************
    aux_source_directory(.  ${module_name}_SHARED_LIB)
    aux_source_directory(../ ${module_name}_SHARED_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/AffineTransform ${module_name}_SHARED_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/BaseImgProc ${module_name}_SHARED_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/BasePointsProc ${module_name}_SHARED_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/CropPadding ${module_name}_SHARED_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/ObjDetect ${module_name}_SHARED_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/ops ${module_name}_SHARED_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/ScopeExit ${module_name}_SHARED_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/DnnCVLib/ThreadPool ${module_name}_SHARED_LIB)
    aux_source_directory(${source_dir}/common/3rdparty/Track/HungarianMatch  ${module_name}_SHARED_LIB)
    aux_source_directory(${source_dir}/common/xlite_wrapper ${module_name}_SHARED_LIB)

    aux_source_directory(${source_dir}/common/default_utils/encrypt ${module_name}_SHARED_LIB)
    
    if(MSVC)
        set_property(GLOBAL PROPERTY USE_FOLDERS ON) #open directory
        add_library(${module_name}_static STATIC ${${module_name}_STATIC_LIB})
        set_property(TARGET ${module_name}_static PROPERTY FOLDER "${module_name}")
        #add_library(${module_name}_shared SHARED ${${module_name}_SHARED_LIB} ${WIFLYDSM_DEF_FILE})
        add_library(${module_name}_shared SHARED ${${module_name}_SHARED_LIB})
        target_link_libraries (${module_name}_shared  ${thirdparty_libs})
        set_property(TARGET ${module_name}_shared PROPERTY FOLDER "${module_name}")
    else()
        add_library(${module_name}_static STATIC ${${module_name}_STATIC_LIB})
        # add_library(${module_name}_shared SHARED ${${module_name}_SHARED_LIB})
        # target_link_libraries (${module_name}_shared ${thirdparty_libs})
    endif()
    message(STATUS "ModuleLinker ****** thirdparty_libs = ${thirdparty_libs}")
endfunction()

function(GetEngineNameWithVersion version_file_path config_in_file_path config_out_file_path)
    MESSAGE(STATUS "~~~~~~~~~~~~~~~~~~version_file_path = ${version_file_path}")
    MESSAGE(STATUS "~~~~~~~~~~~~~~~~~~config_in_file_path = ${config_in_file_path}")
    MESSAGE(STATUS "~~~~~~~~~~~~~~~~~~config_out_file_path = ${config_out_file_path}")
    FILE(STRINGS ${version_file_path} contents)
    foreach(line ${contents})
        string(FIND "${line}" "#define ENGINE_VERSION_MAJOR" test)
        if (NOT(${test} STREQUAL "-1"))
            string(REPLACE "#define ENGINE_VERSION_MAJOR " "" ENGINE_VERSION_MAJOR ${line})
            continue() 
        endif()

        string(FIND "${line}" "#define ENGINE_VERSION_MINOR" test)
        if (NOT(${test} STREQUAL "-1"))
            string(REPLACE "#define ENGINE_VERSION_MINOR " "" ENGINE_VERSION_MINOR ${line})
            continue() 
        endif()

        string(FIND "${line}" "#define ENGINE_VERSION_PATCH" test)
        if (NOT(${test} STREQUAL "-1"))
            string(REPLACE "#define ENGINE_VERSION_PATCH " "" ENGINE_VERSION_PATCH ${line})
            continue() 
        endif()

        string(FIND "${line}" "#define ENGINE_NAME" test)
        if (NOT(${test} STREQUAL "-1"))
            string(REPLACE "#define ENGINE_NAME " "" ENGINE_NAME ${line})
            continue() 
        endif()
    endforeach()

    configure_file(
            ${config_in_file_path}
            ${config_out_file_path}
    )

    set(ENGINE_NAME ${ENGINE_NAME} CACHE STRING "" FORCE)
    set(ENGINE_VERSION_MAJOR ${ENGINE_VERSION_MAJOR} CACHE STRING "" FORCE)
    set(ENGINE_VERSION_MINOR ${ENGINE_VERSION_MINOR} CACHE STRING "" FORCE)
    set(ENGINE_VERSION_PATCH ${ENGINE_VERSION_PATCH} CACHE STRING "" FORCE)

    MESSAGE(STATUS "~~~~~~~~~~~~~~~~~~ENGINE_NAME = ${ENGINE_NAME}")
    MESSAGE(STATUS "~~~~~~~~~~~~~~~~~~ENGINE_VERSION_MAJOR = ${ENGINE_VERSION_MAJOR}")
    MESSAGE(STATUS "~~~~~~~~~~~~~~~~~~ENGINE_VERSION_MINOR = ${ENGINE_VERSION_MINOR}")
    MESSAGE(STATUS "~~~~~~~~~~~~~~~~~~ENGINE_VERSION_PATCH = ${ENGINE_VERSION_PATCH}")

endfunction()

function(GetAEEInfo aee_info_file_path aee_info_in_file_path aee_info_out_file_path)
    FILE(STRINGS ${aee_info_file_path} contents)
    foreach(line ${contents})
        string(REPLACE "	" ";" STR_LIST ${line})
        string(REPLACE " " ";" STR_LIST ${line})

        set(INFO_LIST "")

        foreach(info_str ${STR_LIST})
            list(APPEND INFO_LIST ${info_str})
        endforeach()

        # message(STATUS "#####INFO_LIST = ${INFO_LIST}")

        list (GET INFO_LIST 1 info_name)
        list (GET INFO_LIST 2 info_value)
        set(${info_name} ${info_value})
    endforeach()

    configure_file(
            ${aee_info_in_file_path}
            ${aee_info_out_file_path}
    )
endfunction()