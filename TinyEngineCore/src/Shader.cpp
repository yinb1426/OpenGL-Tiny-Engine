#include "Shader.h"
#include <iostream>
#include <fstream>
#include <glad/glad.h>

namespace TinyEngine
{
	Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
	{
		std::string vertexSourceString = ShaderReader(vertexPath);
		std::string fragmentSourceString = ShaderReader(fragmentPath);
		const char* vertexSource = vertexSourceString.c_str();
		const char* fragmentSource = fragmentSourceString.c_str();
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		CheckCompileErrors(vertexShader, "VERTEX");
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		CheckCompileErrors(fragmentShader, "FRAGMENT");
		unsigned int geometryShader;
		if (geometryPath != NULL)
		{
			std::string geometrySourceString = ShaderReader(geometryPath);
			const char* geometrySource = geometrySourceString.c_str();
			geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometryShader, 1, &geometrySource, NULL);
			glCompileShader(geometryShader);
			CheckCompileErrors(geometryShader, "GEOMETRY");
		}

		this->ID = glCreateProgram();
		glAttachShader(this->ID, vertexShader);
		glAttachShader(this->ID, fragmentShader);
		if(geometryPath != NULL)
			glAttachShader(this->ID, geometryShader);
		glLinkProgram(this->ID);
		CheckCompileErrors(this->ID, "PROGRAM");
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometryPath != NULL)
			glDeleteShader(geometryShader);
	}

	Shader::~Shader()
	{
		std::cout << "Shader Destroyed: " << this->ID << std::endl;
		this->Destroy();
		this->ID = 0;
	}

	void Shader::Use()
	{
		glUseProgram(this->ID);
	}

	void Shader::Unuse()
	{
		glUseProgram(0);
	}
	void Shader::BindUniformBlock(const char* name, unsigned int bindingPoint) const
	{
		unsigned int blockIndex = glGetUniformBlockIndex(this->ID, name);
		glUniformBlockBinding(this->ID, blockIndex, bindingPoint);
	}
	void Shader::Destroy()
	{
		glDeleteProgram(this->ID);
	}

	void Shader::SetUniform(const char* name, bool value) const
	{
		glUniform1i(glGetUniformLocation(this->ID, name), (int)value);

	}
	void Shader::SetUniform(const char* name, int value) const
	{
		glUniform1i(glGetUniformLocation(this->ID, name), value);
	}
	void Shader::SetUniform(const char* name, float value) const
	{
		glUniform1f(glGetUniformLocation(this->ID, name), value);
	}
	void Shader::SetUniform(const char* name, const glm::vec2& vec) const
	{
		glUniform2fv(glGetUniformLocation(this->ID, name), 1, &vec[0]);
	}
	void Shader::SetUniform(const char* name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(this->ID, name), x, y);
	}
	void Shader::SetUniform(const char* name, const glm::vec3& vec) const
	{
		glUniform3fv(glGetUniformLocation(this->ID, name), 1, &vec[0]);
	}
	void Shader::SetUniform(const char* name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
	}
	void Shader::SetUniform(const char* name, const glm::vec4& vec) const
	{
		glUniform4fv(glGetUniformLocation(this->ID, name), 1, &vec[0]);
	}
	void Shader::SetUniform(const char* name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
	}
	void Shader::SetUniform(const char* name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
	}
	void Shader::SetUniform(const char* name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
	}
	void Shader::SetUniform(const char* name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
	}

	std::string Shader::ShaderReader(const char* path)
	{
		std::string content;
		std::ifstream fileStream(path, std::ios::in);
		std::string line;
		while (!fileStream.eof())
		{
			std::getline(fileStream, line);
			content.append(line + "\n");
		}
		fileStream.close();
		return content;
	}

	void Shader::CheckCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
}

