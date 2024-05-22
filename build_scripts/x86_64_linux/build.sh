#-----------------------------------------------------------------
# 172.20.31.106
#-----------------------------------------------------------------
set -e
# Debug/Release
build_type=Release
build_base_utils_lib_test=OFF
build_with_easy_profiler=OFF
use_ffmpeg=ON
if [ ${use_ffmpeg} == "ON" ];then
	# 172.20.191.53
	# MY_FFMPEG_PATH=/data/wensha2/tool/ffmpeg4.2.9/install/x86_64-intellinux64-ffmpeg4.2.9
	# WSL2
	MY_FFMPEG_PATH=/home/wensha2/workspace/opt/ffmpeg4.2.9/install/x86_64-intellinux64-ffmpeg4.2.9
	export PKG_CONFIG_LIBDIR=${MY_FFMPEG_PATH}/lib/pkgconfig
	export PKG_CONFIG_PATH=${MY_FFMPEG_PATH}/lib/pkgconfig
	# export CMAKE_INCLUDE_PATH=${MY_FFMPEG_PATH}/include:${CMAKE_INCLUDE_PATH}
	# export CMAKE_LIBRARY_PATH=${MY_FFMPEG_PATH}/lib:${CMAKE_LIBRARY_PATH}
	echo "################################################## WITH FFMPEG ON #################"
else
	echo "################################################## WITH FFMPEG OFF ################"
fi

cd ../../
build_dir="cmake_build/build_x86_64_linux/"
if [ ! -d ${build_dir} ];then
	mkdir -p ${build_dir}
fi

project_root_path=$(pwd)
echo "${project_root_path}"

cd ${build_dir}

rm -rf *

cmake	-DBUILD_BASE_UTILS_LIB_TEST=${build_base_utils_lib_test}							\
		-DBUILD_WITH_EASY_PROFILER=${build_with_easy_profiler}								\
		-DCMAKE_GENERATOR="Unix Makefiles"													\
		-DCMAKE_TOOLCHAIN_FILE=${project_root_path}/cmake/x86_64_linux.toolchain.cmake		\
		-DCMAKE_BUILD_TYPE=${build_type}													\
		-DUSE_FFMPEG=${use_ffmpeg}															\
		${project_root_path}
make -j2

if [ "${build_type}" == "Debug" ];then
	exit
else
	echo "################################################## Make Build Target #################"
fi