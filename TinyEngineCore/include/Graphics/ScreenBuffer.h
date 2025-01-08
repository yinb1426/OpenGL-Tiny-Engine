#pragma once
#include "Graphics/Framebuffer.h"

namespace TinyEngine
{
	class ScreenBuffer : public Framebuffer
	{
	public:
		ScreenBuffer(unsigned int width, unsigned int height)
			: Framebuffer("Screen Buffer", width, height, 1, true, GL_CLAMP_TO_EDGE, GL_NEAREST)
		{}

		~ScreenBuffer()
		{
			DeleteFramebuffer();
		}

		void RenderToScreen()
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, fbID);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		}

		unsigned int GetTexture(unsigned int index = 0) = delete;

		unsigned int GetScreenTexture()
		{
			return textureIDs[0];
		}


	};
}