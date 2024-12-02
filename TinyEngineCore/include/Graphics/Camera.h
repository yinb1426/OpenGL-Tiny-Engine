#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace TinyEngine
{
	const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 DEFAULT_UP = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 DEFAULT_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);

	const float DEFAULT_YAW = -90.0f;
	const float DEFAULT_PITCH = 0.0f;
	const float DEFAULT_ZNEAR = 0.1f;
	const float DEFAULT_ZFAR = 100.0f;
	const float DEFAULT_FOV = 45.0f;
	const float MAX_ABS_PITCH = 89.5f;

	class Camera
	{
	public:
		Camera(glm::vec3 position = DEFAULT_POSITION, glm::vec3 up = DEFAULT_UP, float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH) 
		{
			this->position = position;
			this->front = DEFAULT_FRONT;
			this->worldUp = up;
			this->yaw = yaw;
			if (pitch > MAX_ABS_PITCH) pitch = MAX_ABS_PITCH;
			else if (pitch < -MAX_ABS_PITCH) pitch = -MAX_ABS_PITCH;
			this->pitch = pitch;
			this->zNear = DEFAULT_ZNEAR;
			this->zFar = DEFAULT_ZFAR;
			this->fov = DEFAULT_FOV;
			UpdateCameraVectors();
		}
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		{
			this->position = glm::vec3(posX, posY, posZ);
			this->front = DEFAULT_FRONT;
			this->worldUp = glm::vec3(upX, upY, upZ);
			this->yaw = yaw;
			if (pitch > MAX_ABS_PITCH) pitch = MAX_ABS_PITCH;
			else if (pitch < -MAX_ABS_PITCH) pitch = -MAX_ABS_PITCH;
			this->pitch = pitch;
			this->zNear = DEFAULT_ZNEAR;
			this->zFar = DEFAULT_ZFAR;
			this->fov = DEFAULT_FOV;
			UpdateCameraVectors();
		}
		~Camera() {}
		void UpdateCameraPosition(glm::vec3 newPos)
		{
			this->position = newPos;
			UpdateCameraVectors();
		}
		void UpdateCameraYaw(float newYaw)
		{
			this->yaw = newYaw;
			UpdateCameraVectors();
		}
		void UpdateCameraPitch(float newPitch)
		{
			if (newPitch > MAX_ABS_PITCH) newPitch = MAX_ABS_PITCH;
			else if (newPitch < -MAX_ABS_PITCH) newPitch = -MAX_ABS_PITCH;
			this->pitch = newPitch;
			UpdateCameraVectors();
		}
		void UpdateCameraNearPlane(float zNear)
		{
			this->zNear = zNear;
		}
		void UpdateCameraFarPlane(float zFar)
		{
			this->zFar = zFar;
		}
		glm::mat4 GetViewMatrix() const
		{
			return glm::lookAt(this->position, this->position + this->front, this->up);
		}
		glm::mat4 GetProjectionMtarix(const float aspect) const
		{
			return glm::perspective(glm::radians(this->fov), aspect, this->zNear, this->zFar);
		}
		void UpdateCameraParams(glm::vec3 position, float pitch, float yaw, float zNear, float zFar)
		{
			this->position = position;
			this->pitch = pitch;
			this->yaw = yaw;
			this->zNear = zNear;
			this->zFar = zFar;
			UpdateCameraVectors();
		}
	private:
		void UpdateCameraVectors()
		{
			glm::vec3 tmpFront{};
			tmpFront.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
			tmpFront.y = sin(glm::radians(this->pitch));
			tmpFront.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
			this->front = glm::normalize(tmpFront);
			this->right = glm::normalize(glm::cross(this->front, this->worldUp));
			this->up = glm::normalize(glm::cross(this->right, this->front));
		}
	public:
		glm::vec3 position;
		float yaw; // º½Ïò½Ç(×óÓÒ)
		float pitch; // ¸©Ñö½Ç(ÉÏÏÂ)
		float zNear;
		float zFar;
		float fov;
	private:
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 worldUp;
	};
}