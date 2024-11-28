cmake_minimum_required(VERSION 3.10)

project(ZoreEngine)

option(ZORE_BUILD_TESTS OFF)
option(ZORE_BUILD_EXAMPLES OFF)
option(ZORE_BUILD_PROFILER ON)

# Detects whether this is a top-level project
get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(HAS_PARENT)
    set(TOPLEVEL_PROJECT OFF)
else()
    set(TOPLEVEL_PROJECT ON)
endif()

list(PREPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Specify project files
file(GLOB_RECURSE ALL_FILES "src/*.cpp" "src/*.hpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src/zore" PREFIX "src" FILES ${ALL_FILES})
add_library(${PROJECT_NAME} ${ALL_FILES})

configure_file("config/path_config.h.in" "config/path_config.h")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/config")
set_target_properties(${PROJECT_NAME} PROPERTIES
	CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE
	CXX_STANDARD 20
)

# Add dependencies
include("cmake/CPM.cmake")
include("cmake/glm.cmake")
include("cmake/glfw.cmake")
add_subdirectory("deps/glad")
add_subdirectory("deps/imgui")
set(ZORE_LIBRARIES "glm" "glfw" "glad" "imgui")

# Add Tracy as a dependency if the ZORE_BUILD_PROFILER option is set
if (${ZORE_BUILD_PROFILER})
	include ("cmake/tracy.cmake")
	set(ZORE_LIBRARIES ${ZORE_LIBRARIES} TracyClient)
endif()
# Link the Winsock2 library if we are compiling for windows
if (WIN32)
	set(WS2_32_LIBRARY ws2_32)
	set(ZORE_LIBRARIES ${ZORE_LIBRARIES} ${WS2_32_LIBRARY})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "src" "deps/include")
target_link_libraries(${PROJECT_NAME} ${ZORE_LIBRARIES})

if (${ZORE_BUILD_EXAMPLES} OR TOPLEVEL_PROJECT)
	Set(ZORE_ENGINE_DEMO_PROJECT "ZoreEngineDemoProject")
	if(WIN32)
		add_executable(${ZORE_ENGINE_DEMO_PROJECT} WIN32 "examples/01_sandbox application/main.cpp" "examples/01_sandbox application/main.hpp")
	else()
		add_executable(${ZORE_ENGINE_DEMO_PROJECT} "examples/01_sandbox application/main.cpp" "examples/01_sandbox application/main.hpp")
	endif()
	target_link_libraries(${ZORE_ENGINE_DEMO_PROJECT} PUBLIC ${PROJECT_NAME})
	set_target_properties(${ZORE_ENGINE_DEMO_PROJECT} PROPERTIES CXX_STANDARD 20)
endif()