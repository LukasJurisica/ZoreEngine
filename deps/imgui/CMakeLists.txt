set(IMGUI_HEADERS
	"include/imconfig.h"
	"include/imgui_impl_glfw.h"
	"include/imgui_internal.h"
	"include/imgui.h"
	"include/imstb_rectpack.h"
	"include/imstb_textedit.h"
	"include/imstb_truetype.h"
    "include/imgui_impl_vulkan.h"
)
set(IMGUI_SOURCES
	"src/imgui.cpp"
	"src/imgui_demo.cpp"
	"src/imgui_draw.cpp"
	"src/imgui_impl_glfw.cpp"
	"src/imgui_tables.cpp"
	"src/imgui_widgets.cpp"
    "src/imgui_impl_vulkan.cpp"
)
set(IMGUI_INCLUDES "${CMAKE_BINARY_DIR}/_deps/glfw-src/include" ${Vulkan_INCLUDE_DIRS})

add_library(imgui ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_include_directories(imgui PUBLIC "include" PRIVATE ${IMGUI_INCLUDES})