cmake_minimum_required(VERSION 2.8)

# force Unicode over Multi-byte
# building the project as unicode instead of multi-byte project
if(MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
	set(BFW_ROOT_DIR TRUE)
	set(BFW_TARGET_OBJECTS CACHE INTERNAL "")
else()
	set(BFW_ROOT_DIR FALSE)
endif()

#-----------------------------------------------------------------------------------------
# MACRO	set_project_id
#		Folder name is set as the project id, which will be added to the build
#
# INPUT
#		${ARGN}	: Folder name
#
# OUTPUT
#		none	: 
#
#-----------------------------------------------------------------------------------------
macro(set_project_id id)
	# folder name is set as project id
	get_filename_component(PROJECT_ID ${CMAKE_CURRENT_SOURCE_DIR} NAME)
	string(REPLACE " " "_" PROJECT_ID ${PROJECT_ID})
	project(${PROJECT_ID})
	
	# Turn on the ability to create folders to organize projects (.vcproj)
	# It creates "CMakePredefinedTargets" folder by default and adds CMake
	# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	
	getLibraryName(LIBRARYNAME ${PROJECT_ID})
	
	# Creates the intall directory to store the binaries, libraries & executables
	if(BFW_ROOT_DIR)
		file(MAKE_DIRECTORY ${PROJECT_INSTALL_DIRECTORY})
	elseif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/source)
		file(MAKE_DIRECTORY ${PROJECT_INSTALL_DIRECTORY}/${PROJECT_ID})
	endif()
	_config_module()
endmacro()

#-----------------------------------------------------------------------------------------
# FUNC	getLibraryName
#		Based on the build options ("BUILD_TYPE"), library name is created and returned
#
# INPUT
#		libraryName	: "Name" in which the library name will be created.
#		folderName	: folder name
#
# OUTPUT
#		libraryName : library name
#-----------------------------------------------------------------------------------------
function(getLibraryName libraryName folderName)
	# Debug versus release naming
	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		set(BUILD_TYPE "d")
	else()
		set(BUILD_TYPE "r")
	endif()
	set(${libraryName} ${folderName}_${BUILD_TYPE} PARENT_SCOPE)
endfunction()

#-----------------------------------------------------------------------------------------
# MACRO	_config_module
#		This macro does the basic module configuration
#
# INPUT
#		none	: 
#
# OUTPUT
#		none	: 
#
#		NOTE: 
#-----------------------------------------------------------------------------------------
macro(_config_module)
	if(NOT BFW_ROOT_DIR)
		#
		# Initialize some module variables that will be used to collect different kind of files
		#
		set(${PROJECT_ID}_PUBLIC_HEADER)		# Module public header files list
		set(${PROJECT_ID}_SRC)					# Module source code files list
		set(${PROJECT_ID}_LIBS)					# Module libs list
		set(${PROJECT_ID}_DEPENDS)				# Module dependency list

		# Include needed directories
		include_directories(
			"${CMAKE_CURRENT_SOURCE_DIR}/source"							# Module's common source and header files
		)
	endif()
  
endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	add_source_layers
#		Source layer is added to the source list, which will be used to set these
#		directories as the sub directory for building
#
# INPUT
#		${ARGN}	: List of the folder names, which will be added as source folders
#					- First argument will be depicting the type of folder 
#							("s" as source folder)
#					- Second argument will be mentioning the name of the folder
#
# OUTPUT
#		none	: 
#
#-----------------------------------------------------------------------------------------
macro(add_source_layers)
	if(${ARGC})
		set(src_type FALSE)
		# _src_layer_list - list to store the source folder names
		set(_src_layer_list)
		foreach(idx ${ARGN})
			if (NOT src_type)
				set(src_type ${idx})
			else()
				if (${src_type} STREQUAL "s")
					list(APPEND _src_layer_list ${idx})
				else()
					message(FATAL_ERROR "invalid argument")
				endif()
				set(src_type FALSE)
			endif()
		endforeach()
	endif()
	if (_src_layer_list)
		_add_source_layers(${_src_layer_list})
	endif()
endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	_add_source_layers
#		Adds the source folders from list received via argument to the build as subdirectory
#
# INPUT
#		${ARGN}	: List of the folder names, which will be added as source folders
#					- First argument will be depicting the type of folder 
#							("s" as source folder)
#					- Second argument will be mentioning the name of the folder
#
# OUTPUT
#		none	: 
#
#-----------------------------------------------------------------------------------------
macro(_add_source_layers)
	if (${ARGC})
		foreach(idx ${ARGN})
			set(folder_source_path ${CMAKE_SOURCE_DIR}/../${idx})
			set(folder_binary_path ${CMAKE_BINARY_DIR}/${idx})
			if (IS_DIRECTORY ${folder_source_path})
				add_subdirectory(${folder_source_path} ${folder_binary_path})
			else()
				message(FATAL_ERROR "Could not find the directory of folder: ${idx}")
			endif()
		endforeach()
	endif()
endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	add_subfolder_dependency
#		This macro checks whether "subfolder.cmake" is available.
#		If the above mentioined cmake script is not available, then sub folders will not be
#		added as sub-directories.
#		If the cmake script is available, then folders mentioned inside this script shall be
#		added as sub-directories.
#
# INPUT
#		none
#
# OUTPUT
#		none	: 
#
#-----------------------------------------------------------------------------------------
macro(add_subfolder_dependency)
	set(subfolder ${CMAKE_CURRENT_SOURCE_DIR}/subfolder.cmake)
	if (EXISTS ${subfolder})
		include(${subfolder})
		set(subfolder_list ${${PROJECT_ID}_SUB_FOLDER_LIST})
	endif()
	foreach(idx ${subfolder_list})
		set(subfolder_path ${CMAKE_CURRENT_SOURCE_DIR}/${idx})
		if (EXISTS ${subfolder_path})
			add_subdirectory(${idx})
		else()
			message(FATAL_ERROR "error: unable to find subfolder: ${subfolder_path}")
		endif()
	endforeach()
endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	add_subfolder
#		Adds the sub folder within current directory to the build
#
# INPUT
#		${ARGC} List of sub folder names
#
# OUTPUT
#		${PROJECT_ID}_SUB_FOLDER_LIST - updates this list with sub-folder names
#
#-----------------------------------------------------------------------------------------
macro(add_subfolder)
	if (${ARGC})
		foreach(idx ${ARGN})
			list(APPEND ${PROJECT_ID}_SUB_FOLDER_LIST ${idx})
		endforeach()
	endif()
endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	add_folder_dependencies
#		Folder mentioned will be included as directory dependencies while 
#		building the corresponding library/executable
#
# INPUT
#		${ARGC}	-	List of the folder names, which should be included with the 
#					library/executable build
#
# OUTPUT
#		${PROJECT_ID}_DEPENDS - This shall be updated with the folder names.
#
#-----------------------------------------------------------------------------------------
macro(add_folder_dependencies)
	if (${ARGC})
		foreach(foldername ${ARGN})
			set(COMPONENT_PATH "NOT_FOUND")
			if (${COMPONENT_PATH} STREQUAL "NOT_FOUND")
				set(_layer_dir ${PROJECT_INSTALL_DIRECTORY})
				# Try to find an external component in the install directory
				if (IS_DIRECTORY ${_layer_dir})
					get_filename_component(COMPONENT_PATH ${_layer_dir}/${foldername} ABSOLUTE)
				endif()
			endif()
			if (NOT ${COMPONENT_PATH} STREQUAL "NOT_FOUND")
				# We've found the dependency, so include its public headers folder
				include_directories(${COMPONENT_PATH})
				list(APPEND ${PROJECT_ID}_DEPENDS ${foldername})
			endif()
		endforeach()
		
		if(${PROJECT_ID}_DEPENDS)
			list(REMOVE_DUPLICATES ${PROJECT_ID}_DEPENDS)
		endif()
	endif()
endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	add_public_header_files
#		creates list of header files, which will be accessed across project
#
# INPUT
#		{ARGC}	-	List of the header file names
#
# OUTPUT
#		${PROJECT_ID}_PUBLIC_HEADER - updates this list with the header file names
#
#-----------------------------------------------------------------------------------------
macro(add_public_header_files)
	if (${ARGC})
		foreach(hdr_filename ${ARGN})
			_add_header_file(${CMAKE_CURRENT_SOURCE_DIR}/source/${hdr_filename})
		endforeach()
	endif()
endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	_add_header_file
#		creates list of header files, which will be accessed across project
#
# INPUT
#		{ARGC}	-	List of the header file names
#
# OUTPUT
#		${PROJECT_ID}_PUBLIC_HEADER - updates this list with the header file names
#		
#-----------------------------------------------------------------------------------------
macro(_add_header_file hdr_file)
	get_filename_component(hdr_file_absolute_path ${hdr_file} ABSOLUTE)
	if (${hdr_file_absolute_path} MATCHES \\.h$|\\.hpp$)
		if(NOT EXISTS ${hdr_file_absolute_path})
			message(FATAL_ERROR "${hdr_file_absolute_path} does not exist")
		endif()
		list(APPEND ${PROJECT_ID}_PUBLIC_HEADER ${hdr_file_absolute_path})
	endif()
endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	add_source_files
#		creates list of source files, which will be added to build
#
# INPUT
#		{ARGC}	-	List of the source file names
#
# OUTPUT
#		${PROJECT_ID}_SRC - updates this list with the source file names
#
#-----------------------------------------------------------------------------------------
macro(add_source_files)
	if (${ARGC})
		foreach(src ${ARGN})
			_add_source_file(${CMAKE_CURRENT_SOURCE_DIR}/source/${src})
		endforeach()
	endif()
endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	_add_source_file
#		creates list of source files, which will be added to build
#
# INPUT
#		{ARGC}	-	List of the source file names
#
# OUTPUT
#		${PROJECT_ID}_SRC - updates this list with the source file names
#
#-----------------------------------------------------------------------------------------
macro(_add_source_file src_file)
	get_filename_component(src_file_absolute_path ${src_file} ABSOLUTE)
	if (${src_file_absolute_path} MATCHES \\.c$|\\.cpp$|\\.rc$)
		list(APPEND ${PROJECT_ID}_SRC ${src_file_absolute_path})
	else()
		message(FATAL_ERROR "${src_file_absolute_path} not found")
	endif()
endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	add_resource_files
#		creates list of resource files, which will be added to build
#
# INPUT
#		{ARGC}	-	List of the resource file names
#
# OUTPUT
#		${PROJECT_ID}_SRC - updates this list with the resource file names
#
#-----------------------------------------------------------------------------------------
macro(add_resource_files)
	if (${ARGC})
		foreach(src ${ARGN})
			_add_source_file(${CMAKE_CURRENT_SOURCE_DIR}/source/${src})
		endforeach()
	endif()
endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	_add_resource_file
#		creates list of resource files, which will be added to build
#
# INPUT
#		{ARGC}	-	List of the resource file names
#
# OUTPUT
#		${PROJECT_ID}_SRC - updates this list with the resource file names
#
#-----------------------------------------------------------------------------------------
macro(_add_resource_file resrc_file)
	get_filename_component(resrc_file_absolute_path ${resrc_file} ABSOLUTE)
	if (${resrc_file_absolute_path} MATCHES \\.rc$)
		list(APPEND ${PROJECT_ID}_SRC ${resrc_file_absolute_path})
	else()
		message(FATAL_ERROR "${resrc_file_absolute_path} not found")
	endif()
endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	install_module_lib
#		Library of the module is built in this function
#
# INPUT
#		none
#
# OUTPUT
#		module library
#
#-----------------------------------------------------------------------------------------
macro(install_module_lib)
	if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/source)
		set(_target_objects ${BFW_TARGET_OBJECTS})
		list(APPEND ${_target_objects} ${LIBRARYNAME})
		set(BFW_TARGET_OBJECTS ${_target_objects} CACHE INTERNAL "")
	else()
		message(FATAL_ERROR "There is no source folder")
	endif()
	
	if(${PROJECT_ID}_PUBLIC_HEADER)
		list(REMOVE_DUPLICATES ${PROJECT_ID}_PUBLIC_HEADER)
	endif()
	
	_add_module_lib()
	
	get_filename_component(_LIB_INSTALL_DIR ${PROJECT_INSTALL_DIRECTORY}/${PROJECT_ID} ABSOLUTE)
	
	#
	# Purge any headers that are no longer part of the public ones.
	#
	file(GLOB _fullfoundhdrs "${_LIB_INSTALL_DIR}/*.h*")
	set(_install_hdrs)
	foreach(_header ${_fullfoundhdrs})
		get_filename_component(_name ${_header} NAME)
		list(APPEND _install_hdrs ${_name})
	endforeach()
	if(_install_hdrs)
		set(_hdrs)
		foreach(_header ${${PROJECT_ID}_PUBLIC_HEADER})
			get_filename_component(_name ${_header} NAME)
			list(APPEND _hdrs ${_name})
		endforeach()
		
		foreach(_header ${_hdrs})
			list(REMOVE_ITEM _install_hdrs ${_hdrs})
		endforeach()
		
		foreach(_header ${_install_hdrs})
			message("Purging dead header file: ${_LIB_INSTALL_DIR}/${_header}")
			execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${_LIB_INSTALL_DIR}/${_header})
		endforeach()
	endif()
	
	#
	# Copy public header files to install dir
	#
	set(_clean_headers)
	foreach(_header ${${PROJECT_ID}_PUBLIC_HEADER})
		configure_file(${_header} ${_LIB_INSTALL_DIR} COPYONLY)
	endforeach()
	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${_clean_headers}")

endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	_add_module_lib
#		
#
# INPUT
#		
#
# OUTPUT
#		none	: 
#
#-----------------------------------------------------------------------------------------
macro(_add_module_lib)
		add_library(${LIBRARYNAME} OBJECT ${${PROJECT_ID}_SRC})
endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	install_module_exe
#		This macro adds the current module as an executable to the build system
#
# INPUT
#		none	: 
#
# OUTPUT
#		none	: 
#
#-----------------------------------------------------------------------------------------
macro(install_module_exe)
	#
	# Set the executable output path
	#
	get_filename_component(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_INSTALL_DIRECTORY}/../images/ ABSOLUTE)
	#
	# Install folder
	#
	file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	_add_module_exe()
endmacro()

#-----------------------------------------------------------------------------------------
# MACRO	_add_module_exe
#		This macro adds the current module as an executable to the build system
#
# INPUT
#		none	: 
#
# OUTPUT
#		none	: 
#
#-----------------------------------------------------------------------------------------
macro(_add_module_exe)
	
	set(_obj_libs)
	foreach(_lib ${${PROJECT_ID}_DEPENDS})
		getLibraryName(_libname ${_lib})
		list(APPEND _obj_libs "$<TARGET_OBJECTS:${_libname}>")
	endforeach()

	add_executable(${LIBRARYNAME} ${${PROJECT_ID}_SRC} ${_obj_libs})
#	target_link_libraries(${LIBRARYNAME} ${${PROJECT_ID}_LIBS})
	
	if(MSVC)
		# 	/SUBSYSTEM:WINDOWS --> 	this flag creates the project as "windows project" 
		#							instead of "windows console project"
		set_target_properties(${LIBRARYNAME} PROPERTIES LINK_FLAGS_DEBUG   "/NODEFAULTLIB:msvcrt /NODEFAULTLIB:libcd /SUBSYSTEM:WINDOWS")
		set_target_properties(${LIBRARYNAME} PROPERTIES LINK_FLAGS_RELEASE "/NODEFAULTLIB:msvcrtd /NODEFAULTLIB:libcd /SUBSYSTEM:WINDOWS")
	endif()
endmacro()