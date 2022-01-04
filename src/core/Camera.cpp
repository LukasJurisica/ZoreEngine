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

	bool Camera::TestPoint(const glm::vec3& point) const {
		glm::vec3 vector = point - position;
		for (int i = 0; i < 4; i++) {
			if (glm::dot(vector, planeNormals[i]) < 0)
				return false;
		}
		return true;
	}

	bool Camera::TestAABB(const glm::vec3& min, const glm::vec3& size) const {
		for (int i = 0; i < 4; i++) {
			glm::vec3 vector = min; // vector = min?
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
		position = pos;
		UpdateViewMatrix();
	}

	void Camera::Translate(glm::vec3& offset) {
		position += offset;
		UpdateViewMatrix();
	}

	void Camera::SetYawPitch(float yaw, float pitch) {
		yaw = glm::radians(yaw);
		pitch = glm::radians(pitch);

		float cosYaw = cos(yaw);
		float sinYaw = sin(yaw);
		float cosPit = cos(pitch);
		float sinPit = sin(pitch);

		front = glm::vec3(cosYaw, 0, sinYaw);
		forward = glm::vec3(cosYaw * cosPit, sinPit, sinYaw * cosPit);
		right = glm::cross(front, { 0.f, 1.f, 0.f });
		up = glm::normalize(glm::cross(right, forward));
		UpdateViewMatrix();
	}

	void Camera::SetYawPitchRoll(float yaw, float pitch, float roll) {

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

	const glm::mat4& Camera::GetProjection() const {
		return projectionMatrix;
	}

	const glm::mat4& Camera::GetView() const {
		return viewMatrix;
	}

	const glm::vec3& Camera::GetPosition() const {
		return position;
	}

	const glm::vec3& Camera::GetFront() const {
		return front;
	}

	const glm::vec3& Camera::GetForward() const {
		return forward;
	}

	const glm::vec3& Camera::GetRight() const {
		return right;
	}

	const glm::vec3& Camera::GetUp() const {
		return up;
	}
}