#pragma once
#include "OpenGLWindow.h"
#include <iostream>
namespace TinyEngine
{
	class OpenGLContext
	{
	public:
		OpenGLContext(unsigned int width, unsigned int height, const char* title)
		{
			Initialize(width, height, title);
		}
		~OpenGLContext()
		{
			Shutdown();
		}
		void Initialize(unsigned int width, unsigned int height, const char* title)
		{
			// glfw: initialize and configure
			if (!glfwInit())
				throw std::runtime_error("Failed to initialize GLFW.");
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
			this->window = std::make_unique<OpenGLWindow>(width, height, title);
			
			glfwMakeContextCurrent(window->GetWindow());
			glfwSetFramebufferSizeCallback(window->GetWindow(), OnFramebufferSizeCallback);
			glfwSetScrollCallback(window->GetWindow(), OnScrollCallBack);

			// glad: load all OpenGL function pointers
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
				throw std::runtime_error("Failed to initialize GLAD");
		}
		void MakeCurrent()
		{
			glfwMakeContextCurrent(window->GetWindow());
		}
		bool ShouldClose()
		{
			return glfwWindowShouldClose(window->GetWindow());
		}
		void SwapBuffersAndPollEvents()
		{
			glfwSwapBuffers(window->GetWindow());
			glfwPollEvents();
		}
		void Shutdown()
		{
			if (window->GetWindow() != nullptr)
				glfwDestroyWindow(window->GetWindow());
			glfwTerminate();
			std::cout << "Context Destroyed!" << std::endl;
		}
		GLFWwindow* GetWindow()
		{
			return window->GetWindow();
		}
		float GetWindowAspect()
		{
			int width = 1, height = 1;
			glfwGetWindowSize(window->GetWindow(), &width, &height);
			return (float)width / (float)height;
		}
		void GetWindowWidthAndHeight(unsigned int& width, unsigned int& height)
		{
			int windowWidth = 0, windowHeight = 0;
			glfwGetWindowSize(window->GetWindow(), &windowWidth, &windowHeight);
			width = static_cast<unsigned int>(windowWidth);
			height = static_cast<unsigned int>(windowHeight);
		}
	private:
		static void OnFramebufferSizeCallback(GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}
		static void OnScrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
		{
			glfwGetWindowUserPointer(window);
			std::cout << yoffset << std::endl;
		}
	private:
		std::unique_ptr<OpenGLWindow> window;
	};

	extern std::unique_ptr<OpenGLContext> gGLContext;
}
