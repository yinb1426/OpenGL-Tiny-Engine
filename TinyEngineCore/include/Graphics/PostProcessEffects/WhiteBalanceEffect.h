#pragma once
#include "Manager/ResourceManager.h"
#include "Graphics/PostProcessEffects/PostProcessEffect.h"

namespace TinyEngine
{
	class WhiteBalanceEffect : public PostProcessEffect
	{
	public:
		WhiteBalanceEffect() : temperature(0.0f), tint(0.0f)
		{
			InitializeEffect();
			isEnabled = false;
		}
		~WhiteBalanceEffect()
		{
			DeleteEffect();
		}

		void InitializeEffect()
		{
			InitializeQuad();
			whiteBalanceShader = gResourceManager->GetShader("White Balance Shader");
		}

		void ApplyEffect(std::shared_ptr<Framebuffer> framebuffers[], std::shared_ptr<ScreenBuffer> screenBuffer)
		{
			framebuffers[0]->UpdateFramebuffer();

			framebuffers[0]->Bind();
			glClear(GL_COLOR_BUFFER_BIT);
			whiteBalanceShader->Use();
			whiteBalanceShader->SetUniform("temperature", temperature);
			whiteBalanceShader->SetUniform("tint", tint);
			screenBuffer->BindTexture(0);
			RenderQuad();
			whiteBalanceShader->Unuse();
			framebuffers[0]->Unbind();

			framebuffers[0]->BlitFramebuffer(screenBuffer.get());
		}
	public:
		float temperature; 
		float tint;
	private:
		std::shared_ptr<Shader> whiteBalanceShader;
	};
}