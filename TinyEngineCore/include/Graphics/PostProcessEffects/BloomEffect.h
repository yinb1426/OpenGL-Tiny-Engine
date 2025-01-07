#pragma once
#include "Graphics/Framebuffer.h"
#include "Graphics/PostProcessEffects/PostProcessEffect.h"

namespace TinyEngine
{
	class BloomEffect : public PostProcessEffect
	{
	public:
		BloomEffect() : threshold(0.5f), intensity(1.5f)
		{
			InitializeEffect();
		}

		~BloomEffect()
		{
			brightnessFramebuffer->DeleteFramebuffer();
			DeleteEffect();
		}

		void InitializeEffect()
		{
			InitializeQuad();
			brightnessFramebuffer = std::make_shared<Framebuffer>("Brightness Framebuffer", 1200, 800, 1, false);
			brightnessShader = gResourceManager->GetShader("Bloom-Brightness Shader");
			blurShader = gResourceManager->GetShader("Bloom-Blur Shader");
			combineShader = gResourceManager->GetShader("Bloom-Combine Shader");
		}

		void ApplyEffect(std::shared_ptr<Framebuffer> curFramebuffer, std::shared_ptr<ScreenBuffer> screenBuffer)
		{
			brightnessFramebuffer->UpdateFramebuffer();
			curFramebuffer->UpdateFramebuffer();

			brightnessFramebuffer->Bind();
			glClear(GL_COLOR_BUFFER_BIT);
			brightnessShader->Use();
			brightnessShader->SetUniform("threshold", threshold);
			brightnessShader->SetUniform("albedo", 0);
			screenBuffer->BindTexture(0);
			RenderQuad();
			brightnessShader->Unuse();
			brightnessFramebuffer->Unbind();

			bool horizontal = true;
			unsigned int amount = 10;
			blurShader->Use();
			blurShader->SetUniform("albedo", 0);
			for (unsigned int i = 0; i < amount; i++)
			{
				blurShader->SetUniform("horizontal", horizontal);
				if (horizontal)
				{
					curFramebuffer->Bind();
					brightnessFramebuffer->BindTexture(0);
				}
				else
				{
					brightnessFramebuffer->Bind();
					curFramebuffer->BindTexture(0);
				}
				RenderQuad();
				horizontal = !horizontal;

			}
			blurShader->Unuse();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			combineShader->Use();
			combineShader->SetUniform("intensity", intensity);
			combineShader->SetUniform("albedoScene", 0);
			combineShader->SetUniform("blurScene", 1);
			screenBuffer->BindTexture(0);
			if (amount % 2)
			{
				curFramebuffer->BindTexture(1);
				brightnessFramebuffer->Bind();
			}
			else
			{
				brightnessFramebuffer->BindTexture(1);
				curFramebuffer->Bind();
			}
			RenderQuad();

			if (amount % 2)
				brightnessFramebuffer->BlitFramebuffer(screenBuffer.get());
			else
				curFramebuffer->BlitFramebuffer(screenBuffer.get());
		}

	public:
		float threshold;
		float intensity;
	private:
		std::shared_ptr<Framebuffer> brightnessFramebuffer;
		std::shared_ptr<Shader> brightnessShader;
		std::shared_ptr<Shader> blurShader;
		std::shared_ptr<Shader> combineShader;
	};
}