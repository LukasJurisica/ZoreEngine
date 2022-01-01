cmake_minimum_required(VERSION 3.8)
# Name of project
project(ZoreEngine VERSION 0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# This requires more research - May need to recompile glfw as well
set(CMAKE_CXX_FLAGS_RELEASE "/MD") #Switch this to MT to compile statically (rather than dynamically with dll)
set(CMAKE_CXX_FLAGS_DEBUG "/MDd")

# Specify project files
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.hpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "src" FILES ${SOURCE_FILES})
include_directories("src")
file (GLOB_RECURSE SHADER_FILES "assets/shaders/*")
source_group("shaders" FILES ${SHADER_FILES})

# Add path name to configuration file
configure_file(config/path_config.h.in config/path_config.h)
include_directories("${PROJECT_BINARY_DIR}/config")

# Add executable based on the source files
if(WIN32)
	add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES} ${SHADER_FILES})
	
	set(CMAKE_SUPPRESS_REGENERATION true)
	#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    #set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	#set_property(TARGET MyTarget PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
	
else()
	add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${SHADER_FILES})
endif(WIN32)

# Import libraries
include_directories("deps")
add_subdirectory("deps/glad")
add_subdirectory("deps/fnl")

if(NOT WIN32)
    find_library(GLFW_LIBRARY GLFW)
elseif(WIN32)
    find_library(GLFW_LIBRARY glfw3 HINTS ${PROJECT_SOURCE_DIR}/deps/glfw)
endif(NOT WIN32)
target_link_libraries(${PROJECT_NAME} 
	glad
	fastnoiselite
	${GLFW_LIBRARY}
)

# Copy over asset files
#file(COPY "assets" DESTINATION ${CMAKE_BINARY_DIR})