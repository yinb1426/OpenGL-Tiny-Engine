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

			// Initialize Scene
			InitializeScene();
		}
		
		static Application& GetInstance() { return *sInstance; }

		void Loop()
		{
			while (!gGLContext->ShouldClose())
			{
				// state update
				gStateManager->Enable(GL_DEPTH_TEST);

				// render
				gStateManager->ClearPerFrame();
				gSceneManager->Render();
				gUIContext->Render(gSceneManager->GetActiveSceneCamera());

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
		void InitializeScene()
		{
			std::shared_ptr<Scene> scene = std::make_shared<Scene>();

			Camera camera(glm::vec3(20.0f, 20.0f, 110.0f));
			camera.UpdateCameraYaw(-101.0f);
			camera.UpdateCameraPitch(-15.0f);
			camera.UpdateCameraFarPlane(200.0f);
			scene->SetCamera(camera);
			
			// planet
			std::shared_ptr<Material> planetMaterial = gResourceManager->GetMaterial("Planet Material");
			std::shared_ptr<Model> planetModel = gResourceManager->GetModel("Planet");
			scene->AddGameObject("planet", {
				planetModel,
				Transform(glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(0.0f), glm::vec3(6.0f, 6.0f, 6.0f)),
				planetMaterial
			});

			// rocks
			std::shared_ptr<Material> rockMaterial = gResourceManager->GetMaterial("Rock Material");
			std::shared_ptr<Model> rockModel = gResourceManager->GetModel("Rock");
			unsigned int amount = 1000;
			float radius = 50.0f;
			float offset = 2.5f;
			for (unsigned int i = 0; i < amount; i++)
			{
				// translate
				float angle = (float)i / (float)amount * 360.0f;
				float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float x = sin(angle) * radius + displacement;
				displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
				displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float z = cos(angle) * radius + displacement;
				glm::vec3 tv = glm::vec3(x, y, z);
				// scale
				float scale = static_cast<float>((rand() % 20) / 25.0 + 0.05);
				glm::vec3 sv = glm::vec3(scale);
				// rotation
				glm::vec3 rv = glm::vec3(static_cast<float>(rand() % 360), static_cast<float>(rand() % 360), static_cast<float>(rand() % 360));	

				scene->AddGameObject("rock" + std::to_string(i), {
					rockModel,
					Transform(tv, rv, sv),
					rockMaterial
				});
			}

			gSceneManager->AddScene("Planet Scene", scene);
			gSceneManager->SetActiveScene("Planet Scene");
		}
	private:
		static Application* sInstance;
	};
}