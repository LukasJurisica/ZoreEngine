cmake_minimum_required (VERSION 3.8)

# Name of project
set(PROJECT_NAME "ZoreEngine")
project(${PROJECT_NAME})

# Specify project files
file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.hpp" "src/*.glsl")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}\\src" PREFIX "src" FILES ${SRC_FILES})
include_directories("src")

#Specify Dependencies
file(GLOB_RECURSE DEP_FILES "deps/*.h" "deps/*.hpp" "deps/*.inl" "deps/*.c")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}\\deps" PREFIX "deps" FILES ${DEP_FILES})
include_directories("deps")

# Add executable based on the header and source files
add_executable(${PROJECT_NAME} ${SRC_FILES} ${DEP_FILES})

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
# Other libraries needed
find_library(GLFW_LIBRARY glfw3 HINTS "deps/glfw")
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARY})

# Copy over asset files
file(COPY "assets" DESTINATION ${CMAKE_BINARY_DIR})

if (WIN32)
	set(CMAKE_SUPPRESS_REGENERATION true)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	#set_property(TARGET MyTarget PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
endif (WIN32)