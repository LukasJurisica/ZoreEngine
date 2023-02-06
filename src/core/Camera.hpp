#pragma once
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/matrix_float4x4.hpp"

namespace zore {

	//========================================================================
	//	3D Camera Class
	//========================================================================

	class Camera {
	public:
		Camera(float fov, float aspectRatio, float nearDist, float farDist);

		void UpdateViewMatrix();
		void UpdateProjectionMatrix();
		void UpdateFrustum();

		bool TestPoint(const glm::vec3& point) const;
		bool TestAABB(const glm::vec3& min, const glm::vec3& size) const;

		void SetPosition(const glm::vec3& position);
		void Translate(glm::vec3& offset);
		void SetYawPitch(float yaw, float pitch);
		void SetYawPitchRoll(float yaw, float pitch, float roll);
		void SetViewVectors(const glm::vec3& forward, const glm::vec3& right);
		void SetFOV(float fov);
		void SetAspectRatio(float aspectRatio);
		void SetNearFarPlanes(float nearDist, float farDist);

		const glm::mat4& GetProjection() const;
		const glm::mat4& GetView() const;
		const glm::vec3& GetPosition() const;
		const glm::vec3& GetFront() const;
		const glm::vec3& GetForward() const;
		const glm::vec3& GetRight() const;
		const glm::vec3& GetUp() const;
		const float GetNearDist() const;
		const float GetFarDist() const;
		
	private:
		float fov;
		float aspectRatio;
		float nearDist, farDist;
		float farWidth, farHeight;
		glm::vec3 planeNormals[4];

		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
	};
}