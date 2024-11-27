#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace TinyEngine
{
	class OpenGLWindow
	{
	public:
		OpenGLWindow(unsigned int width, unsigned int height, const char* title)
		{
			Initialize(width, height, title);
		}

		void Initialize(unsigned int width, unsigned int height, const char* title)
		{
			// glfw window creation
			this->window = glfwCreateWindow(width, height, title, NULL, NULL);
			if (this->window == NULL)
			{
				glfwTerminate();
				throw std::runtime_error("Failed to create GLFW window");
			}
		}
		GLFWwindow* GetWindow()
		{
			return this->window;
		}
		
	private:
		GLFWwindow* window;
	};
}