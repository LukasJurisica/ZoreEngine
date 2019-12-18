cmake_minimum_required (VERSION 3.8)

# Name of project
set(PROJECT_NAME "GameEngine")
project(${PROJECT_NAME})

# Specify project files
file(GLOB ROOT_SOURCE src/*.cpp)
file(GLOB_RECURSE CORE_SOURCE src/Core/*.cpp)
file(GLOB_RECURSE CORE_HEADER src/Core/*.hpp)
file(GLOB_RECURSE OPENGL_SOURCE src/OpenGL/*.cpp src/OpenGL/*.c)
file(GLOB_RECURSE OPENGL_HEADER src/OpenGL/*.hpp)
file(GLOB_RECURSE SHADER_SOURCE shaders/*.vs shaders/*.fs)

source_group("Core Files" FILES ${ROOT_SOURCE})
source_group("Core Files" FILES ${CORE_SOURCE})
source_group("Core Files" FILES ${CORE_HEADER})
source_group("OpenGL Files" FILES ${OPENGL_SOURCE})
source_group("OpenGL Files" FILES ${OPENGL_HEADER})
source_group("Shader Files" FILES ${SHADER_SOURCE})

# Add path name to configuration file
configure_file(config/config.h.in config/config.h)
# Add executable based on the header and source files
add_executable(${PROJECT_NAME}
	${ROOT_SOURCE}
	${CORE_SOURCE}
	${CORE_HEADER}
	${OPENGL_SOURCE}
	${OPENGL_HEADER}
	${SHADER_SOURCE}
)

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
# Other libraries needed
set(LIBRARY_PATH "" CACHE PATH "External Library Dependencies")
include_directories(${LIBRARY_PATH}/include)
find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/libs)
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARY})

if (WIN32)
	set(CMAKE_SUPPRESS_REGENERATION true)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif (WIN32)