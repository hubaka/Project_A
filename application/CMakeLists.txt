cmake_minimum_required(VERSION 2.8)

include(cmakebuild)
set_project_id("_PROJECT_NAME_")

add_source_layers(
	"s" "windows"
	"s" "common"
	"s" "database"
	"s" "grid"
	"s" "errorhandle"
)

add_subfolder_dependency()