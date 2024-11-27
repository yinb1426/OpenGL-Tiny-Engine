#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "OpenGLContext.h"
#include "ResourceManager.h"
#include "StateManager.h"
#include "Config.h"
#include "Common.h"
#include "Scene.h"

namespace TinyEngine 
{
	class Application
	{
	public:
		Application(unsigned int width, unsigned int height, const char* title)
		{
			context = std::make_unique<OpenGLContext>(width, height, title);
			resourceManager = std::make_unique<ResourceManager>();
			stateManager = std::make_unique<StateManager>();
			scene = std::make_unique<Scene>();

			// Initialize Resources
			InitializeResourceManager();
			InitializeScene();

			// Test: Init imgui
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(context->GetWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 430 core");
		}
		
		static Application& GetInstance() { return *sInstance; }

		void Loop()
		{
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			while (!context->ShouldClose())
			{
				// input
				stateManager->Enable(GL_DEPTH_TEST);

				// render
				stateManager->ClearPerFrame(clear_color.x, clear_color.y, clear_color.z, 1.0f);
				scene->Render(context->GetWindowAspect(), resourceManager->GetTextureMap());
				
				// Test: Imgui window
				// Draw Ingui Window "Hello World"
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				static float f = 0.0f;
				ImGui::Begin("Hello World");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
				ImGui::SameLine();

				ImGui::End();

				// Render All windows
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				context->SwapBuffersAndPollEvents();
			}

			// Destroy Imgui
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
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

			//// rocks
			//std::shared_ptr<Material> rockMaterial = resourceManager->GetMaterial("Rock Material");
			//std::shared_ptr<Model> rockModel = resourceManager->GetModel("Rock");
			//unsigned int amount = 10000;
			//float radius = 50.0f;
			//float offset = 2.5f;
			//for (unsigned int i = 0; i < amount; i++)
			//{
			//	// translate
			//	float angle = (float)i / (float)amount * 360.0f;
			//	float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			//	float x = sin(angle) * radius + displacement;
			//	displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			//	float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
			//	displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			//	float z = cos(angle) * radius + displacement;
			//	glm::vec3 tv = glm::vec3(x, y, z);
			//	// scale
			//	float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
			//	glm::vec3 sv = glm::vec3(scale);
			//	// rotation
			//	glm::vec3 rv = glm::vec3(static_cast<float>(rand() % 360), static_cast<float>(rand() % 360), static_cast<float>(rand() % 360));	

			//	scene->AddGameObject("rock" + std::to_string(i), {
			//		rockModel,
			//		Transform(tv, rv, sv),
			//		rockMaterial
			//	});
			//}

		}
	private:
		static Application* sInstance;
		std::unique_ptr<OpenGLContext> context;
		std::unique_ptr<ResourceManager> resourceManager;
		std::unique_ptr<StateManager> stateManager;
		std::unique_ptr<Scene> scene;
	};
}