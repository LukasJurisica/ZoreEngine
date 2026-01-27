#include "zore/core/camera.hpp"
#include "zore/devices/window.hpp"
#include "zore/math/math.hpp"

namespace zore {

	//========================================================================
	//	2D Camera Class
	//========================================================================

	Camera2D::Camera2D(float aspect_ratio, float height) : m_aspect_ratio(aspect_ratio), m_height(height), m_position(0, 0), m_multiplier(1, 1) {
		UpdateProjection();
	}

	void Camera2D::Invert(bool x, bool y) {
		m_multiplier = { x ? -1 : 1 , y ? -1 : 1 };
		UpdateProjection();
	}

	void Camera2D::SetPosition(const zm::vec2& position) {
		m_position = position;
	}

	const zm::vec2& Camera2D::Translate(const zm::vec2& offset) {
		return (m_position += offset);
	}

	const zm::vec2& Camera2D::TranslatePixels(const zm::vec2& offset) {
		return Translate(GetWorldSpaceTranslation(offset));
	}

	const zm::vec2& Camera2D::SetAspectRatio(float aspect_ratio) {
		m_aspect_ratio = aspect_ratio;
		return UpdateProjection();
	}

	const zm::vec2& Camera2D::SetHeight(float height) {
		m_height = height;
		return UpdateProjection();
	}

	bool Camera2D::TestPoint(zm::vec2 point) const {
		point -= m_position;
		return (point.x >= -m_width && point.x <= m_width && point.y >= -m_height && point.y <= m_height);
	}

	bool Camera2D::TestAABB(zm::vec2 min, const zm::vec2& size) const {
		min -= m_position;
		const zm::vec2 max = min + size;
		return (min.x <= m_width && -m_width <= max.x && min.y <= m_height && -m_height <= max.y);
	}

	const zm::vec2& Camera2D::GetPosition() const {
		return m_position;
	}

	const zm::vec2& Camera2D::GetScale() const {
		return m_scale;
	}
	
	float Camera2D::GetHeight() const {
		return m_height;
	}

	zm::vec2 Camera2D::GetWorldSpaceTranslation(const zm::vec2& offset) const {
		return (offset / static_cast<float>(Window::GetSize().y)) * m_height;
	}

	zm::vec2 Camera2D::GetWorldSpacePosition(zm::vec2 coordinate) const {
		coordinate -= zm::vec2(Window::GetSize()) / 2.f;
		return m_position + GetWorldSpaceTranslation({ coordinate.x, -coordinate.y });
	}

	const zm::vec2& Camera2D::UpdateProjection() {
		m_width = m_height * m_aspect_ratio;
		m_scale = { 2.f / m_width, 2.f / m_height };
		return (m_scale *= m_multiplier);
	}

	//========================================================================
	//	Camera3D Base Class
	//========================================================================

	Camera3D::Camera3D(float aspect_ratio, float near_dist, float far_dist) :
		m_aspect_ratio(aspect_ratio), m_near_dist(near_dist), m_far_dist(far_dist),
		m_view_matrix(0), m_projection_matrix(0), m_position(0, 0, 0), m_forward(0, 0, -1), m_right(1, 0, 0) {

		m_up = zm::Normalize(zm::Cross(m_right, m_forward));
		m_front = zm::Normalize(zm::vec3(m_forward.x, 0.f, m_forward.z));
	}

	void Camera3D::SetPosition(const zm::vec3& position) {
		m_position = position;
		UpdateViewMatrix();
	}

	void Camera3D::Translate(const zm::vec3& offset) {
		m_position += offset;
		UpdateViewMatrix();
	}

	void Camera3D::SetYawPitch(float yaw, float pitch) {
		yaw = zm::Radians(yaw);
		pitch = zm::Radians(pitch);

		float cos_yaw = cos(yaw);
		float sin_yaw = sin(yaw);
		float cos_pitch = cos(pitch);
		float sin_pitch = sin(pitch);

		m_front = zm::vec3(cos_yaw, 0.f, sin_yaw);
		m_forward = zm::vec3(cos_yaw * cos_pitch, sin_pitch, sin_yaw * cos_pitch);
		m_right = zm::Cross(m_front, { 0.f, 1.f, 0.f });
		m_up = zm::Normalize(zm::Cross(m_right, m_forward));
		UpdateViewMatrix();
	}

