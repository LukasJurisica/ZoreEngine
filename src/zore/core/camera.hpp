#pragma once

#include "zore/math/vector/vec2.hpp"
#include "zore/math/matrix/mat4.hpp"

namespace zore {

	//========================================================================
	//	2D Camera Class
	//========================================================================

	class Camera2D {
	public:
		Camera2D(float aspect_ratio, float height);
		~Camera2D() = default;

		void Invert(bool x = false, bool y = true);
		void SetPosition(const zm::vec2& position);
		const zm::vec2& Translate(const zm::vec2& offset);
		const zm::vec2& TranslatePixels(const zm::vec2& offset);
		const zm::vec2& SetAspectRatio(float aspect_ratio);
		const zm::vec2& SetHeight(float height);

		bool TestPoint(zm::vec2 point) const;
		bool TestAABB(zm::vec2 min, const zm::vec2& size) const;

		const zm::vec2& GetPosition() const;
		const zm::vec2& GetScale() const;
		float GetHeight() const;
		zm::vec2 GetWorldSpaceTranslation(const zm::vec2& offset) const;
		zm::vec2 GetWorldSpacePosition(zm::vec2 coordinate) const;

	private:
		const zm::vec2& UpdateProjection();

	private:
		zm::vec2 m_multiplier;
		zm::vec2 m_position;
		zm::vec2 m_scale;
		float m_width, m_height;
		float m_aspect_ratio;
	};

	//========================================================================
	//	3D Camera Base Class
	//========================================================================

	class Camera3D {
	public:
		virtual ~Camera3D() = default;

		void SetPosition(const zm::vec3& position);
		void Translate(const zm::vec3& offset);
		void SetYawPitch(float yaw, float pitch);
		void SetViewVectors(const zm::vec3& forward, const zm::vec3& right);
		void SetAspectRatio(float aspect_ratio);
		void SetNearFarPlanes(float near_dist, float far_dist);

		virtual bool TestPoint(const zm::vec3& point) const = 0;
		virtual bool TestAABB(const zm::vec3& min, const zm::vec3& size) const = 0;

		const zm::mat4& GetProjection() const;
		const zm::mat4& GetView() const;
		const zm::vec3& GetPosition() const;
		const zm::vec3& GetFront() const;
		const zm::vec3& GetForward() const;
		const zm::vec3& GetRight() const;
		const zm::vec3& GetUp() const;

	protected:
		Camera3D(float aspect_ratio, float near_dist, float far_dist);
		void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix() = 0;
		virtual void UpdateFrustum() = 0;

	protected:
		float m_aspect_ratio;
		float m_near_dist, m_far_dist;

		zm::mat4 m_projection_matrix;
		zm::mat4 m_view_matrix;
		zm::vec3 m_position;
		zm::vec3 m_front;
		zm::vec3 m_forward;
		zm::vec3 m_right;
		zm::vec3 m_up;
	};

	//========================================================================
	//	Perspective Projection Camera
	//========================================================================

	class PerspectiveCamera : public Camera3D {
	public:
		PerspectiveCamera(float fov, float aspect_ratio, float near_dist, float far_dist);
		~PerspectiveCamera() = default;

		void SetFOV(float fov);
		bool TestPoint(const zm::vec3& point) const override;
		bool TestAABB(const zm::vec3& min, const zm::vec3& size) const override;

	private:
		void UpdateProjectionMatrix() override;
		void UpdateFrustum() override;

	private:
		float m_fov;
		zm::vec3 m_frustum_plane_normals[4];
	};

	//========================================================================
	//	Orthographic Projection Camera
	//========================================================================

	class OrthographicCamera : public Camera3D {
	public:
		OrthographicCamera(float aspect_ratio, float height, float near_dist, float far_dist);
		~OrthographicCamera() = default;

		void SetHeight(float height);
		bool TestPoint(const zm::vec3& point) const override;
		bool TestAABB(const zm::vec3& min, const zm::vec3& size) const override;

	private:
		void UpdateProjectionMatrix() override;
		void UpdateFrustum() override;

	private:
		float m_frustum_width, m_frustum_height;
	};
}