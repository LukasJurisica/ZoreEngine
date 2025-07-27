#include <zore/core/Application.hpp>
#include <zore/core/Camera.hpp>
#include <zore/core/ActionMap.hpp>
#include <zore/ui/Layer.hpp>

#include <zore/Devices.hpp>
#include <zore/Graphics.hpp>
#include <zore/Events.hpp>

namespace zore {

	class DemoApplication : public Application {
	public:
		DemoApplication(const LaunchOptions& options);
		~DemoApplication() = default;

		void ReloadShaders();

	private:
		void Run() override;
		void CreateSimpleUI();
		static DemoApplication* Get();

		bool OnMousePress(const MousePressedEvent& e);
		bool OnMouseRelease(const MouseReleasedEvent& e);
		bool OnKeyPress(const KeyPressedEvent& e);
		bool OnKeyRelease(const KeyReleasedEvent& e);
		bool OnWindowResize(const WindowResizedEvent& e);

	private:
		Camera2D m_camera;
		Shader m_quad_shader;
		Shader m_panel_shader;
		Shader m_text_shader;
		ActionMap action_map;

		event::MultiHandler m_event_handler;
	};
}