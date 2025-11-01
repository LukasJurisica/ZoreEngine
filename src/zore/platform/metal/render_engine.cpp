#include "zore/graphics/render_engine.hpp"
#include "zore/platform/metal/metal_core.hpp"
#include "zore/devices/window.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

namespace zore {

    static NS::Window* s_metal_window = nullptr;
    static CA::MetalLayer* s_metal_layer = nullptr;

    //========================================================================
    //	Render Engine Interface
    //========================================================================

    void RenderEngine::Init() {
        s_metal_device = MTLCreateSystemDefaultDevice();
        s_metal_window = glfwGetCocoaWindow(Window::GetWindowHandle());
        s_metal_layer = [CAMetalLayer layer];
        // s_metal_layer.device = (__bridge id<MTLDevice>)s_metal_device;
        // s_metal_layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        // s_metal_window.contentView.layer = s_metal_layer;
        // s_metal_window.contentView.wantsLayer = YES;
    }

    void RenderEngine::Cleanup() {
        s_metal_layer->release();
        s_metal_device->release();
    }
}