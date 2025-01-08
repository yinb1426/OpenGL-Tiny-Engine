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
#include "Graphics/PostProcessVolume.h"
#include "Geometry/Model.h"
#include "Graphics/PostProcessVolume.h"
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
			postProcessVolume = std::make_shared<PostProcessVolume>();

			screenBuffer = std::make_shared<ScreenBuffer>(width, height);
			for (int i = 0; i < 4; i++)
				framebuffers[i] = std::make_shared<Framebuffer>("Framebuffers" + std::to_string(i), screenBuffer->GetWidth(), screenBuffer->GetWidth(), 1, false);
			
			gSceneManager->SetActiveScene("Planet Scene2");

			// postProcessVolume = std::make_shared<PostProcessVolume>();
		}
		
		static Application& GetInstance() { return *sInstance; }

		void Loop()
		{
			// Initialize Step
			postProcessVolume->InitializeEffect();

			while (!gGLContext->ShouldClose())
			{
				// state update
				gStateManager->Enable(GL_DEPTH_TEST);
				// screenBuffer->UpdateFrameBuffer();

				// Render Scene
				// Tick -> TickLogic: 更新所有物体的信息; TickRender: 渲染所有物体
				gStateManager->ClearPerFrame();
				screenBuffer->Bind();
				TickLogic();
				TickRender();
				screenBuffer->Unbind();

				// Post Process
				postProcessVolume->ApplyEffects(framebuffers, screenBuffer);

				screenBuffer->RenderToScreen();
				RenderUI();

				RenderUI();

				// Swap Buffer and Poll Event
				gGLContext->SwapBuffersAndPollEvents();
			}
		}

	private:
		void TickLogic()
		{
			gUIContext->Tick(gSceneManager->GetActiveSceneCamera(), postProcessVolume.get());
		}
		void TickRender()
		{
			gSceneManager->Render();
		}
		void RenderUI()
		{
			gUIContext->Render();
		}
	private:
		static Application* sInstance;
		std::shared_ptr<ScreenBuffer> screenBuffer;
		std::shared_ptr<Framebuffer> framebuffers[4];
		std::shared_ptr<PostProcessVolume> postProcessVolume;
	};
}