set(IMGUI_HEADERS
	"include/imconfig.h"
	"include/imgui_impl_glfw.h"
	"include/imgui_internal.h"
	"include/imgui.h"
	"include/imstb_rectpack.h"
	"include/imstb_textedit.h"
	"include/imstb_truetype.h"
)
set(IMGUI_SOURCES
	"src/imgui.cpp"
	"src/imgui_demo.cpp"
	"src/imgui_draw.cpp"
	"src/imgui_impl_glfw.cpp"
	"src/imgui_impl_opengl3.cpp"
	"src/imgui_tables.cpp"
	"src/imgui_widgets.cpp"
)

if (${ZORE_RENDER_BACKEND} STREQUAL "OpenGL")
    set(IMGUI_HEADERS ${IMGUI_HEADERS} "include/imgui_impl_opengl3.h" "include/imgui_impl_opengl3_loader.h")
    set(IMGUI_SOURCES ${IMGUI_SOURCES} "src/imgui_impl_opengl3.cpp")
elseif(${ZORE_RENDER_BACKEND} STREQUAL "Metal")
    set(IMGUI_HEADERS ${IMGUI_HEADERS} "include/imgui_impl_metal.h")
    set(IMGUI_SOURCES ${IMGUI_SOURCES} "src/imgui_impl_metal.mm")
elseif(${ZORE_RENDER_BACKEND} STREQUAL "Vulkan")
    set(IMGUI_HEADERS ${IMGUI_HEADERS} "include/imgui_impl_vulkan.h")
    set(IMGUI_SOURCES ${IMGUI_SOURCES} "src/imgui_impl_vulkan.cpp")
endif()

add_library(imgui ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_include_directories(imgui PUBLIC "include")
target_include_directories(imgui PRIVATE "${CMAKE_BINARY_DIR}/_deps/glfw-src/include")