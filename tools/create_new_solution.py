import os
import sys

def replace_keywords_in_file(file_path, old_keyword, new_keyword):
    """
    Replaces all occurrences of old_keyword with new_keyword in the given file.
    
    :param file_path: The path to the file.
    :param old_keyword: The keyword to replace.
    :param new_keyword: The keyword to replace with.
    """
    with open(file_path, 'r', encoding='utf-8') as file:
        content = file.read()
    
    content = content.replace(old_keyword, new_keyword)
    
    with open(file_path, 'w', encoding='utf-8') as file:
        file.write(content)

def replace_keywords_in_directory(source_directory, old_keyword, new_keyword, target_directory):
    """
    Recursively replaces all occurrences of old_keyword with new_keyword 
    in both the file names, directory names, and the contents of all files in the source directory.
    The modified files and directories are saved to the target directory.
    
    :param source_directory: The root directory to start the search.
    :param old_keyword: The keyword to replace.
    :param new_keyword: The keyword to replace with.
    :param target_directory: The directory to save the modified files and directories.
    """
    if not os.path.exists(target_directory):
        os.makedirs(target_directory)

    for root, dirs, files in os.walk(source_directory):
        for dir_name in dirs:
            new_dir_name = dir_name.replace(old_keyword, new_keyword)
            source_dir_path = os.path.join(root, dir_name)
            relative_path = os.path.relpath(source_dir_path, source_directory)
            target_dir_path = os.path.join(target_directory, relative_path.replace(old_keyword, new_keyword))
            
            if not os.path.exists(target_dir_path):
                os.makedirs(target_dir_path)

        for filename in files:
            file_path = os.path.join(root, filename)
            
            # Read and replace keyword in file content
            with open(file_path, 'r', encoding='utf-8') as file:
                content = file.read()
            content = content.replace(old_keyword, new_keyword)
            
            # Determine new file path
            relative_path = os.path.relpath(root, source_directory)
            new_filename = filename.replace(old_keyword, new_keyword)
            new_file_dir = os.path.join(target_directory, relative_path.replace(old_keyword, new_keyword))
            new_file_path = os.path.join(new_file_dir, new_filename)
            
            if not os.path.exists(new_file_dir):
                os.makedirs(new_file_dir)
            
            # Write modified content to new file path
            with open(new_file_path, 'w', encoding='utf-8') as file:
                file.write(content)

def main(source_directory, old_keyword, new_keyword, target_directory):
    """
    The main function to execute the keyword replacement in the source directory
    and save the results to the target directory.
    
    :param source_directory: The root directory to start the search.
    :param old_keyword: The keyword to replace.
    :param new_keyword: The keyword to replace with.
    :param target_directory: The directory to save the modified files and directories.
    """
    replace_keywords_in_directory(source_directory, old_keyword, new_keyword, target_directory)

if __name__ == '__main__':
    if len(sys.argv) != 5:
        print("Usage: python script.py <source_directory> <old_keyword> <new_keyword> <target_directory>")
    else:
        source_directory = sys.argv[1]
        old_keyword = sys.argv[2]
        new_keyword = sys.argv[3]
        target_directory = sys.argv[4]
        main(source_directory, old_keyword, new_keyword, target_directory)

# If you want to create a new solution name "Chauvin", please follow the steps below:
# Step1
# python3 create_new_solution.py ../source/solutions/iCVFrameWork/ FrameWork Chauvin ../source/solutions/iCVChauvin/ 
# Step2
# Add add_subdirectory(${source_dir}/solutions/iCVChauvin/iCVChauvinApi) && add_subdirectory(${source_dir}/solutions/iCVChauvin/iCVChauvinTest) in root CMakeLists.txt.
# Step3
# Modify the line list(APPEND thirdparty_lib "iCVFaceDetect_static") at ../source/solutions/iCVChauvin/iCVChauvinApi/CMakeLists.txt to fit your needs.
# Step4
# Rewrite the function res_inst_register in ../source/solutions/iCVChauvin/iCVChauvinApi/iCVChauvin_mgr.hpp to fit your needs.
# Step5
# Add the header #include "iCVChauvin_types.h" to the Modules/restype/xxx you need.
# Step6
# Add ${PROJECT_SOURCE_DIR}/source/solutions/iCVChauvin/iCVChauvinApi to the CMakeLists in Modules/restype/xxx you need.
