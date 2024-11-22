#include "Camera.h"

namespace TinyEngine
{
	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}

	glm::mat4 Camera::GetProjectionMtarix(const float aspect) const
	{
		return glm::perspective(glm::radians(this->fov), aspect, this->zNear, this->zFar);
	}

	void Camera::UpdateCameraVectors()
	{
		glm::vec3 tmpFront{};
		tmpFront.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		tmpFront.y = sin(glm::radians(this->pitch));
		tmpFront.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(tmpFront);
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}
}
