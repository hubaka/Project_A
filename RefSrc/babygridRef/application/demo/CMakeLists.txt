cmake_minimum_required(VERSION 2.8)

include(cmakebuild)
set_project_id("_PROJECT_NAME_")

#
# List with modules we depend on
#
add_folder_dependencies()

add_lib_dependencies()

#
# List of public module header files, relative to the module source folder
#
add_public_header_files(
	"simpleGrid.h"
	"main.h"
	"resource.h"
)
#
# List of resource files, relative to the module source folder
#
add_resource_files(
	"main.rc"
)
#
# List of source files, relative to the module source folder
#
add_source_files(
	"main.cpp"
	"simpleGrid.cpp"
)

#add_libs_to_exe(
#	${DEP_LIBS}
#)

#
# Build and install library
#
install_module_exe()