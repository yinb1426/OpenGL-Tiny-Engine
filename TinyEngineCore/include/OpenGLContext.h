#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

			// glfw window creation
			this->window = glfwCreateWindow(width, height, title, NULL, NULL);
			if (this->window == NULL)
			{
				glfwTerminate();
				throw std::runtime_error("Failed to create GLFW window");
			}
			glfwMakeContextCurrent(window);
			glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

			// glad: load all OpenGL function pointers
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
				throw std::runtime_error("Failed to initialize GLAD");
		}
		void MakeCurrent()
		{
			glfwMakeContextCurrent(this->window);
		}
		bool ShouldClose()
		{
			return glfwWindowShouldClose(this->window);
		}
		void SwapBuffersAndPollEvents()
		{
			glfwSwapBuffers(this->window);
			glfwPollEvents();
		}
		void Shutdown()
		{
			if (this->window != nullptr)
				glfwDestroyWindow(this->window);
			glfwTerminate();
			std::cout << "Context Destroyed!" << std::endl;
		}
		GLFWwindow* GetWindow() const
		{
			return this->window;
		}

	private:
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}
	private:
		unsigned int width;
		unsigned int height;
		GLFWwindow* window;
	};
}
