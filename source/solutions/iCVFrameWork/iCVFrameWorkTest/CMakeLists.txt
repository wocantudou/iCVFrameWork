#generate library

# opencv 4.9.0
include_directories(
    ${source_dir}/submodules/iCVIPLib/source/submodules/Opencv4.9.0/include
    ${source_dir}/submodules/iCVIPLib/source/submodules/Opencv4.9.0/modules/core/include
    ${source_dir}/submodules/iCVIPLib/source/submodules/Opencv4.9.0/modules/features2d/include
    ${source_dir}/submodules/iCVIPLib/source/submodules/Opencv4.9.0/modules/flann/include
    ${source_dir}/submodules/iCVIPLib/source/submodules/Opencv4.9.0/modules/highgui/include
    ${source_dir}/submodules/iCVIPLib/source/submodules/Opencv4.9.0/modules/imgproc/include
    ${source_dir}/submodules/iCVIPLib/source/submodules/Opencv4.9.0/modules/imgcodecs/include
    ${source_dir}/submodules/iCVIPLib/source/submodules/Opencv4.9.0/modules/videoio/include
    ${source_dir}/submodules/iCVIPLib/source/submodules/Opencv4.9.0/modules/calib3d/include
    )

include_directories(
./
${PROJECT_SOURCE_DIR}/source/include
${PROJECT_SOURCE_DIR}/source/solutions/iCVFrameWork/iCVFrameWorkApi
${PROJECT_SOURCE_DIR}/source/submodules/iCVBaseUtilsLib/include/utils
)

#*************executable target*********
aux_source_directory(.  iCVFrameWorkTest_execute)
# add thridyparty library
set(thirdparty_lib_dir "")
set(thirdparty_lib "")
# list(APPEND thirdparty_lib "iflyFaceRetrieval_shared")
if(MSVC)
	list(APPEND thirdparty_lib_dir "../../thirdparty/opencv/x64/vc15/lib")
	list(APPEND thirdparty_lib_dir ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
	list(APPEND thirdparty_lib_dir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
	list(APPEND thirdparty_lib ${ENGINE_NAME_WITH_VERSION})
	if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND NOT BUILD_WITH_OPENCV)
		list(APPEND thirdparty_lib "opencv_flann409d;opencv_videoio409d;opencv_imgcodecs409d;libpngd;libtiffd;libjpeg-turbod;libwebpd;ippiwd;zlibd;libopenjp2d")
	else () 
		list(APPEND thirdparty_lib "opencv_flann409;opencv_videoio409;opencv_imgcodecs409;libpng;libtiff;libjpeg-turbo;libwebp;ippiw;zlib;libopenjp2")
	endif()
else ()
	if(USE_FFMPEG)
		list(APPEND thirdparty_lib_dir $ENV{PKG_CONFIG_PATH}/..)
		if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,noexecstack,-z,relro,-z,now,-z,defs")
		endif ()
	endif()
	# add thridyparty library
	list(APPEND thirdparty_lib_dir ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
	list(APPEND thirdparty_lib_dir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
	if (CMAKE_SYSTEM_NAME STREQUAL "Android")
		list(APPEND thirdparty_lib ${ENGINE_NAME_WITH_VERSION})
	else()
		list(APPEND thirdparty_lib ${ENGINE_NAME})
	endif()
	
	if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND NOT BUILD_WITH_OPENCV)
		list(APPEND thirdparty_lib "opencv_videoiod;opencv_imgcodecsd;ittnotifyd;libpngd;libtiffd;libjpeg-turbod;libwebpd;zlibd;libopenjp2d")
	else()
		list(APPEND thirdparty_lib "opencv_videoio;opencv_imgcodecs;ittnotify;libpng;libtiff;libjpeg-turbo;libwebp;zlib;libopenjp2")
	endif()
endif ()

link_directories(
    ${thirdparty_lib_dir}
    #${user_lib_dir}
)

#link_libraries(
#    ${thirdparty_lib}
#)

set(EXE_TARGET_NAME iCVFrameWorkTest)
if(MSVC)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON) #open directory
	#*************executable target*********
	add_executable(${EXE_TARGET_NAME}  ${iCVFrameWorkTest_execute})
	target_link_libraries (${EXE_TARGET_NAME} ${thirdparty_lib})
	set_property(TARGET ${EXE_TARGET_NAME} PROPERTY FOLDER "iCVFrameWorkTest")
	#new add 
	SET_TARGET_PROPERTIES(${EXE_TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}")
else()
	#*************executable target*********
	add_executable(${EXE_TARGET_NAME}  ${iCVFrameWorkTest_execute})
	target_link_libraries (${EXE_TARGET_NAME} ${thirdparty_lib})
	if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
		add_custom_command(TARGET ${EXE_TARGET_NAME} POST_BUILD
						   WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
						   COMMAND ${CMAKE_STRIP} "-s" "${EXE_TARGET_NAME}")
	endif()
endif()


