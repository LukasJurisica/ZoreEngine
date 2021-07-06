cmake_minimum_required(VERSION 3.8)

# Name of project
project(ZoreEngine VERSION 0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set("SRC_DIR" "${PROJECT_SOURCE_DIR}/src")

# Specify project files
file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.hpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "src" FILES ${SRC_FILES})
include_directories("src")
file (GLOB_RECURSE SHADER_FILES "assets/shaders/*.glsl")
source_group("shaders" FILES ${SHADER_FILES})

# Specify Dependencies
file(GLOB_RECURSE DEP_FILES "deps/*.h" "deps/*.hpp" "deps/*.inl" "deps/*.c")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/deps" PREFIX "deps" FILES ${DEP_FILES})
include_directories("deps")

# Add path name to configuration file
configure_file(config/path_config.h.in config/path_config.h)
include_directories("${PROJECT_BINARY_DIR}/config")

# Add executable based on the source files
if(WIN32)
	add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES} ${DEP_FILES} ${SHADER_FILES})
else()
	add_executable(${PROJECT_NAME} ${SRC_FILES} ${DEP_FILES} ${SHADER_FILES})
endif(WIN32)

# Import libraries
include_directories(${PROJECT_SOURCE_DIR}/deps)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
if(NOT WIN32)
    find_library(GLFW_LIBRARY GLFW)
elseif(WIN32)
    find_library(GLFW_LIBRARY glfw3 HINTS ${PROJECT_SOURCE_DIR}/deps/glfw)
endif(NOT WIN32)
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARY})

# Copy over asset files
file(COPY "assets" DESTINATION ${CMAKE_BINARY_DIR})

#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# This will use the proper libraries in debug mode in Visual Studio
if(WIN32)
	set(CMAKE_SUPPRESS_REGENERATION true)
    #set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	#set_property(TARGET MyTarget PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
endif(WIN32)