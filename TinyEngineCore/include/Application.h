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

			postProcessVolume = std::make_shared<PostProcessVolume>();
		}
		
		static Application& GetInstance() { return *sInstance; }

		void Loop()
		{
			//std::unique_ptr<VignetteEffect> effect2 = std::make_unique<VignetteEffect>();

			while (!gGLContext->ShouldClose())
			{
				// state update
				gStateManager->Enable(GL_DEPTH_TEST);
				screenBuffer->UpdateFramebuffer();

				// Render Scene
				// Tick -> TickLogic: 更新所有物体的信息; TickRender: 渲染所有物体
				gStateManager->ClearPerFrame();
				screenBuffer->Bind();
				TickLogic();
				TickRender();
				screenBuffer->Unbind();
				
				// Post Process Effect
				postProcessVolume->ApplyEffects(curFramebuffer.get(), screenBuffer.get());
				
				screenBuffer->RenderToScreen();
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
		std::unique_ptr<ScreenBuffer> screenBuffer;
		std::unique_ptr<Framebuffer> curFramebuffer;
		std::shared_ptr<PostProcessVolume> postProcessVolume;
	};
}