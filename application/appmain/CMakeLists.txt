cmake_minimum_required(VERSION 2.8)

include(cmakebuild)
set_project_id("_PROJECT_NAME_")

#
# List with modules we depend on
#
add_folder_dependencies(
	"resource"
	"mainwind"
	"sqlite"
)

#
# List of public module header files, relative to the module source folder
#
add_public_header_files()
#
# List of source files, relative to the module source folder
#
add_source_files(
	"t114.cpp"
)

#
# Build and install library
#
install_module_exe()