	void Camera3D::SetViewVectors(const zm::vec3& forward, const zm::vec3& right) {
		m_forward = forward;
		m_right = right;
		m_up = zm::Normalize(zm::Cross(right, forward));
		m_front = zm::Normalize(zm::vec3(m_forward.x, 0.f, m_forward.z));
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

	const zm::mat4& Camera3D::GetProjection() const {
		return m_projection_matrix;
	}

	const zm::mat4& Camera3D::GetView() const {
		return m_view_matrix;
	}

	const zm::vec3& Camera3D::GetPosition() const {
		return m_position;
	}

	const zm::vec3& Camera3D::GetFront() const {
		return m_front;
	}

	const zm::vec3& Camera3D::GetForward() const {
		return m_forward;
	}

	const zm::vec3& Camera3D::GetRight() const {
		return m_right;
	}

	const zm::vec3& Camera3D::GetUp() const {
		return m_up;
	}

	void Camera3D::UpdateViewMatrix() {
		m_view_matrix = zm::LookAt(m_position, m_position + m_forward, m_up);
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
		m_fov = zm::Radians(fov);
		UpdateProjectionMatrix();
	}

	bool PerspectiveCamera::TestPoint(const zm::vec3& point) const {
		zm::vec3 vector = point - m_position;
		for (int i = 0; i < 4; i++) {
			if (zm::Dot(vector, m_frustum_plane_normals[i]) < 0)
				return false;
		}
		return true;
	}

	bool PerspectiveCamera::TestAABB(const zm::vec3& min, const zm::vec3& size) const {
		for (const zm::vec3& frustum_plane : m_frustum_plane_normals) {
			zm::vec3 vector = min;
			for (int j = 0; j < 3; j++) {
				if (frustum_plane[j] > 0)
					vector[j] += size[j];
			}
			if (zm::Dot(vector - m_position, frustum_plane) < 0)
				return false;
		}
		return true;
	}

	void PerspectiveCamera::UpdateProjectionMatrix() {
		m_projection_matrix = zm::Perspective(m_fov, m_aspect_ratio, m_near_dist, m_far_dist);
		UpdateFrustum();
	}

	void PerspectiveCamera::UpdateFrustum() {
		float frustum_height = m_far_dist * tan(m_fov * 0.5f);
		float frustum_width = frustum_height * m_aspect_ratio;

		zm::vec3 l_forward = m_forward * m_far_dist;
		zm::vec3 l_right = m_right * frustum_width;
		zm::vec3 l_up = m_up * frustum_height;

		// Left Plane
		m_frustum_plane_normals[0] = zm::Cross((l_forward - l_right), m_up);
		// Right Plane
		m_frustum_plane_normals[1] = zm::Cross(m_up, (l_forward + l_right));
		// Top Plane
		m_frustum_plane_normals[2] = zm::Cross((l_forward + l_up), m_right);
		// Bottom Plane
		m_frustum_plane_normals[3] = zm::Cross(m_right, (l_forward - l_up));
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

	bool OrthographicCamera::TestPoint(const zm::vec3& point) const {
		return true;
	}

	bool OrthographicCamera::TestAABB(const zm::vec3& min, const zm::vec3& size) const {
		return true;
	}

	void OrthographicCamera::UpdateProjectionMatrix() {
		m_frustum_width = m_frustum_height * m_aspect_ratio;
		m_projection_matrix = zm::Orthographic(-m_frustum_width, m_frustum_width, -m_frustum_height, m_frustum_height, m_near_dist, m_far_dist);
		UpdateFrustum();
	}

	void OrthographicCamera::UpdateFrustum() {

	}
}