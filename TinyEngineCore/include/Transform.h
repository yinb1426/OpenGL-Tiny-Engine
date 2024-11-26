#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace TinyEngine
{
	class Transform
	{
	public:
		Transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
			: position(position), rotation(rotation), scale(scale) {}
		const glm::mat4 GetModelMatrix()
		{
			//scale
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

			//rotate
			glm::mat4 rotationMatrix = glm::mat4_cast(glm::quat(glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
													  glm::quat(glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
													  glm::quat(glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)));
			
			//translate
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
			return translationMatrix * rotationMatrix * scaleMatrix;

		}
	public:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};
}