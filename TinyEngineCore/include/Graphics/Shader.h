#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace TinyEngine
{
	class Shader
	{
	public:
		Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = NULL);
		~Shader();
	public:
		void Use();
		void Destroy();
		void Unuse();
		void BindUniformBlock(const char* name, unsigned int bindingPoint) const;
		void SetUniform(const char* name, bool value) const;
		void SetUniform(const char* name, int value) const;
		void SetUniform(const char* name, float value) const;
		void SetUniform(const char* name, const glm::vec2& vec) const;
		void SetUniform(const char* name, float x, float y) const;
		void SetUniform(const char* name, const glm::vec3& vec) const;
		void SetUniform(const char* name, float x, float y, float z) const;
		void SetUniform(const char* name, const glm::vec4& vec) const;
		void SetUniform(const char* name, float x, float y, float z, float w) const;
		void SetUniform(const char* name, const glm::mat2& mat) const;
		void SetUniform(const char* name, const glm::mat3& mat) const;
		void SetUniform(const char* name, const glm::mat4& mat) const;
		void SetUniform(const char* name, const std::vector<float> vec) const;
	private:
		std::string ShaderReader(const char* path);
		void CheckCompileErrors(unsigned int shader, std::string type);
	private:
		unsigned int ID;
	};
}
