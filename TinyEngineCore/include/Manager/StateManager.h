#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <unordered_map>
namespace TinyEngine
{
	struct OpenGLState
	{
		std::unordered_map<GLenum, bool> capabilities = {
			{ GL_DEPTH_TEST, false },
			{ GL_BLEND, false }
		};
		GLenum depthFunc = GL_LEQUAL;
		GLenum srcBlendFunc = GL_SRC_ALPHA, dstBlendFunc = GL_ONE_MINUS_SRC_ALPHA;
	};

	class StateManager
	{
	public:
		StateManager() { currentState = OpenGLState(); }
		~StateManager() { std::cout << "StateManager Destroyed!" << std::endl; }
		void ClearPerFrame()
		{
			glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		void Enable(GLenum capability)
		{
			if (this->currentState.capabilities[capability] == false)
				glEnable(capability);
			this->currentState.capabilities[capability] = true;
		}

		void Disable(GLenum capability)
		{
			if (this->currentState.capabilities[capability] == true)
				glDisable(capability);
			this->currentState.capabilities[capability] = false;
		}
		void SetDepthFunc(GLenum depthFunc)
		{
			if (this->currentState.depthFunc != depthFunc)
				glDepthFunc(depthFunc);
			this->currentState.depthFunc = depthFunc;
		}
		void SetBlendFunc(GLenum srcBlendFunc, GLenum dstBlendFunc)
		{
			if (this->currentState.srcBlendFunc != srcBlendFunc || this->currentState.dstBlendFunc != dstBlendFunc)
				glBlendFunc(srcBlendFunc, dstBlendFunc);
			this->currentState.srcBlendFunc = srcBlendFunc;
			this->currentState.dstBlendFunc = dstBlendFunc;
		}
		OpenGLState GetCurrentState() const
		{
			return this->currentState;
		}
		glm::vec4 GetBackgroundColor() const
		{
			return this->backgroundColor;
		}
		void SetBackgroundColor(glm::vec4 backgroundColor) 
		{
			this->backgroundColor = backgroundColor;
		}

		void Reset()
		{
			for (auto& capability : this->currentState.capabilities)
			{
				glDisable(capability.second);
			}
			this->currentState = OpenGLState();
		}

	private:
		OpenGLState currentState;
		glm::vec4 backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	};

	extern std::unique_ptr<StateManager> gStateManager;
}
