#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace TinyEngine
{
	class Framebuffer
	{
	public:
		Framebuffer(std::string name, unsigned int width, unsigned int height, unsigned int attachmentNum = 1, bool useRBODepth = true, unsigned int warpMode = GL_CLAMP_TO_EDGE, unsigned int filterMode = GL_NEAREST)
			: name(name)
		{
			CreateFrameBuffer(width, height, attachmentNum, useRBODepth, warpMode, filterMode);
		}

		void DeleteFrameBuffer()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			if (fbID)
			{
				glDeleteFramebuffers(1, &fbID);
				glDeleteTextures(attachmentNum, textureIDs);
				if (!depthID)  glDeleteTextures(1, &depthID);
				delete[] textureIDs;
				attachmentNum = 0;
				fbID = 0;
				depthID = 0;
			}
		}

		void UpdateFrameBuffer()
		{
			unsigned int windowWidth, windowHeight;
			gGLContext->GetWindowWidthAndHeight(windowWidth, windowHeight);

			if (windowWidth != width || windowHeight != height)
			{
				this->width = windowWidth;
				this->height = windowHeight;

				glBindFramebuffer(GL_FRAMEBUFFER, fbID);

				for (int i = 0; i < attachmentNum; i++)
				{
					glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
				}
				if (depthID)
				{
					glBindRenderbuffer(GL_RENDERBUFFER, depthID);
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
				}

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}

		void Bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbID);
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		unsigned int GetTexture(unsigned int index = 0)
		{
			if (index > attachmentNum - 1 || index < 0)
				throw std::invalid_argument("Framebuffer index: " + std::to_string(index) + "is out of range!");
			return textureIDs[index];
		}

		unsigned int GetID() const
		{
			return fbID;
		}

	protected:
		void CreateFrameBuffer(unsigned int width, unsigned int height, unsigned int attachmentNum, bool useRBODepth, unsigned int warpMode, unsigned int filterMode)
		{
			this->attachmentNum = attachmentNum;
			this->width = width;
			this->height = height;

			glGenFramebuffers(1, &fbID);
			glBindFramebuffer(GL_FRAMEBUFFER, fbID);
			textureIDs = new unsigned int[attachmentNum];
			memset(textureIDs, 0, sizeof(unsigned int) * attachmentNum);

			for (int i = 0; i < attachmentNum; i++)
			{
				glGenBuffers(1, &textureIDs[i]);
				glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureIDs[i], 0);
			}

			unsigned int* attachment = new unsigned int[attachmentNum];
			for (int i = 0; i < attachmentNum; i++)
				attachment[i] = GL_COLOR_ATTACHMENT0 + i;
			glDrawBuffers(attachmentNum, attachment);

			if (useRBODepth)
			{
				glGenRenderbuffers(1, &depthID);
				glBindRenderbuffer(GL_RENDERBUFFER, depthID);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthID);
			}

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				assert("Framebuffer not complete!");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

	protected:
		std::string name;
		unsigned int attachmentNum = 0;
		unsigned int fbID = 0;
		unsigned int depthID = 0;
		unsigned int* textureIDs;
		unsigned int width;
		unsigned int height;
	};
}