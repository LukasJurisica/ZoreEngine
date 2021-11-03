#pragma once
#include "devices/Mouse.hpp"
#include "devices/Keyboard.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/matrix_float4x4.hpp"

namespace zore {

	//========================================================================
	//	3D Camera Class
	//========================================================================

	class Camera {
		friend class CameraController;
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
		//void SetYawPitchRoll(float yaw, float pitch, float roll);
		void SetViewVectors(const glm::vec3& forward, const glm::vec3& right);
		void SetFOV(float fov);
		void SetAspectRatio(float aspectRatio);
		void SetNearFarPlanes(float nearDist, float farDist);

		const glm::mat4& GetView() const;
		const glm::mat4& GetProjection() const;
		const glm::vec3& GetPosition() const;
		const glm::vec3 GetViewPosition() const;

	private:
		float distance;
		float fov;
		float aspectRatio;
		float nearDist, farDist;
		float farWidth, farHeight;
		glm::vec3 planeNormals[4];

		glm::vec3 front;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;

		glm::vec3 position;
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
	};

	//========================================================================
	//	Base Class for Camera controlling objects (like a player)
	//========================================================================
	
	class CameraController : MouseListener, KeyListener {
	public:
		CameraController(Camera* camera, float sensitivity, const glm::vec3& position = glm::vec3(0, 0, 0));
		~CameraController() = default;

		void Update(float deltaTime);

	protected:
		void OnMouseScroll(float xOffset, float yOffset) override;
		void OnMouseMove(float x, float y, float dx, float dy) override;

	private:
		float yaw, pitch;
		float sensitivity;
		Camera* camera;
	};
}