#pragma once

#include "Graphics/Cubemap.h"
#include "Graphics/Material.h"
#include <3rd/glm/glm.hpp>


namespace TinyEngine
{
	class Skybox
	{
	public:
		enum class SkyboxType
		{
			SOLID_COLOR,
			CUBEMAP
		};
		Skybox() : type(SkyboxType::SOLID_COLOR) {}
		Skybox(SkyboxType type) : type(type)
		{
			InitializeSkybox();
		}
		void SetSkyboxType(const SkyboxType type)
		{
			this->type = type;
		}
		void SetSkyboxColor(const glm::vec4 color)
		{
			if(type == SkyboxType::SOLID_COLOR)
				this->bgColor = color;
		}
		void SetSkyboxCubemap(const std::shared_ptr<Cubemap> cubemap)
		{
			if (type == SkyboxType::CUBEMAP)
				this->cubemap = cubemap;
		}
		~Skybox() {}
		void DrawSkybox(const Camera camera, const std::shared_ptr<Shader> skyboxShader)
		{
			if (type == SkyboxType::CUBEMAP)
			{
				glDepthFunc(GL_LEQUAL);
				skyboxShader->Use();
				skyboxShader->SetUniform("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
				skyboxShader->SetUniform("projection", camera.GetProjectionMatrix(gGLContext->GetWindowAspect()));
				skyboxShader->SetUniform("skybox", 0);
				glBindVertexArray(skyboxVAO);
				cubemap->Bind(0);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindVertexArray(0);
				glDepthFunc(GL_LESS);
			}
		}
	private:
		void InitializeSkybox()
		{
			float skyboxVertices[] = {
				// positions          
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f
			};
			glGenVertexArrays(1, &skyboxVAO);
			glGenBuffers(1, &skyboxVBO);
			glBindVertexArray(skyboxVAO);
			glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			bgColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
	private:
		SkyboxType type;
		unsigned int skyboxVAO, skyboxVBO;
		glm::vec4 bgColor;
		std::shared_ptr<Cubemap> cubemap;
	};
}