#pragma once
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/matrix_float4x4.hpp"

namespace zore {

	//========================================================================
	//	2D Camera Class
	//========================================================================

	class Camera2D {
	public:
		Camera2D(float aspect_ratio, float height);
		~Camera2D() = default;

		void Invert(bool x = false, bool y = true);
		void SetPosition(const glm::vec2& position);
		const glm::vec2& Translate(const glm::vec2& offset);
		const glm::vec2& TranslatePixels(const glm::vec2& offset);
		const glm::vec2& SetAspectRatio(float aspect_ratio);
		const glm::vec2& SetHeight(float height);

		bool TestPoint(glm::vec2 point) const;
		bool TestAABB(glm::vec2 min, const glm::vec2& size) const;

		const glm::vec2& GetPosition() const;
		const glm::vec2& GetScale() const;
		float GetHeight() const;
		glm::vec2 GetWorldSpaceTranslation(const glm::vec2& offset) const;
		glm::vec2 GetWorldSpacePosition(glm::vec2 coordinate) const;

	private:
		const glm::vec2& UpdateProjection();

	private:
		glm::vec2 m_multiplier;
		glm::vec2 m_position;
		glm::vec2 m_scale;
		float m_width, m_height;
		float m_aspect_ratio;
	};

	//========================================================================
	//	3D Camera Base Class
	//========================================================================

	class Camera3D {
	public:
		virtual ~Camera3D() = default;

		void SetPosition(const glm::vec3& position);
		void Translate(const glm::vec3& offset);
		void SetYawPitch(float yaw, float pitch);
		void SetViewVectors(const glm::vec3& forward, const glm::vec3& right);
		void SetAspectRatio(float aspect_ratio);
		void SetNearFarPlanes(float near_dist, float far_dist);

		virtual bool TestPoint(const glm::vec3& point) const = 0;
		virtual bool TestAABB(const glm::vec3& min, const glm::vec3& size) const = 0;

		const glm::mat4& GetProjection() const;
		const glm::mat4& GetView() const;
		const glm::vec3& GetPosition() const;
		const glm::vec3& GetFront() const;
		const glm::vec3& GetForward() const;
		const glm::vec3& GetRight() const;
		const glm::vec3& GetUp() const;

	protected:
		Camera3D(float aspect_ratio, float near_dist, float far_dist);
		void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix() = 0;
		virtual void UpdateFrustum() = 0;

	protected:
		float m_aspect_ratio;
		float m_near_dist, m_far_dist;

		glm::mat4 m_projection_matrix;
		glm::mat4 m_view_matrix;
		glm::vec3 m_position;
		glm::vec3 m_front;
		glm::vec3 m_forward;
		glm::vec3 m_right;
		glm::vec3 m_up;
	};

	//========================================================================
	//	Perspective Projection Camera
	//========================================================================

	class PerspectiveCamera : public Camera3D {
	public:
		PerspectiveCamera(float fov, float aspect_ratio, float near_dist, float far_dist);
		~PerspectiveCamera() = default;

		void SetFOV(float fov);
		bool TestPoint(const glm::vec3& point) const override;
		bool TestAABB(const glm::vec3& min, const glm::vec3& size) const override;

	private:
		void UpdateProjectionMatrix() override;
		void UpdateFrustum() override;

	private:
		float m_fov;
		glm::vec3 m_frustum_plane_normals[4];
	};

	//========================================================================
	//	Orthographic Projection Camera
	//========================================================================

	class OrthographicCamera : public Camera3D {
	public:
		OrthographicCamera(float aspect_ratio, float height, float near_dist, float far_dist);
		~OrthographicCamera() = default;

		void SetHeight(float height);
		bool TestPoint(const glm::vec3& point) const override;
		bool TestAABB(const glm::vec3& min, const glm::vec3& size) const override;

	private:
		void UpdateProjectionMatrix() override;
		void UpdateFrustum() override;

	private:
		float m_frustum_width, m_frustum_height;
	};
}