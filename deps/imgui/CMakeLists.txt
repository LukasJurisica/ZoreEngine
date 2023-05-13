set(SOURCES
	src/imgui.cpp
	src/imgui_demo.cpp
	src/imgui_draw.cpp
	src/imgui_impl_glfw.cpp
	src/imgui_impl_opengl3.cpp
	src/imgui_tables.cpp
	src/imgui_widgets.cpp
)
add_library(imgui ${SOURCES})
target_include_directories(imgui PUBLIC "include")
target_include_directories(imgui PRIVATE "../include")