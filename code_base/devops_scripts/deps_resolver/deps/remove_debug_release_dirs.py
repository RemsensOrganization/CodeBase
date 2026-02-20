import os
import shutil

# CONSTANTS
RELEASE_DIR_NAME = "release"
DEBUG_DIR_NAME = "debug"
LEVELS_UP_TO_BUILD_DIRS = "../"

print('Cleaning debug release directories')

build_dirs_list = []
for dir in os.listdir(LEVELS_UP_TO_BUILD_DIRS):
    if not RELEASE_DIR_NAME  in dir.lower() and not DEBUG_DIR_NAME in dir.lower():
        print('IGNORE THIS DIR (no release and debug phrase in its name) -->', dir)
        continue
    if os.path.isfile(dir):
        continue
    print("BUILD folder: " + dir)
    build_dirs_list.append(dir)

for file in build_dirs_list:
    path = LEVELS_UP_TO_BUILD_DIRS + file + "/"
    files_and_folders_list = os.listdir(path)
    print(files_and_folders_list)
    for file_or_folder_for_deleting in files_and_folders_list:
            print(path+file_or_folder_for_deleting)
            if os.path.isfile(path+file_or_folder_for_deleting):
                    os.remove(path+file_or_folder_for_deleting)
            if os.path.isdir(path+file_or_folder_for_deleting):
                   shutil.rmtree(path+file_or_folder_for_deleting)
