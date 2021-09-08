#include "core/Camera.hpp"
#include "math/MathUtils.hpp"
#include "debug/Logger.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace zore {

	//========================================================================
	//	3D Camera Class
	//========================================================================

	Camera::Camera(float fov, float aspectRatio, float nearDist, float farDist) :
		fov(glm::radians(fov)), aspectRatio(aspectRatio), nearDist(nearDist), farDist(farDist), viewMatrix(0), projectionMatrix(0) {
		distance = 0;
		UpdateProjectionMatrix();
	}

	void Camera::UpdateViewMatrix() {
		viewMatrix = glm::lookAt(position, position + forward, up);
		UpdateFrustum();
	}

	void Camera::UpdateProjectionMatrix() {
		projectionMatrix = glm::perspective(fov, aspectRatio, nearDist, farDist);
		farHeight = farDist * tan(fov * 0.5f);
		farWidth = farHeight * aspectRatio;
		UpdateFrustum();
	}

	void Camera::UpdateFrustum() {
		glm::vec3 lForward = forward * farDist;
		glm::vec3 lRight = right * farWidth;
		glm::vec3 lUp = up * farHeight;

		// Left Plane
		planeNormals[0] = glm::cross((lForward - lRight), up);
		// Right Plane
		planeNormals[1] = glm::cross(up, (lForward + lRight));
		// Top Plane
		planeNormals[2] = glm::cross((lForward + lUp), right);
		// Bottom Plane
		planeNormals[3] = glm::cross(right, (lForward - lUp));
	}

	bool Camera::TestPoint(float x, float y, float z) {
		glm::vec3 vector = glm::vec3(x, y, z) - position;
		for (int i = 0; i < 4; i++) {
			if (glm::dot(vector, planeNormals[i]) < 0)
				return false;
		}
		return true;
	}

	bool Camera::TestAABB(glm::vec3 min, glm::vec3 size) {
		for (int i = 0; i < 4; i++) {
			glm::vec3 vector = glm::vec3(min); // vector = min?
			for (int j = 0; j < 3; j++) {
				if (planeNormals[i][j] > 0)
					vector[j] += size[j];
			}
			if (glm::dot(vector - position, planeNormals[i]) < 0)
				return false;
		}
		return true;
	}

	void Camera::SetPosition(const glm::vec3& pos) {
		position = pos;// -(forward * distance);
		UpdateViewMatrix();
	}

	void Camera::SetYawPitch(float yaw, float pitch) {
		yaw = glm::radians(yaw);
		pitch = glm::radians(pitch);

		glm::vec2 cVal = glm::cos(glm::vec2(yaw, pitch));
		glm::vec2 sVal = glm::sin(glm::vec2(yaw, pitch));

		front = glm::vec3(cVal.x, 0, sVal.x);
		forward = glm::vec3(front.x * cVal.y, sVal.y, front.z * cVal.y);
		right = glm::cross(front, { 0.f, 1.f, 0.f });
		UpdateViewMatrix();
	}

	void Camera::SetViewVectors(const glm::vec3& f, const glm::vec3& r) {
		forward = f;
		right = r;
		up = glm::normalize(glm::cross(r, f));
		front = glm::normalize(glm::vec3(forward.x, 0, forward.z));
		UpdateViewMatrix();
	}

	void Camera::SetFOV(float f) {
		fov = f;
		UpdateProjectionMatrix();
	}

	void Camera::SetAspectRatio(float ar) {
		aspectRatio = ar;
		UpdateProjectionMatrix();
	}

	void Camera::SetNearFarPlanes(float n, float f) {
		nearDist = n;
		farDist = f;
		UpdateProjectionMatrix();
	}

	glm::mat4& Camera::GetView() {
		return viewMatrix;
	}

	glm::mat4& Camera::GetProjection() {
		return projectionMatrix;
	}

	glm::vec3& Camera::GetPosition() {
		return position;
	}

	//========================================================================
	//	Base Class for Camera controlling objects (like a player)
	//========================================================================

	CameraController::CameraController(Camera* camera, float sensitivity)
		: camera(camera), sensitivity(sensitivity), yaw(-90.f), pitch(0.f), distance(0.f) {
		camera->SetPosition({ 0, 0, 0 });
		camera->SetViewVectors({ 0, 0, -1 }, {1, 0, 0});
	}

	void CameraController::Update(float deltaTime) {
		float speed = 1.f * deltaTime;
		glm::vec3 velocity = glm::vec3(0);

		if (Keyboard::GetKey(KEY_W))
			velocity += camera->front;
		if (Keyboard::GetKey(KEY_S))
			velocity -= camera->front;
		if (Keyboard::GetKey(KEY_D))
			velocity += camera->right;
		if (Keyboard::GetKey(KEY_A))
			velocity -= camera->right;
		if (Keyboard::GetKey(KEY_SPACE))
			velocity += camera->up;
		if (Keyboard::GetKey(KEY_X))
			velocity -= camera->up;

		if (velocity.x != 0 || velocity.y!= 0 || velocity.z != 0)
			velocity = glm::normalize(velocity) * speed;

		camera->SetPosition(camera->position + velocity);
	}

	void CameraController::OnMouseScroll(float xOffset, float yOffset) {
		distance = zm::Clamp(distance - (yOffset * 10), 0.f, 100.f);
	}

	void CameraController::OnMouseMove(float x, float y, float dx, float dy) {
		yaw += dx * sensitivity;
		yaw = zm::WrapClamp(yaw, -180.f, 180.f);
		pitch -= dy * sensitivity;
		pitch = zm::Clamp(pitch, -89.5f, 89.5f);
		camera->SetYawPitch(yaw, pitch);
	}
}