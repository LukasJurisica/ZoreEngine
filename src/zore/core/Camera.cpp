#include "zore/core/Camera.hpp"
#include "zore/math/MathUtils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace zore {

	//========================================================================
	//	Camera Base Class
	//========================================================================

	Camera::Camera(float aspectRatio, float nearDist, float farDist) :
		aspectRatio(aspectRatio), nearDist(nearDist), farDist(farDist),
		frustumWidth(0), frustumHeight(0), viewMatrix(0), projectionMatrix(0),
		position({ 0, 0, 0 }), forward({ 0, 0, -1 }), right({ 1, 0, 0 }) {

		up = glm::normalize(glm::cross(right, forward));
		front = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	}

	void Camera::UpdateViewMatrix() {
		viewMatrix = glm::lookAt(position, position + forward, up);
		UpdateFrustum();
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

	void Camera::SetViewVectors(const glm::vec3& f, const glm::vec3& r) {
		forward = f;
		right = r;
		up = glm::normalize(glm::cross(r, f));
		front = glm::normalize(glm::vec3(forward.x, 0, forward.z));
		UpdateViewMatrix();
	}

	void Camera::SetAspectRatio(float ar) {
		aspectRatio = ar;
		UpdateProjectionMatrix();
	}

	void Camera::SetNearFarPlanes(float _nearDist, float _farDist) {
		nearDist = _nearDist;
		farDist = _farDist;
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

	const float Camera::GetNearDist() const {
		return nearDist;
	}

	const float Camera::GetFarDist() const {
		return farDist;
	}

	//========================================================================
	//	Perspective Projection Camera
	//========================================================================

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearDist, float farDist)
		: Camera(aspectRatio, nearDist, farDist) {
		SetFOV(fov);
		UpdateViewMatrix();
	}

	void PerspectiveCamera::SetFOV(float _fov) {
		fov = glm::radians(_fov);
		UpdateProjectionMatrix();
	}

	bool PerspectiveCamera::TestPoint(const glm::vec3& point) const {
		glm::vec3 vector = point - position;
		for (int i = 0; i < 4; i++) {
			if (glm::dot(vector, frustumPlaneNormals[i]) < 0)
				return false;
		}
		return true;
	}

	bool PerspectiveCamera::TestAABB(const glm::vec3& min, const glm::vec3& size) const {
		for (int i = 0; i < 4; i++) {
			glm::vec3 vector = min; // vector = min?
			for (int j = 0; j < 3; j++) {
				if (frustumPlaneNormals[i][j] > 0)
					vector[j] += size[j];
			}
			if (glm::dot(vector - position, frustumPlaneNormals[i]) < 0)
				return false;
		}
		return true;
	}

	void PerspectiveCamera::UpdateProjectionMatrix() {
		projectionMatrix = glm::perspective(fov, aspectRatio, nearDist, farDist);
		frustumHeight = farDist * tan(fov * 0.5f);
		frustumWidth = frustumHeight * aspectRatio;
		UpdateFrustum();
	}

	void PerspectiveCamera::UpdateFrustum() {
		glm::vec3 lForward = forward * farDist;
		glm::vec3 lRight = right * frustumWidth;
		glm::vec3 lUp = up * frustumHeight;

		// Left Plane
		frustumPlaneNormals[0] = glm::cross((lForward - lRight), up);
		// Right Plane
		frustumPlaneNormals[1] = glm::cross(up, (lForward + lRight));
		// Top Plane
		frustumPlaneNormals[2] = glm::cross((lForward + lUp), right);
		// Bottom Plane
		frustumPlaneNormals[3] = glm::cross(right, (lForward - lUp));
	}

	//========================================================================
	//	Orthographic Projection Camera
	//========================================================================

	OrthographicCamera::OrthographicCamera(float height, float aspectRatio, float nearDist, float farDist)
		: Camera(aspectRatio, nearDist, farDist) {
		SetHeight(height);
		UpdateViewMatrix();
	}

	void OrthographicCamera::SetHeight(float height) {
		frustumHeight = height / 2.f;
		UpdateProjectionMatrix();
	}

	bool OrthographicCamera::TestPoint(const glm::vec3& point) const {
		return true;
	}

	bool OrthographicCamera::TestAABB(const glm::vec3& min, const glm::vec3& size) const {
		return true;
	}

	void OrthographicCamera::UpdateProjectionMatrix() {
		frustumWidth = frustumHeight * aspectRatio;
		projectionMatrix = glm::ortho(-frustumWidth, frustumWidth, -frustumHeight, frustumHeight, nearDist, farDist);
		UpdateFrustum();
	}

	void OrthographicCamera::UpdateFrustum() {

	}
}