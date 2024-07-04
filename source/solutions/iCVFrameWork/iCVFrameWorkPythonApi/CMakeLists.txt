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

link_directories(
    ${thirdparty_lib_dir}
)

set(PYTHON_API_NAME iCVFrameWorkPY)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# add_library(${PYTHON_API_NAME} MODULE ${iCVFrameWork_PYTHON_API_SHARED})
pybind11_add_module(${PYTHON_API_NAME} ${iCVFrameWork_PYTHON_API_SHARED})
target_link_libraries(${PYTHON_API_NAME} PRIVATE pybind11::module ${thirdparty_lib})
set_target_properties(${PYTHON_API_NAME} PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
                                                    SUFFIX "${PYTHON_MODULE_SEXTENSION}")
set_property(TARGET ${PYTHON_API_NAME} PROPERTY FOLDER "iCVFrameWorkPythonAPI")


