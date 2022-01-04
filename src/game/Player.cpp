#include "game/Player.hpp"
#include "math/MathUtils.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Player Class
	//========================================================================

	Player::Player(Camera* camera, const glm::vec3& position) : camera(camera), position(position), sensitivity(0.1f), yaw(-90.f), pitch(0.f) {
		camera->SetPosition(position);
		camera->SetViewVectors({ 0, 0,-1 }, { 1, 0, 0 });
	}

	void Player::Update(float deltaTime) {
		float speed = 15.f * deltaTime;
		glm::vec3 velocity = glm::vec3(0);

		if (Keyboard::GetKey(KEY_W))
			velocity += camera->GetFront();
		if (Keyboard::GetKey(KEY_S))
			velocity -= camera->GetFront();
		if (Keyboard::GetKey(KEY_D))
			velocity += camera->GetRight();
		if (Keyboard::GetKey(KEY_A))
			velocity -= camera->GetRight();
		if (Keyboard::GetKey(KEY_SPACE))
			velocity += glm::vec3(0.f, 1.f, 0.f);
		if (Keyboard::GetKey(KEY_X))
			velocity -= glm::vec3(0.f, 1.f, 0.f);

		if (Keyboard::GetKey(KEY_L_SHIFT))
			speed *= 20;

		if (velocity.x != 0.f || velocity.y!= 0.f || velocity.z != 0.f)
			velocity = glm::normalize(velocity) * speed;

		position += velocity;
		camera->SetPosition(position);
	}

	void Player::OnMouseScroll(float xOffset, float yOffset) {
		//camera->distance = zm::Clamp(camera->distance - (yOffset * 10), 0.f, 100.f);
	}

	void Player::OnMouseMove(float x, float y, float dx, float dy) {
		yaw += dx * sensitivity;
		yaw = zm::WrapClamp(yaw, -180.f, 180.f);
		pitch -= dy * sensitivity;
		pitch = zm::Clamp(pitch, -89.5f, 89.5f);
		camera->SetYawPitch(yaw, pitch);
	}
}