cmake_minimum_required(VERSION 3.23)

project(ZoreEngine)

option(ZORE_BUILD_TESTS "Build ZoreEngine tests" OFF)
option(ZORE_BUILD_EXAMPLES "Build ZoreEngine examples" OFF)
option(ZORE_BUILD_PROFILER "Enable profiler tooling" ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
list(PREPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Specify project files
file(GLOB_RECURSE ALL_FILES CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp" "src/*.inl")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src/zore" PREFIX "src" FILES ${ALL_FILES})
add_library(${PROJECT_NAME} ${ALL_FILES})

configure_file("config/path_config.h.in" "config/path_config.h")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/config")
set_target_properties(${PROJECT_NAME} PROPERTIES
	INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
	CXX_STANDARD 20
)

# Add dependencies
include("cmake/CPM.cmake")
include("cmake/glfw.cmake")
include("cmake/tlse.cmake")
add_subdirectory("deps/glad")
add_subdirectory("deps/imgui")
set(ZORE_LIBRARIES "glfw" "glad" "imgui" "tlse")

# Add Tracy as a dependency if the ZORE_BUILD_PROFILER option is set
if (${ZORE_BUILD_PROFILER})
	include ("cmake/tracy.cmake")
	set(ZORE_LIBRARIES ${ZORE_LIBRARIES} TracyClient)
endif()
# Link the Winsock2 library if we are compiling for windows
if (WIN32)
	set(ZORE_LIBRARIES ${ZORE_LIBRARIES} ws2_32)
endif()

# Add TLS Support
set(ZORE_CA_BUNDLE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/deps/certificates/cacert.pem")
if(NOT EXISTS "${ZORE_CA_BUNDLE_FILE}")
    message(FATAL_ERROR "CA certificate bundle not found: ${ZORE_CA_BUNDLE_FILE}" )
endif()
file(READ "${ZORE_CA_BUNDLE_FILE}" ZORE_CA_BUNDLE_CONTENT)
configure_file("config/ca_bundle.cpp.in" "generated/ca_bundle.cpp" @ONLY)
target_sources(${PROJECT_NAME} PRIVATE "generated/ca_bundle.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC "src" "deps/include")
target_link_libraries(${PROJECT_NAME} ${ZORE_LIBRARIES})

if (${ZORE_BUILD_EXAMPLES} OR PROJECT_IS_TOP_LEVEL)
	Set(ZORE_ENGINE_DEMO_PROJECT "ZoreEngineDemoProject")
	if(WIN32)
		add_executable(${ZORE_ENGINE_DEMO_PROJECT} WIN32 "examples/01_sandbox application/main.cpp" "examples/01_sandbox application/main.hpp")
		target_link_options(${ZORE_ENGINE_DEMO_PROJECT} PRIVATE "/ENTRY:wWinMainCRTStartup")
	else()
		add_executable(${ZORE_ENGINE_DEMO_PROJECT} "examples/01_sandbox application/main.cpp" "examples/01_sandbox application/main.hpp")
	endif()
	target_link_libraries(${ZORE_ENGINE_DEMO_PROJECT} PUBLIC ${PROJECT_NAME})
	set_target_properties(${ZORE_ENGINE_DEMO_PROJECT} PROPERTIES CXX_STANDARD 20)
endif()