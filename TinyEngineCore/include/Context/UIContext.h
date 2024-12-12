#pragma once
#include "Manager/StateManager.h"
#include "Graphics/Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

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

		void Render(std::shared_ptr<Camera> sceneCamera)
		{
			// Initialize ImGui
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Get data needed to be updated
			glm::vec4 curBackgroungColor = gStateManager->GetBackgroundColor();
			glm::vec3 curCamPos = sceneCamera->position;
			float curCamPitch = sceneCamera->pitch;
			float curCamYaw = sceneCamera->yaw;
			float curCamZNear = sceneCamera->zNear;
			float curCamZFar = sceneCamera->zFar;

			// Create UI
			ImGui::Begin("Scene Settings");
			if (ImGui::CollapsingHeader("Colors"))
			{
				ImGui::ColorEdit4("BackGroung Color", (float*)&curBackgroungColor);
			}
			if (ImGui::CollapsingHeader("Camera"))
			{
				ImGui::DragFloat3("Position", (float*)&curCamPos, 0.1f);
				ImGui::DragFloat("Pitch", &curCamPitch, 0.1f, -MAX_ABS_PITCH, MAX_ABS_PITCH);
				ImGui::DragFloat("Yaw", &curCamYaw, 0.1f);
				ImGui::DragFloat("Near Plane", &curCamZNear, 0.1f);
				ImGui::DragFloat("Far Plane", &curCamZFar, 0.1f);
			}
			ImGui::End();

			// Update data
			gStateManager->SetBackgroundColor(curBackgroungColor);
			sceneCamera->UpdateCameraParams(curCamPos, curCamPitch, curCamYaw, curCamZNear, curCamZFar);

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