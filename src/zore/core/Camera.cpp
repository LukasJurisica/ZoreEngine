#include "zore/core/Camera.hpp"
#include "zore/devices/Window.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace zore {

	//========================================================================
	//	2D Camera Class
	//========================================================================

	Camera2D::Camera2D(float aspect_ratio, float height) : m_aspect_ratio(aspect_ratio), m_position(0, 0) {
		SetHeight(height);
	}

	void Camera2D::SetPosition(const glm::vec2& position) {
		m_position = position;
	}

	const glm::vec2& Camera2D::Translate(const glm::vec2& offset) {
		m_position += offset;
		return m_position;
	}

	const glm::vec2& Camera2D::TranslatePixels(const glm::vec2& offset) {
		m_position += GetWorldSpaceTranslation(offset);
		return m_position;
	}

	const glm::vec2& Camera2D::SetAspectRatio(float aspect_ratio) {
		m_aspect_ratio = aspect_ratio;
		UpdateProjection();
		return m_scale;
	}

	const glm::vec2& Camera2D::SetHeight(float height) {
		m_height = height;
		UpdateProjection();
		return m_scale;
	}

	bool Camera2D::TestPoint(glm::vec2 point) const {
		point -= m_position;
		return (point.x >= -m_width && point.x <= m_width && point.y >= -m_height && point.y <= m_height);
	}

	bool Camera2D::TestAABB(glm::vec2 min, const glm::vec2& size) const {
		min -= m_position;
		const glm::vec2 max = min + size;
		return (min.x <= m_width && -m_width <= max.x && min.y <= m_height && -m_height <= max.y);
	}

	const glm::vec2& Camera2D::GetPosition() const {
		return m_position;
	}

	const glm::vec2& Camera2D::GetScale() const {
		return m_scale;
	}

	glm::vec2 Camera2D::GetWorldSpaceTranslation(const glm::vec2& offset) const {
		return (offset / static_cast<float>(Window::GetSize().y)) * m_height;
	}

	glm::vec2 Camera2D::GetWorldSpacePosition(glm::vec2 coordinate) const {
		coordinate -= glm::vec2(Window::GetSize()) / 2.f;
		return m_position + GetWorldSpaceTranslation({ coordinate.x, -coordinate.y });
	}

	void Camera2D::UpdateProjection() {
		m_width = m_height * m_aspect_ratio;
		m_scale = { 2.f / m_width, 2.f / m_height };
	}

	//========================================================================
	//	Camera3D Base Class
	//========================================================================

	Camera3D::Camera3D(float aspect_ratio, float near_dist, float far_dist) :
		m_aspect_ratio(aspect_ratio), m_near_dist(near_dist), m_far_dist(far_dist),
		m_view_matrix(0), m_projection_matrix(0), m_position(0, 0, 0), m_forward(0, 0, -1), m_right(1, 0, 0) {

		m_up = glm::normalize(glm::cross(m_right, m_forward));
		m_front = glm::normalize(glm::vec3(m_forward.x, 0, m_forward.z));
	}

	void Camera3D::SetPosition(const glm::vec3& position) {
		m_position = position;
		UpdateViewMatrix();
	}

	void Camera3D::Translate(const glm::vec3& offset) {
		m_position += offset;
		UpdateViewMatrix();
	}

	void Camera3D::SetYawPitch(float yaw, float pitch) {
		yaw = glm::radians(yaw);
		pitch = glm::radians(pitch);

		float cos_yaw = cos(yaw);
		float sin_yaw = sin(yaw);
		float cos_pitch = cos(pitch);
		float sin_pitch = sin(pitch);

		m_front = glm::vec3(cos_yaw, 0, sin_yaw);
		m_forward = glm::vec3(cos_yaw * cos_pitch, sin_pitch, sin_yaw * cos_pitch);
		m_right = glm::cross(m_front, { 0.f, 1.f, 0.f });
		m_up = glm::normalize(glm::cross(m_right, m_forward));
		UpdateViewMatrix();
	}

	void Camera3D::SetViewVectors(const glm::vec3& forward, const glm::vec3& right) {
		m_forward = forward;
		m_right = right;
		m_up = glm::normalize(glm::cross(right, forward));
		m_front = glm::normalize(glm::vec3(m_forward.x, 0, m_forward.z));
		UpdateViewMatrix();
	}

	void Camera3D::SetAspectRatio(float aspect_ratio) {
		m_aspect_ratio = aspect_ratio;
		UpdateProjectionMatrix();
	}

	void Camera3D::SetNearFarPlanes(float near_dist, float far_dist) {
		m_near_dist = near_dist;
		m_far_dist = far_dist;
		UpdateProjectionMatrix();
	}

	const glm::mat4& Camera3D::GetProjection() const {
		return m_projection_matrix;
	}

	const glm::mat4& Camera3D::GetView() const {
		return m_view_matrix;
	}

	const glm::vec3& Camera3D::GetPosition() const {
		return m_position;
	}

	const glm::vec3& Camera3D::GetFront() const {
		return m_front;
	}

	const glm::vec3& Camera3D::GetForward() const {
		return m_forward;
	}

	const glm::vec3& Camera3D::GetRight() const {
		return m_right;
	}

	const glm::vec3& Camera3D::GetUp() const {
		return m_up;
	}

	void Camera3D::UpdateViewMatrix() {
		m_view_matrix = glm::lookAt(m_position, m_position + m_forward, m_up);
		UpdateFrustum();
	}

	//========================================================================
	//	Perspective Projection Camera
	//========================================================================

	PerspectiveCamera::PerspectiveCamera(float fov, float aspect_ratio, float near_dist, float far_dist)
		: Camera3D(aspect_ratio, near_dist, far_dist) {
		SetFOV(fov);
		UpdateViewMatrix();
	}

	void PerspectiveCamera::SetFOV(float fov) {
		m_fov = glm::radians(fov);
		UpdateProjectionMatrix();
	}

	bool PerspectiveCamera::TestPoint(const glm::vec3& point) const {
		glm::vec3 vector = point - m_position;
		for (int i = 0; i < 4; i++) {
			if (glm::dot(vector, m_frustum_plane_normals[i]) < 0)
				return false;
		}
		return true;
	}

	bool PerspectiveCamera::TestAABB(const glm::vec3& min, const glm::vec3& size) const {
		for (const glm::vec3& frustum_plane : m_frustum_plane_normals) {
			glm::vec3 vector = min;
			for (int j = 0; j < 3; j++) {
				if (frustum_plane[j] > 0)
					vector[j] += size[j];
			}
			if (glm::dot(vector - m_position, frustum_plane) < 0)
				return false;
		}
		return true;
	}

	void PerspectiveCamera::UpdateProjectionMatrix() {
		m_projection_matrix = glm::perspective(m_fov, m_aspect_ratio, m_near_dist, m_far_dist);
		UpdateFrustum();
	}

	void PerspectiveCamera::UpdateFrustum() {
		float frustum_height = m_far_dist * tan(m_fov * 0.5f);
		float frustum_width = frustum_height * m_aspect_ratio;

		glm::vec3 l_forward = m_forward * m_far_dist;
		glm::vec3 l_right = m_right * frustum_width;
		glm::vec3 l_up = m_up * frustum_height;

		// Left Plane
		m_frustum_plane_normals[0] = glm::cross((l_forward - l_right), m_up);
		// Right Plane
		m_frustum_plane_normals[1] = glm::cross(m_up, (l_forward + l_right));
		// Top Plane
		m_frustum_plane_normals[2] = glm::cross((l_forward + l_up), m_right);
		// Bottom Plane
		m_frustum_plane_normals[3] = glm::cross(m_right, (l_forward - l_up));
	}

	//========================================================================
	//	Orthographic Projection Camera
	//========================================================================

	OrthographicCamera::OrthographicCamera(float height, float aspect_ratio, float near_dist, float far_dist)
		: Camera3D(aspect_ratio, near_dist, far_dist) {
		SetHeight(height);
		UpdateViewMatrix();
	}

	void OrthographicCamera::SetHeight(float height) {
		m_frustum_height = height * 0.5f;
		UpdateProjectionMatrix();
	}

	bool OrthographicCamera::TestPoint(const glm::vec3& point) const {
		return true;
	}

	bool OrthographicCamera::TestAABB(const glm::vec3& min, const glm::vec3& size) const {
		return true;
	}

	void OrthographicCamera::UpdateProjectionMatrix() {
		m_frustum_width = m_frustum_height * m_aspect_ratio;
		m_projection_matrix = glm::ortho(-m_frustum_width, m_frustum_width, -m_frustum_height, m_frustum_height, m_near_dist, m_far_dist);
		UpdateFrustum();
	}

	void OrthographicCamera::UpdateFrustum() {

	}
}