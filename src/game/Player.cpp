#include "game/Player.hpp"
#include "game/World.hpp"
#include "voxel/ChunkManager.hpp"
#include "math/MathUtils.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Player Class
	//========================================================================

	Player::Player(Camera* camera, const glm::vec3& position) : camera(camera), position(position), vSpeed(0), size(0.4f, 0.9f, 0.4f), sensitivity(0.1f), yaw(-90.f), pitch(0.f) {
		camera->SetPosition(position);
		camera->SetViewVectors({ 0, 0,-1 }, { 1, 0, 0 });

		flying = true;
		noclip = false;
		grounded = true;
	}

	void Player::Update(float deltaTime) {
		float speed = 10.f;
		glm::vec3 velocity = glm::vec3(0);

		if (Keyboard::GetKey(KEY_W))
			velocity += camera->GetFront();
		if (Keyboard::GetKey(KEY_S))
			velocity -= camera->GetFront();
		if (Keyboard::GetKey(KEY_D))
			velocity += camera->GetRight();
		if (Keyboard::GetKey(KEY_A))
			velocity -= camera->GetRight();
		if (Keyboard::GetKey(KEY_X) && !flying)
			speed *= 0.3f;
		else if (Keyboard::GetKey(KEY_L_SHIFT))
			speed *= 1.5f;

		if (velocity.x != 0.f || velocity.z != 0.f)
			velocity = glm::normalize(velocity) * speed;

		if (flying) {
			if (Keyboard::GetKey(KEY_SPACE))
				velocity.y += speed;
			if (Keyboard::GetKey(KEY_X))
				velocity.y -= speed;
			velocity *= 5.f;
		}
		else {
			velocity.y = vSpeed - 40.f * deltaTime;
			if (Keyboard::GetKey(KEY_SPACE) && grounded) {
				velocity.y = 10.f;
				grounded = false;
			}
		}

		vSpeed = velocity.y;
		velocity *= deltaTime;
		if (!noclip) {
			glm::bvec3 collision = World::DoCollision(position, velocity, size);
			if (collision[1]) {
				grounded = true;
				vSpeed = 0;
			}
			else {
				grounded = false;
			}
		}
		position += velocity;
		camera->SetPosition(position + glm::vec3(0.f, 1.75f - size.y, 0.f));
		ChunkManager::Update(camera);
	}

	void Player::OnMouseMove(float x, float y, float dx, float dy) {
		yaw += dx * sensitivity;
		yaw = zm::WrapClamp(yaw, -180.f, 180.f);
		pitch -= dy * sensitivity;
		pitch = zm::Clamp(pitch, -89.5f, 89.5f);
		camera->SetYawPitch(yaw, pitch);
	}

	void Player::OnMousePress(int button) {
		if (button == MOUSE_BUTTON_LEFT) {
			if (World::BreakBlock(camera->GetPosition(), camera->GetForward(), 10))
				Logger::Log("Block Broken");
		}
		else if (button == MOUSE_BUTTON_RIGHT) {
			if (World::PlaceBlock(camera->GetPosition(), camera->GetForward(), BLOCK_STONE, 10))
				Logger::Log("Block Placed");
		}
	}

	void Player::OnMouseScroll(float xOffset, float yOffset) {
		//camera->distance = zm::Clamp(camera->distance - (yOffset * 10), 0.f, 100.f);
	}

	void Player::OnKeyPress(int key, int mods) {
		if (key == KEY_F)
			flying = !flying;
		if (key == KEY_N)
			noclip = !noclip;
	}
}