#pragma once
#include "core/Camera.hpp"
#include "devices/Mouse.hpp"
#include "devices/Keyboard.hpp"

namespace zore {

	//========================================================================
	//	Player Class
	//========================================================================

	class Player : MouseListener, KeyListener {
	public:
		Player(Camera* camera, const glm::vec3& position);
		~Player() = default;

		void Update(float deltaTime);

	protected:
		void OnMouseScroll(float xOffset, float yOffset) override;
		void OnMouseMove(float x, float y, float dx, float dy) override;

	private:
		glm::vec3 position;
		float yaw, pitch;
		float sensitivity;
		Camera* camera;
	};
}