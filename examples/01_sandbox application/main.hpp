#include <zore/core/Application.hpp>
#include <zore/core/Camera.hpp>
#include <zore/core/ActionMap.hpp>
#include <zore/ui/Layer.hpp>

#include <zore/Devices.hpp>
#include <zore/Graphics.hpp>

namespace zore {

	class DemoApplication : public Application, Window::Listener, Mouse::Listener, Keyboard::Listener {
	public:
		DemoApplication(const LaunchOptions& options);
		~DemoApplication() = default;

		void ReloadShaders();

	private:
		void Run() override;
		void CreateSimpleUI();
		static DemoApplication* Get();

		bool OnMouseMove(float x, float y, float dx, float dy) override;
		bool OnMousePress(int button) override;
		bool OnMouseRelease(int button) override;
		bool OnKeyPress(int key, int mods) override;
		bool OnKeyRelease(int key) override;
		void OnWindowResize(int width, int height, float aspect_ratio) override;

	private:
		Camera2D m_camera;
		Shader m_quad_shader;
		Shader m_panel_shader;
		Shader m_text_shader;
		UI::Layer& m_main_menu;
		ActionMap action_map;
	};
}