# macro: generate source files
macro (generate_sources obj_name abs_source_dir sub_dirs_flag)
    # set lib directory
    set (${obj_name}_dir "${src_dir}/${abs_source_dir}")
    set (srcs_regex 
        "${${obj_name}_dir}/*.c"
        "${${obj_name}_dir}/*.h"
        "${${obj_name}_dir}/*.hh"
        "${${obj_name}_dir}/*.cpp"
        "${${obj_name}_dir}/*.hpp"
        "${${obj_name}_dir}/*.rc"
        "${${obj_name}_dir}/*.cc"
        )
    if (${sub_dirs_flag} STREQUAL ON)
		# 查找(包括子文件夹)符合${srcs_regex}的文件存在变量${obj_name}_srcs中
        file(GLOB_RECURSE ${obj_name}_srcs  ${srcs_regex})
    else ()
		# 不包括子文件夹
        file(GLOB ${obj_name}_srcs ${srcs_regex})
    endif ()
endmacro ()

## Visual Studio Options
# keep directory structure, source_file should add "" as lists.
macro(source_group_by_dir abs_cur_dir source_files)
    if (MSVC)
        # set (${source_files})
        foreach (sgbd_file  ${source_files})
            string(REGEX REPLACE ${abs_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
            source_group(${sgbd_group_name} FILES ${sgbd_file})
        endforeach (sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)

# macro: Visual Studio operation
macro (vs_lib_operation obj_name type)
    if (MSVC)
        if (${type} STREQUAL SHARED)
            set (${obj_name}_srcs ${${obj_name}_srcs} "${${obj_name}_dir}/${obj_name}.def")
        endif ()
    endif ()
    # keep directory structure just for Visual Studio
    source_group_by_dir(${${obj_name}_dir} "${${obj_name}_srcs}")
endmacro ()

# macro: generate lib
macro (generate_lib lib_name abs_source_dir type sub_dirs_flag add_files remove_files)
    # generate source files
    generate_sources (${lib_name} ${abs_source_dir} ${sub_dirs_flag})
    # add files
    list (LENGTH add_files files_num)
    if (${files_num} GREATER 0)
        list (APPEND ${lib_name}_srcs ${add_files})
    endif ()
    # remove files
    list (LENGTH remove_files files_num)
    if (${files_num} GREATER 0)
        list (REMOVE_ITEM ${lib_name}_srcs ${remove_files})
    endif ()
    # Visual Studio operation
    if (MSVC)
        vs_lib_operation(${lib_name} ${type})
    endif()
    # generate library
    add_library (${lib_name} ${type} ${${lib_name}_srcs})
endmacro ()

# macro: generate lib with dependents
macro (generate_lib_dependents lib_name abs_source_dir type dependents sub_dirs_flag add_files remove_files)
    # generate lib
    generate_lib(${lib_name} ${abs_source_dir} ${type} ${sub_dirs_flag} "${add_files}" "${remove_files}")  # must use ""
    # link snapphoenix library
    target_link_libraries (${lib_name} ${dependents})
endmacro ()


# macro: generate executable
macro (generate_executable executable_name abs_source_dir sub_dirs_flag add_files remove_files)
    # generate config
    generate_sources (${executable_name} ${abs_source_dir} ${sub_dirs_flag})
    # add files
    list (LENGTH add_files files_num)
    if (${files_num} GREATER 0)
        list (APPEND ${executable_name}_srcs ${add_files})
    endif ()
    # remove files
    list (LENGTH remove_files files_num)
    if (${files_num} GREATER 0)
        list (REMOVE_ITEM ${executable_name}_srcs ${remove_files})
    endif ()
    # keep directory structure just for Visual Studio
    source_group_by_dir(${${executable_name}_dir} "${${executable_name}_srcs}")
    # generate library
    add_executable (${executable_name} ${${executable_name}_srcs})
endmacro ()


# macro: then generate executable with dependents
macro (generate_executable_dependents executable_name abs_source_dir dependents sub_dirs_flag add_files remove_files)
    # generate executable
    generate_executable(${executable_name} ${abs_source_dir} ${sub_dirs_flag} "${add_files}" "${remove_files}")  # must use ""
    # link snapphoenix library
    target_link_libraries (${executable_name} ${dependents})
endmacro ()


# macro: set_folder
macro (set_folder target folder)
    if (MSVC)
        # 在vs中以文件夹的形式显示，将${target}放在${folder}文件夹中
        set_target_properties(${target} PROPERTIES FOLDER ${folder}) 
    endif ()
endmacro ()
