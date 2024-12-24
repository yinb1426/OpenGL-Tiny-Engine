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
            shader = gResourceManager->GetShader("Vignette Shader");
        }

        void ApplyEffect(std::unique_ptr<Framebuffer>& curFramebuffer, std::unique_ptr<ScreenBuffer>& screenBuffer)
        {
            curFramebuffer->UpdateFramebuffer();
            curFramebuffer->Bind();
            glClear(GL_COLOR_BUFFER_BIT);
            shader->Use();
            shader->SetUniform("vignetteColor", vignetteColor);
            shader->SetUniform("center", center);
            shader->SetUniform("intensity", intensity);
            shader->SetUniform("smoothness", smoothness);
            screenBuffer->BindTexture(0);
            RenderQuad();
            shader->Unuse();
            curFramebuffer->Unbind();

            curFramebuffer->BlitFramebuffer(screenBuffer.get());
        }
    public:
        glm::vec4 vignetteColor;
        glm::vec2 center;
        float intensity;
        float smoothness;
    };
}