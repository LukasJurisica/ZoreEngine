# ========================================================================
#	Configuration
# ========================================================================

cmake_minimum_required(VERSION 3.24)
project(ZoreEngine)
option(ZORE_BUILD_TESTS OFF)
option(ZORE_BUILD_EXAMPLES OFF)
option(ZORE_BUILD_PROFILER ON)
option(ZORE_BUILD_SPIRV_COMPILER ON)

message("-------------------------------------------------------------------")
message("  Beginning Build of Zore Engine")
message("-------------------------------------------------------------------")

#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
list(PREPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Detects whether this is a top-level project
get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(HAS_PARENT)
    set(TOPLEVEL_PROJECT OFF)
else()
    set(TOPLEVEL_PROJECT ON)
endif()

# ========================================================================
#	Dependencies
# ========================================================================

include("cmake/CPM.cmake")
include("cmake/glm.cmake")
include("cmake/glfw.cmake")
set(ZORE_LIBRARIES "glm" "glfw" "imgui")
set(ZORE_INCLUDES "src" "deps/include")

# Add rendering backend dependencies
set (VULKAN_REQUIRED_COMPONENTS shaderc_combined)
if(APPLE)
    list(APPEND VULKAN_REQUIRED_COMPONENTS MoltenVK)
    list(APPEND ZORE_LIBRARIES "-framework IOSurface" "-framework Metal" "-framework QuartzCore" "-framework Foundation" "-framework CoreGraphics")
endif()
find_package(Vulkan REQUIRED COMPONENTS ${VULKAN_REQUIRED_COMPONENTS})
if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "You must install the Vulkan SDK here: https://vulkan.lunarg.com/")
endif()
list(APPEND ZORE_INCLUDES ${Vulkan_INCLUDE_DIRS})
list(APPEND ZORE_LIBRARIES Vulkan::Vulkan Vulkan::shaderc_combined)

# Add IMGUI here as it may rely on vulkan include directories
add_subdirectory("deps/imgui")

# Add Tracy as a dependency if the ZORE_BUILD_PROFILER option is set
if(ZORE_BUILD_PROFILER)
	include ("cmake/tracy.cmake")
	list(APPEND ZORE_LIBRARIES TracyClient)
endif()

if(WIN32)
	# Link the Winsock2 library if we are compiling for windows
	list(APPEND ZORE_LIBRARIES ws2_32)
    list(APPEND ZORE_EXCLUDE_DIRS "src/zore/platform/macos" "src/zore/platform/linux")
elseif(APPLE)
    list(APPEND ZORE_EXCLUDE_DIRS "src/zore/platform/win32" "src/zore/platform/linux")
endif()

# ========================================================================
#	Core Project Source Definition
# ========================================================================

file(GLOB_RECURSE ALL_FILES "src/*.cpp" "src/*.hpp" "src/*.inl" "src/*.mm")
foreach(EXCLUDE_DIR ${ZORE_EXCLUDE_DIRS})
    foreach(FILE ${ALL_FILES})
        string(FIND "${FILE}" "${EXCLUDE_DIR}" FOUND)
        if(NOT ${FOUND} EQUAL -1)
            list(REMOVE_ITEM ALL_FILES "${FILE}")
        endif()
    endforeach()
endforeach()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src/zore" PREFIX "src" FILES ${ALL_FILES})
add_library(${PROJECT_NAME} ${ALL_FILES})

configure_file("config/path_config.h.in" "config/path_config.h")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/config")
set_target_properties(${PROJECT_NAME} PROPERTIES
	CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE
	CXX_STANDARD 20
)

#target_compile_definitions(${PROJECT_NAME} PUBLIC ${ZORE_DEFINES})
target_include_directories(${PROJECT_NAME} PUBLIC ${ZORE_INCLUDES})
target_link_libraries(${PROJECT_NAME} ${ZORE_LIBRARIES})

# ========================================================================
#	SPIRV_COMPILER
# ========================================================================

# if(${ZORE_BUILD_SPIRV_COMPILER} OR TRUE)
	# Set(ZORE_SPIRV_COMPILER_PROJECT "ZoreEngineSpirvCompiler")
	# if(WIN32)
		# add_executable(${ZORE_SPIRV_COMPILER_PROJECT} "spirv_compiler/main.cpp")
	# else()
		# add_executable(${ZORE_SPIRV_COMPILER_PROJECT} "spirv_compiler/main.cpp")
	# endif()
	# target_link_libraries(${ZORE_SPIRV_COMPILER_PROJECT} PRIVATE Vulkan::shaderc_combined)
	# set_target_properties(${ZORE_SPIRV_COMPILER_PROJECT} PROPERTIES CXX_STANDARD 20)
# endif()

# ========================================================================
#	Examples
# ========================================================================

if (${ZORE_BUILD_EXAMPLES} OR TOPLEVEL_PROJECT)
	Set(ZORE_ENGINE_DEMO_PROJECT "ZoreEngineDemoProject")
	if(WIN32)
    #WIN32
		add_executable(${ZORE_ENGINE_DEMO_PROJECT} "examples/01_sandbox application/main.cpp" "examples/01_sandbox application/main.hpp")
	elseif(APPLE)
    #MACOSX_BUNDLE
        add_executable(${ZORE_ENGINE_DEMO_PROJECT} "examples/01_sandbox application/main.cpp" "examples/01_sandbox application/main.hpp")
    else()
		add_executable(${ZORE_ENGINE_DEMO_PROJECT} "examples/01_sandbox application/main.cpp" "examples/01_sandbox application/main.hpp")
	endif()
	target_link_libraries(${ZORE_ENGINE_DEMO_PROJECT} PUBLIC ${PROJECT_NAME})
	set_target_properties(${ZORE_ENGINE_DEMO_PROJECT} PROPERTIES CXX_STANDARD 20)
endif()