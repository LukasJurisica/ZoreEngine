cmake_minimum_required (VERSION 3.8)

# Name of project
set(PROJECT_NAME "GameEngine")
project(${PROJECT_NAME})

# Specify project files
file(GLOB CORE_SOURCE "src/core/*.cpp" "src/core/*.hpp")
file(GLOB GRAPHICS_SOURCE "src/graphics/*.cpp" "src/graphics/*.hpp")
file(GLOB OPENGL_SOURCE "src/graphics/opengl/*.cpp" "src/graphics/opengl/*.hpp")
file(GLOB OPENGL_SHADER_SOURCE "src/graphics/opengl/shaders/*.vs" "src/graphics/opengl/shaders/*.fs")
file(GLOB UTILS_SOURCE "src/utils/*.cpp" "src/utils/*.hpp")
file(GLOB WINDOW_SOURCE "src/window/*.cpp" "src/window/*.hpp")
source_group("src\\core" FILES ${CORE_SOURCE})
source_group("src\\graphics" FILES ${GRAPHICS_SOURCE})
source_group("src\\graphics\\opengl" FILES ${OPENGL_SOURCE})
source_group("src\\graphics\\opengl\\shaders" FILES ${OPENGL_SHADER_SOURCE})
source_group("src\\utils" FILES ${UTILS_SOURCE})
source_group("src\\window" FILES ${WINDOW_SOURCE})
source_group("src\\shaders" FILES ${SHADER_SOURCE})
include_directories("src")

#Specify Dependencies
set(DEP_INC "deps/include")
set(DEP_LIB "deps/libs")
set(DEP_SRC "deps/src")
file(GLOB GLAD_SOURCE "${DEP_SRC}/glad/glad.c")
source_group("dependencies\\glad" FILES ${GLAD_SOURCE})
include_directories(${DEP_INC})

# Add path name to configuration file
configure_file(config/config.h.in config/config.h)
# Add executable based on the header and source files
add_executable(${PROJECT_NAME}
	${CORE_SOURCE}
	${GRAPHICS_SOURCE}
	${OPENGL_SOURCE}
	${OPENGL_SHADER_SOURCE}
	${UTILS_SOURCE}
	${WINDOW_SOURCE}
	${GLAD_SOURCE}
)

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
# Other libraries needed
find_library(GLFW_LIBRARY glfw3 HINTS ${DEP_LIB})
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARY})

if (WIN32)
	set(CMAKE_SUPPRESS_REGENERATION true)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif (WIN32)