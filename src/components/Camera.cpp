#include "components/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float aspectRatio) {
	m_position = glm::vec3(0.0);
	m_rotation = 0;
	m_viewMatrix = glm::mat4(1.0);
	updateProjectionMatrix(aspectRatio);
}

void Camera::updateProjectionMatrix(float aspectRatio) {
	if (aspectRatio >= 1)
		m_projectionMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f, -1.f, 1.f);
	else
		m_projectionMatrix = glm::ortho(-1.f, 1.f, -1/aspectRatio, 1/aspectRatio, -1.f, 1.f);

	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void Camera::updateViewMatrix() {
	//m_viewMatrix = glm::lookAt(m_position, { 0.0, 0.0, -1.0 }, { 0.0, 1.0, 0.0 });
	glm::mat4 trans = glm::translate(glm::mat4(1.0), m_position) * glm::rotate(glm::mat4(1.0), glm::radians(m_rotation), {0.0, 0.0, 1.0});
	m_viewMatrix = glm::inverse(trans);
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}