file(GLOB_RECURSE ALL_FILES "src/*.cpp" "src/*.hpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src/zore" PREFIX "src" FILES ${ALL_FILES})
add_library(zorEngine ${ALL_FILES})

configure_file("config/path_config.h.in" "config/path_config.h")
target_include_directories(zorEngine PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/config")

add_subdirectory("deps/glad")
add_subdirectory("deps/imgui")
target_include_directories(zorEngine PUBLIC "src" "deps/include")

if(WIN32)
	find_library(GLFW_LIBRARY glfw3 HINTS "${CMAKE_CURRENT_SOURCE_DIR}/deps/include/glfw")
else()
	find_library(GLFW_LIBRARY GLFW)
endif()
target_link_libraries(zorEngine glad imgui ${GLFW_LIBRARY})