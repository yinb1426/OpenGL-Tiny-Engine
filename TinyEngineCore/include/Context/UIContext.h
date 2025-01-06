#pragma once
#include "Manager/StateManager.h"
#include "Graphics/Camera.h"
#include "Graphics/PostProcessVolume.h"
#include <3rd/glad/glad.h>
#include <3rd/GLFW/glfw3.h>
#include <3rd/imgui/imgui.h>
#include <3rd/imgui/backends/imgui_impl_glfw.h>
#include <3rd/imgui/backends/imgui_impl_opengl3.h>

namespace TinyEngine
{
	class UIContext
	{
	public:
		UIContext()
		{
			InitializeUIContext(gGLContext->GetWindow());
		}
		UIContext(GLFWwindow* window)
		{
			InitializeUIContext(window);
		}
		~UIContext()
		{
			DestroyUIContext();
		}
		void InitializeUIContext(GLFWwindow* window)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 430 core");
		}

		void Tick(std::shared_ptr<Camera> sceneCamera, PostProcessVolume* volume)
		{
			// Initialize ImGui
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Scene Settings
			ImGui::Begin("Scene Settings");
			if (ImGui::CollapsingHeader("Colors"))
			{
				glm::vec4 curBackgroungColor = gStateManager->GetBackgroundColor();
				ImGui::ColorEdit4("BackGroung Color", (float*)&curBackgroungColor);
				gStateManager->SetBackgroundColor(curBackgroungColor);
			}

			// Camera Part
			if (ImGui::CollapsingHeader("Camera"))
			{
				glm::vec3 curCamPos = sceneCamera->position;
				float curCamPitch = sceneCamera->pitch;
				float curCamYaw = sceneCamera->yaw;
				float curCamZNear = sceneCamera->zNear;
				float curCamZFar = sceneCamera->zFar;

				ImGui::DragFloat3("Position", (float*)&curCamPos, 0.1f);
				ImGui::DragFloat("Pitch", &curCamPitch, 0.1f, -MAX_ABS_PITCH, MAX_ABS_PITCH);
				ImGui::DragFloat("Yaw", &curCamYaw, 0.1f);
				ImGui::DragFloat("Near Plane", &curCamZNear, 0.1f);
				ImGui::DragFloat("Far Plane", &curCamZFar, 0.1f);

				sceneCamera->UpdateCameraParams(curCamPos, curCamPitch, curCamYaw, curCamZNear, curCamZFar);
			}

			// Post Process Volume Part
			if(ImGui::CollapsingHeader("Post Process Volume"))
			{
				if (ImGui::CollapsingHeader("Vignette Effect"))
				{
					static bool isVignetteEffectEnabled = volume->effects["VignetteEffect"]->isEnabled;
					if (ImGui::Checkbox("Enable Vignette Effect", &isVignetteEffectEnabled))
					{
						volume->effects["VignetteEffect"]->isEnabled = isVignetteEffectEnabled;
					}
						
					if (isVignetteEffectEnabled)
					{

						std::shared_ptr<VignetteEffect> effect = std::dynamic_pointer_cast<VignetteEffect>(volume->effects["VignetteEffect"]->effect);

						glm::vec4 curVignetteColor = effect->vignetteColor;
						glm::vec2 curCenter = effect->center;
						float curIntensity = effect->intensity;
						float curSmoothness = effect->smoothness;

						ImGui::ColorEdit4("Color", (float*)&curVignetteColor);
						ImGui::DragFloat2("Center", (float*)&curCenter, 0.01f, 0.0f, 1.0f);
						ImGui::DragFloat("Intensity", &curIntensity, 0.01f, 0.0f, 1.0f);
						ImGui::DragFloat("Smoothness", &curSmoothness, 0.01f, 0.0f, 1.0f);

						effect->vignetteColor = curVignetteColor;
						effect->center = curCenter;
						effect->intensity = curIntensity;
						effect->smoothness = curSmoothness;
					}
				}
			}

			ImGui::End();			
		}

		void Render()
		{
			// Render UI
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		void DestroyUIContext()
		{
			// Destroy Imgui
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
	};

	extern std::unique_ptr<UIContext> gUIContext;
}