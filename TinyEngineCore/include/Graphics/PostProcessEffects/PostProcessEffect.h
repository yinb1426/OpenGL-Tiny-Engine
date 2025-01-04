#pragma once
#include "Manager/ResourceManager.h"
#include "Geometry/VertexArray.h"
#include "Graphics/Shader.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/ScreenBuffer.h"
#include "Config.h"
#include <memory>
#include <vector>

namespace TinyEngine
{
	class PostProcessEffect
	{
	public:
		virtual void InitializeEffect() = 0;
		virtual void ApplyEffect(Framebuffer* curFramebuffer, ScreenBuffer* screenBuffer) = 0;
		void DeleteEffect()
		{
            glDeleteVertexArrays(1, &quadVAO);
            glDeleteBuffers(1, &quadVBO);
		}
	protected: 
		void InitializeQuad()
		{
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

            glBindVertexArray(0);
		}
        void RenderQuad()
        {
            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);
        }
	protected:
		std::shared_ptr<Shader> shader;
		unsigned int quadVAO, quadVBO;
	};
}
