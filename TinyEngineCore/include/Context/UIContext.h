#pragma once
#include "Graphics/Camera.h"
#include "Graphics/PostProcessVolume.h"
#include "Manager/StateManager.h"
#include <3rd/glad/glad.h>
#include <3rd/GLFW/glfw3.h>
#include <3rd/imgui/backends/imgui_impl_glfw.h>
#include <3rd/imgui/backends/imgui_impl_opengl3.h>
#include <3rd/imgui/imgui.h>

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
			if (ImGui::CollapsingHeader("Post Process Volume"))
			{
				if (ImGui::CollapsingHeader("Bloom Effect"))
				{
					ImGui::Checkbox("Enable Bloom Effect", &(volume->effects["BloomEffect"]->isEnabled));
					if (volume->effects["BloomEffect"]->isEnabled)
					{
						std::shared_ptr<BloomEffect> bloomEffect = std::dynamic_pointer_cast<BloomEffect>(volume->effects["BloomEffect"]);

						float bloomThreshold = bloomEffect->threshold;
						float bloomIntensity = bloomEffect->intensity;
						ImGui::DragFloat("Threshold", &bloomThreshold, 0.01f, 0.1f, 1.5f);
						ImGui::DragFloat("Bloom Intensity", &bloomIntensity, 0.01f, 0.1f, 3.0f);
						bloomEffect->threshold = bloomThreshold;
						bloomEffect->intensity = bloomIntensity;
					}
				}
				if (ImGui::CollapsingHeader("Vignette Effect"))
				{
					ImGui::Checkbox("Enable Vignette Effect", &(volume->effects["VignetteEffect"]->isEnabled));
					if (volume->effects["VignetteEffect"]->isEnabled)
					{
						std::shared_ptr<VignetteEffect> vignetteEffect = std::dynamic_pointer_cast<VignetteEffect>(volume->effects["VignetteEffect"]);

						glm::vec4 vignetteColor = vignetteEffect->vignetteColor;
						glm::vec2 vignetteCenter = vignetteEffect->center;
						float vignetteIntensity = vignetteEffect->intensity;
						float vignetteSmoothness = vignetteEffect->smoothness;

						ImGui::ColorEdit4("Color", (float*)&vignetteColor);
						ImGui::DragFloat2("Center", (float*)&vignetteCenter, 0.01f, 0.0f, 1.0f);
						ImGui::DragFloat("Vignette Intensity", &vignetteIntensity, 0.01f, 0.0f, 1.0f);
						ImGui::DragFloat("Smoothness", &vignetteSmoothness, 0.01f, 0.0f, 1.0f);

						vignetteEffect->vignetteColor = vignetteColor;
						vignetteEffect->center = vignetteCenter;
						vignetteEffect->intensity = vignetteIntensity;
						vignetteEffect->smoothness = vignetteSmoothness;
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