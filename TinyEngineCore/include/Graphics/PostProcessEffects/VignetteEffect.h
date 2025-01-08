#pragma once
#include "Manager/ResourceManager.h"
#include "Graphics/PostProcessEffects/PostProcessEffect.h"

namespace TinyEngine
{
	class VignetteEffect : public PostProcessEffect
	{
	public:
		VignetteEffect() : vignetteColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), center(glm::vec2(0.5f, 0.5f)), intensity(0.45f), smoothness(0.3f)
		{
			InitializeEffect();
			isEnabled = false;
		}

		~VignetteEffect()
		{
			DeleteEffect();
		}

		void InitializeEffect()
		{
			InitializeQuad();
			vignetteShader = gResourceManager->GetShader("Vignette Shader");
		}

		void ApplyEffect(std::shared_ptr<Framebuffer> framebuffers[], std::shared_ptr<ScreenBuffer> screenBuffer)
		{
			framebuffers[0]->UpdateFramebuffer();
			screenBuffer->UpdateFramebuffer();

			framebuffers[0]->Bind();
			glClear(GL_COLOR_BUFFER_BIT);
			vignetteShader->Use();
			vignetteShader->SetUniform("vignetteColor", vignetteColor);
			vignetteShader->SetUniform("center", center);
			vignetteShader->SetUniform("intensity", intensity);
			vignetteShader->SetUniform("smoothness", smoothness);
			screenBuffer->BindTexture(0);
			RenderQuad();
			vignetteShader->Unuse();
			framebuffers[0]->Unbind();

			framebuffers[0]->BlitFramebuffer(screenBuffer.get());
		}
	public:
		glm::vec4 vignetteColor;
		glm::vec2 center;
		float intensity;
		float smoothness;
	private:
		std::shared_ptr<Shader> vignetteShader;
	};
}