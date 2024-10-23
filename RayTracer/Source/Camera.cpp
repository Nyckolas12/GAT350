#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::SetView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
{
	m_view = glm::lookAt(eye, target, up);
}

void Camera::SetProjection(float fov, float aspect, float near, float far)
{
	m_projection = glm::perspective(glm::radians(fov), aspect, near, far);
}

/// <summary>
/// Converts point from world space to view space
/// </summary>
/// <param name="position">The given position in world space</param>
/// <returns>The given position in view (Camera) space</returns>
glm::vec3 Camera::ModelToView(const glm::vec3 position) const
{

	return m_view * glm::vec4{ position, 1 };
}

glm::vec3 Camera::ViewToProjection(const glm::vec3 position) const
{
	return m_projection * glm::vec4{ position, 1 };
}

glm::ivec2 Camera::ToScreen(const glm::vec3 position) const
{
	glm::vec4 clip = m_projection * glm::vec4{ position, 1 };
	glm::vec3 ndc = clip / clip.w;
	float x = (ndc.x + 1) * (m_width * 0.5f);
	float y = (ndc.y + 1) * (m_height * 0.5f);

	return glm::ivec2(x, y);
}
