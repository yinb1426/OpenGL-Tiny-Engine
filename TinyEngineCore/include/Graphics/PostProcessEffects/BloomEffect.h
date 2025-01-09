#pragma once
#include "Manager/ResourceManager.h"
#include "Graphics/PostProcessEffects/PostProcessEffect.h"

namespace TinyEngine
{
	class BloomEffect : public PostProcessEffect
	{
	public:
		BloomEffect() : threshold(0.5f), intensity(1.5f)
		{
			InitializeEffect();
			isEnabled = false;
		}

		~BloomEffect()
		{
			DeleteEffect();
		}

		void InitializeEffect()
		{
			InitializeQuad();
			for (int i = 0; i < 3; i++)
				framebuffers[i] = std::make_shared<Framebuffer>("Framebuffers" + std::to_string(i), SCREEN_WIDTH, SCREEN_HEIGHT, 1, false);
			brightnessShader = gResourceManager->GetShader("Bloom-Brightness Shader");
			blurShader = gResourceManager->GetShader("Bloom-Blur Shader");
			combineShader = gResourceManager->GetShader("Bloom-Combine Shader");
		}

		void ApplyEffect(std::shared_ptr<ScreenBuffer> screenBuffer)
		{
			for (int i = 0; i < 3; i++)
				framebuffers[i]->UpdateFramebuffer();

			framebuffers[2]->Bind();
			glClear(GL_COLOR_BUFFER_BIT);
			brightnessShader->Use();
			brightnessShader->SetUniform("threshold", threshold);
			brightnessShader->SetUniform("albedo", 0);
			screenBuffer->BindTexture(0);
			RenderQuad();
			brightnessShader->Unuse();
			framebuffers[2]->Unbind();

			bool horizontal = true, firstIteration = true;
			unsigned int amount = 10;
			blurShader->Use();
			blurShader->SetUniform("albedo", 0);
			for (unsigned int i = 0; i < amount; i++)
			{
				framebuffers[horizontal]->Bind();
				glClear(GL_COLOR_BUFFER_BIT);
				blurShader->SetUniform("horizontal", horizontal);
				RenderQuad();
				if (firstIteration)
					framebuffers[2]->BindTexture(0);
				else
					framebuffers[!horizontal]->BindTexture(0);
				RenderQuad();
				horizontal = !horizontal;
				if (firstIteration)
					firstIteration = false;
			}
			blurShader->Unuse();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			combineShader->Use();
			combineShader->SetUniform("intensity", intensity);
			combineShader->SetUniform("albedoScene", 0);
			combineShader->SetUniform("blurScene", 1);
			screenBuffer->BindTexture(0);
			framebuffers[!horizontal]->BindTexture(1);
			framebuffers[horizontal]->Bind();
			RenderQuad();
			combineShader->Unuse();
			framebuffers[horizontal]->BlitFramebuffer(screenBuffer.get());
			framebuffers[horizontal]->Unbind();
		}

	public:
		float threshold;
		float intensity;
	private:
		std::shared_ptr<Framebuffer> framebuffers[3];
		std::shared_ptr<Shader> brightnessShader;
		std::shared_ptr<Shader> blurShader;
		std::shared_ptr<Shader> combineShader;
	};
}