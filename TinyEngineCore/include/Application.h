#pragma once
#include <iostream>
#include "OpenGLContext.h"
#include "UIContext.h"
#include "ResourceManager.h"
#include "StateManager.h"
#include "Config.h"
#include "Common.h"
#include "SceneManager.h"
namespace TinyEngine 
{
	class Application
	{
	public:
		Application(unsigned int width, unsigned int height, const char* title)
		{
			glContext = std::make_unique<OpenGLContext>(width, height, title);
			resourceManager = std::make_unique<ResourceManager>();
			stateManager = std::make_unique<StateManager>();
			sceneManager = std::make_unique<SceneManager>();
			uiContext = std::make_unique<UIContext>(glContext->GetWindow());

			// Initialize Resources and Scene
			InitializeResourceManager();
			InitializeScene();
		}
		
		static Application& GetInstance() { return *sInstance; }

		void Loop()
		{
			while (!glContext->ShouldClose())
			{
				// state update
				stateManager->Enable(GL_DEPTH_TEST);

				// render
				stateManager->ClearPerFrame();
				sceneManager->Render(glContext->GetWindowAspect(), resourceManager->GetTextureMap());
				uiContext->Render(stateManager.get(), sceneManager->GetActiveSceneCamera());

				// Swap Buffer and Poll Event
				glContext->SwapBuffersAndPollEvents();
			}
		}

		void Destroy()
		{
			uiContext->DestroyUIContext();
			glContext->Shutdown();
		}
	private:
		void InitializeResourceManager()
		{
			resourceManager->AddShader("Triangle Shader", std::make_shared<Shader>(SHADER_PATH "/triangle.vert.glsl", SHADER_PATH "/triangle.frag.glsl"));
			resourceManager->AddShader("Rock Shader", std::make_shared<Shader>(SHADER_PATH "/rock.vert.glsl", SHADER_PATH "/rock.frag.glsl"));
			resourceManager->AddShader("Cube Shader2", std::make_shared<Shader>(SHADER_PATH "/cube2.vert.glsl", SHADER_PATH "/cube2.frag.glsl"));
			resourceManager->AddShader("Planet Shader", std::make_shared<Shader>(SHADER_PATH "/planet.vert.glsl", SHADER_PATH "/planet.frag.glsl"));
			resourceManager->AddTexture("Container", std::make_shared<Texture>(TEXTURE_PATH "/container.jpg"));
			resourceManager->AddTexture("Awesomeface", std::make_shared<Texture>(TEXTURE_PATH "/awesomeface.png"));
			resourceManager->AddModel("Rock", std::make_shared<Model>(MODEL_PATH "/rock/rock.obj"));
			resourceManager->AddModel("Planet", std::make_shared<Model>(MODEL_PATH "/planet/planet.obj"));
			resourceManager->AddMaterial("Rock Material", std::make_shared<Material>(MATERIAL_PATH "/rock.json", resourceManager->GetShader("Rock Shader")));
			resourceManager->AddMaterial("Planet Material", std::make_shared<Material>(MATERIAL_PATH "/planet.json", resourceManager->GetShader("Planet Shader")));
		}
		void InitializeScene()
		{
			std::shared_ptr<Scene> scene = std::make_shared<Scene>();

			Camera camera(glm::vec3(20.0f, 20.0f, 110.0f));
			camera.UpdateCameraYaw(-101.0f);
			camera.UpdateCameraPitch(-15.0f);
			camera.UpdateCameraFarPlane(200.0f);
			scene->SetCamera(camera);
			
			// planet
			std::shared_ptr<Material> planetMaterial = resourceManager->GetMaterial("Planet Material");
			std::shared_ptr<Model> planetModel = resourceManager->GetModel("Planet");
			scene->AddGameObject("planet", {
				planetModel,
				Transform(glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(0.0f), glm::vec3(6.0f, 6.0f, 6.0f)),
				planetMaterial
			});

			// rocks
			std::shared_ptr<Material> rockMaterial = resourceManager->GetMaterial("Rock Material");
			std::shared_ptr<Model> rockModel = resourceManager->GetModel("Rock");
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

			sceneManager->AddScene("Planet Scene", scene);
			sceneManager->SetActiveScene("Planet Scene");
		}
	private:
		static Application* sInstance;
		std::unique_ptr<OpenGLContext> glContext;
		std::unique_ptr<UIContext> uiContext;
		std::unique_ptr<ResourceManager> resourceManager;
		std::unique_ptr<StateManager> stateManager;
		std::unique_ptr<SceneManager> sceneManager;
	};
}