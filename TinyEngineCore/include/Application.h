#pragma once
#include "Context/OpenGLContext.h"
#include "Context/UIContext.h"
#include "Manager/ResourceManager.h"
#include "Manager/StateManager.h"
#include "Manager/SceneManager.h"
#include "Config.h"
#include "Graphics/Shader.h"
#include "Graphics/Material.h"
#include "Graphics/Texture.h"
#include "Graphics/ScreenBuffer.h"
#include "Geometry/Model.h"
#include <iostream>

namespace TinyEngine 
{
	std::unique_ptr<OpenGLContext> gGLContext;
	std::unique_ptr<UIContext> gUIContext;
	std::unique_ptr<ResourceManager> gResourceManager;
	std::unique_ptr<StateManager> gStateManager;
	std::unique_ptr<SceneManager> gSceneManager;

	class Application
	{
	public:
		Application(unsigned int width, unsigned int height, const char* title)
		{
			gGLContext = std::make_unique<OpenGLContext>(width, height, title);
			gResourceManager = std::make_unique<ResourceManager>(RESOURCES_JSON_PATH);
			gStateManager = std::make_unique<StateManager>();
			gSceneManager = std::make_unique<SceneManager>();
			gUIContext = std::make_unique<UIContext>();
			screenBuffer = std::make_unique<ScreenBuffer>(width, height);

			gSceneManager->SetActiveScene("Planet Scene");
		}
		
		static Application& GetInstance() { return *sInstance; }

		void Loop()
		{
			//framebuffer->DeleteFrameBuffer();
			//framebuffer = nullptr;
			//unsigned int windowWidth, windowHeight;
			//gGLContext->GetWindowWidthAndHeight(windowWidth, windowHeight);
			//framebuffer = std::make_unique<Framebuffer>("ScreenFramebuffer", windowWidth, windowHeight);
			while (!gGLContext->ShouldClose())
			{
				// state update
				gStateManager->Enable(GL_DEPTH_TEST);
				screenBuffer->UpdateFrameBuffer();

				// render
				// Tick -> TickLogic: 更新所有物体的信息; TickRender: 渲染所有物体
				gStateManager->ClearPerFrame();
				screenBuffer->Bind();
				TickLogic();
				TickRender();
				screenBuffer->Unbind();
				
				screenBuffer->RenderToScreen();

				// TODO: screenBuffer : framebuffer

				// Swap Buffer and Poll Event
				gGLContext->SwapBuffersAndPollEvents();
			}
		}

		void Destroy()
		{
			gUIContext->DestroyUIContext();
			gGLContext->Shutdown();
		}

	private:
		void TickLogic()
		{
			gUIContext->Tick(gSceneManager->GetActiveSceneCamera());
		}
		void TickRender()
		{
			gSceneManager->Render();
			gUIContext->Render();
		}
		void RenderQuad()
		{
			unsigned int quadVAO = 0, quadVBO = 0;
			if (quadVAO == 0)
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
			}
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
		}
	private:
		static Application* sInstance;
		std::unique_ptr<ScreenBuffer> screenBuffer;
	};
}