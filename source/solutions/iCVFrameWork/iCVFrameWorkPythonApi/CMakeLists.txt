include_directories(
                    ./
                    ${PROJECT_SOURCE_DIR}/source/include
                    ${PROJECT_SOURCE_DIR}/source/solutions/iCVFrameWork/iCVFrameWorkApi
                    )
#generate library
aux_source_directory(. iCVFrameWork_PYTHON_API_SHARED)

# add thridyparty library
set(thirdparty_lib_dir "")
set(thirdparty_lib "")
if(MSVC)
	list(APPEND thirdparty_lib_dir "../../thirdparty/opencv/x64/vc15/lib")
	list(APPEND thirdparty_lib_dir ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
	list(APPEND thirdparty_lib_dir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
	list(APPEND thirdparty_lib ${ENGINE_NAME_WITH_VERSION})
else ()
	# add thridyparty library
	list(APPEND thirdparty_lib_dir ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
	list(APPEND thirdparty_lib_dir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
	if (CMAKE_SYSTEM_NAME STREQUAL "Android")
		list(APPEND thirdparty_lib ${ENGINE_NAME_WITH_VERSION})
	else()
		list(APPEND thirdparty_lib ${ENGINE_NAME})
	endif()
endif ()
# list(APPEND thirdparty_lib iCVFrameWorkRT)
link_directories(
    ${thirdparty_lib_dir}
)
message(STATUS "++++++++++++++++++++++++++++thirdparty_lib = ${thirdparty_lib}")
set(PYTHON_API_NAME iCVFrameWorkPY)
if(MSVC)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	pybind11_add_module(${PYTHON_API_NAME} iCVFrameWork_pybind.cpp)
	target_link_libraries(${PYTHON_API_NAME} PRIVATE ${thirdparty_lib})
	set_property(TARGET ${PYTHON_API_NAME} PROPERTY FOLDER "iCVFrameWorkPythonAPI")
else()
	pybind11_add_module(${PYTHON_API_NAME} iCVFrameWork_pybind.cpp)
	target_link_libraries(${PYTHON_API_NAME} PRIVATE ${thirdparty_lib})
	set_property(TARGET ${PYTHON_API_NAME} PROPERTY CXX_STANDARD 11)
endif()



