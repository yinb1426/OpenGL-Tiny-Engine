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

	class Camera
	{
	public:
		Camera(glm::vec3 position = DEFAULT_POSITION, glm::vec3 up = DEFAULT_UP, float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH) 
		{
			this->position = position;
			this->front = DEFAULT_FRONT;
			this->worldUp = up;
			this->yaw = yaw;
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
			this->pitch = pitch;
			this->zNear = DEFAULT_ZNEAR;
			this->zFar = DEFAULT_ZFAR;
			this->fov = DEFAULT_FOV;
			UpdateCameraVectors();
		}
		~Camera() {}
		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMtarix(const float aspect) const;
	private:
		void UpdateCameraVectors();
	private:
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 worldUp;
		float yaw; // º½Ïò½Ç(×óÓÒ)
		float pitch; // ¸©Ñö½Ç(ÉÏÏÂ)
		float zNear;
		float zFar;
		float fov;
	};
}