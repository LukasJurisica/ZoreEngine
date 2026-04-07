#include <zore/core/application.hpp>
#include <zore/core/camera.hpp>
#include <zore/core/action_map.hpp>
#include <zore/ui/layer.hpp>
#include <zore/devices.hpp>
#include <zore/graphics.hpp>
#include <zore/events.hpp>

namespace zore {

	class DemoApplication : public Application {
	public:
		DemoApplication(const LaunchOptions& options);
		~DemoApplication() = default;

	private:
		void Run() override;
		void CreateSimpleUI();
		void ReloadShaders(ActionMap::Key);

		bool OnWindowResize(const WindowResizedEvent& e);

	private:
		Camera2D m_camera;
		Shader m_quad_shader;
		Shader m_panel_shader;
		Shader m_text_shader;
		ComputeShader m_compute_shader;
		ActionMap action_map;

		event::MultiHandler m_event_handler;
	};
}