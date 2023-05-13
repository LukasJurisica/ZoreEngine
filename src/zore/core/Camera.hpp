#pragma once
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/matrix_float4x4.hpp"

namespace zore {

	//========================================================================
	//	Camera Base Class
	//========================================================================

	class Camera {
	public:
		virtual ~Camera() = default;

		void SetPosition(const glm::vec3& position);
		void Translate(glm::vec3& offset);
		void SetYawPitch(float yaw, float pitch);
		void SetViewVectors(const glm::vec3& forward, const glm::vec3& right);
		void SetAspectRatio(float aspectRatio);
		void SetNearFarPlanes(float nearDist, float farDist);

		virtual bool TestPoint(const glm::vec3& point) const = 0;
		virtual bool TestAABB(const glm::vec3& min, const glm::vec3& size) const = 0;

		const glm::mat4& GetProjection() const;
		const glm::mat4& GetView() const;
		const glm::vec3& GetPosition() const;
		const glm::vec3& GetFront() const;
		const glm::vec3& GetForward() const;
		const glm::vec3& GetRight() const;
		const glm::vec3& GetUp() const;
		const float GetNearDist() const;
		const float GetFarDist() const;

	protected:
		Camera(float aspectRatio, float nearDist, float farDist);
		void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix() = 0;
		virtual void UpdateFrustum() = 0;

	protected:
		float aspectRatio;
		float nearDist, farDist;
		float frustumWidth, frustumHeight;

		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
	};

	//========================================================================
	//	Perspective Projection Camera
	//========================================================================

	class PerspectiveCamera : public Camera {
	public:
		PerspectiveCamera(float fov, float aspectRatio, float nearDist, float farDist);
		~PerspectiveCamera() = default;

		void SetFOV(float fov);
		bool TestPoint(const glm::vec3& point) const override;
		bool TestAABB(const glm::vec3& min, const glm::vec3& size) const override;

	private:
		void UpdateProjectionMatrix() override;
		void UpdateFrustum() override;

	private:
		float fov;
		glm::vec3 frustumPlaneNormals[4];
	};

	//========================================================================
	//	Orthographic Projection Camera
	//========================================================================

	class OrthographicCamera : public Camera {
	public:
		OrthographicCamera(float aspectRatio, float height, float nearDist, float farDist);
		~OrthographicCamera() = default;

		void SetHeight(float height);
		bool TestPoint(const glm::vec3& point) const override;
		bool TestAABB(const glm::vec3& min, const glm::vec3& size) const override;

	private:
		void UpdateProjectionMatrix() override;
		void UpdateFrustum() override;
	};
}