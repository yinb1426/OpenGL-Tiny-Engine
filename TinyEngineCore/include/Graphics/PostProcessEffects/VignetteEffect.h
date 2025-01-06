#pragma once
#include "Graphics/PostProcessEffects/PostProcessEffect.h"

namespace TinyEngine
{
    class VignetteEffect : public PostProcessEffect
    {
    public:
        VignetteEffect() : vignetteColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), center(glm::vec2(0.5f, 0.5f)), intensity(0.45f), smoothness(0.3f)
        {
            InitializeEffect();
        }

        ~VignetteEffect()
        {
            DeleteEffect();
        }

        void InitializeEffect() override
        {
            InitializeQuad();
            vignetteShader = gResourceManager->GetShader("Vignette Shader");
        }

        void ApplyEffect(Framebuffer* curFramebuffer, ScreenBuffer* screenBuffer)
        {
            curFramebuffer->UpdateFramebuffer();
            curFramebuffer->Bind();
            glClear(GL_COLOR_BUFFER_BIT);
            vignetteShader->Use();
            vignetteShader->SetUniform("vignetteColor", vignetteColor);
            vignetteShader->SetUniform("center", center);
            vignetteShader->SetUniform("intensity", intensity);
            vignetteShader->SetUniform("smoothness", smoothness);
            screenBuffer->BindTexture(0);
            RenderQuad();
            vignetteShader->Unuse();
            curFramebuffer->Unbind();

            curFramebuffer->BlitFramebuffer(screenBuffer);
        }
    public:
        std::shared_ptr<Shader> vignetteShader;
        glm::vec4 vignetteColor;
        glm::vec2 center;
        float intensity;
        float smoothness;
    };
}