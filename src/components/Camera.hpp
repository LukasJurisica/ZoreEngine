#pragma once

#include <glm/mat4x4.hpp>
//#include <glm/vec3.hpp>

class Camera {
public:
	Camera(float aspectRatio);

	void updateProjectionMatrix(float aspectRatio);

	//inline const glm::vec3& getPosition() const { return m_position; }
	//inline float getRotation() const { return m_rotation; }
	inline void setPosition(const glm::vec3& position) { m_position = position; updateViewMatrix(); }
	inline void setRotation(float rotation) { m_rotation = rotation; updateViewMatrix(); }
	//const glm::mat4& getViewMatrix() const { return m_viewMatrix; }
	//const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }
	const glm::mat4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

private:
	void updateViewMatrix();

	float m_rotation;
	glm::vec3 m_position;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewProjectionMatrix;
};