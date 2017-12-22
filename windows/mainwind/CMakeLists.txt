cmake_minimum_required(VERSION 2.8)

include(cmakebuild)
set_project_id("_PROJECT_NAME_")

add_folder_dependencies(
	"basic"
	"resource"
	"igrid"
	"errhandle"
	"toolbar"
	"dbms"
)

add_lib_dependencies(
	"sqlite"
)
#
# List of public module header files, relative to the module source folder
#
add_public_header_files(
	"mainwind.h"
)
#
# List of source files, relative to the module source folder
#
add_source_files(
	"mainwind.cpp"
)
#
# Build and install library
#
install_module_lib()