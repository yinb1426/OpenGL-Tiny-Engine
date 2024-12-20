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
#include "Graphics/PostProcessEffect.h"
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
			curFramebuffer = std::make_unique<Framebuffer>("Current Framebuffer", screenBuffer->GetWidth(), screenBuffer->GetHeight());

			gSceneManager->SetActiveScene("Planet Scene2");
		}
		
		static Application& GetInstance() { return *sInstance; }

		void Loop()
		{
			std::unique_ptr<TestEffect> effect = std::make_unique<TestEffect>();

			while (!gGLContext->ShouldClose())
			{
				// state update
				gStateManager->Enable(GL_DEPTH_TEST);
				screenBuffer->UpdateFramebuffer();

				// render
				// Tick -> TickLogic: 更新所有物体的信息; TickRender: 渲染所有物体
				gStateManager->ClearPerFrame();
				screenBuffer->Bind();
				TickLogic();
				TickRender();
				screenBuffer->Unbind();
				

				// Vintage测试
				//curFramebuffer->UpdateFramebuffer();
				//curFramebuffer->Bind();
				//glClear(GL_COLOR_BUFFER_BIT);
				//std::shared_ptr<Shader> vintageShader = gResourceManager->GetShader("Vintage Shader");
				//vintageShader->Use();
				//screenBuffer->BindTexture(0);
				//RenderQuad1();
				//vintageShader->Unuse();
				//curFramebuffer->Unbind();

				//curFramebuffer->BlitFramebuffer(screenBuffer.get());
				
				effect->ApplyEffect(curFramebuffer, screenBuffer);
				
				screenBuffer->RenderToScreen();

				RenderUI();

				// Swap Buffer and Poll Event
				gGLContext->SwapBuffersAndPollEvents();
			}
		}

	private:
		void TickLogic()
		{
			gUIContext->Tick(gSceneManager->GetActiveSceneCamera());
		}
		void TickRender()
		{
			gSceneManager->Render();
			
		}
		void RenderUI()
		{
			gUIContext->Render();
		}

		//void VintagePostProcess()
		//{
		//	std::unique_ptr<Framebuffer> curFramebuffer = std::make_unique<Framebuffer>("Vintage Framebuffer", screenBuffer->GetWidth(), screenBuffer->GetHeight());
		//	

		//	curFramebuffer->Bind();
		//	
		//	screenBuffer->BindTexture(0);
		//	RenderQuad();
		//	
		//	curFramebuffer->Unbind();

		//	curFramebuffer->BlitFramebuffer(screenBuffer.get());
		//	curFramebuffer->DeleteFramebuffer();
		//}


		void RenderQuad1()
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
		std::unique_ptr<Framebuffer> curFramebuffer;
	};
}