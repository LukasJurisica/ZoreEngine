#pragma once
#include "core/Camera.hpp"
#include "devices/Mouse.hpp"
#include "devices/Keyboard.hpp"
#include "utils/DataTypes.hpp"

namespace zore {

	class Chunk;

	//========================================================================
	//	Player Class
	//========================================================================

	class Player : MouseListener, KeyListener {
	public:
		Player(Camera* camera, const glm::vec3& position);
		~Player() = default;

		void Update(float deltaTime);

	protected:
		void OnMouseMove(float x, float y, float dx, float dy) override;
		void OnMousePress(int button) override;
		void OnMouseScroll(float xOffset, float yOffset) override;
		void OnKeyPress(int key, int mods) override;

	private:
		glm::vec3 position;
		glm::vec3 size;
		float vSpeed;
		float yaw, pitch;
		float sensitivity;
		Camera* camera;

		bool grounded;
		bool flying;
		bool noclip;
		ushort heldBlock;
	};
}