#pragma once
#include "Graphics/PostProcessEffects/VignetteEffect.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace TinyEngine
{
	struct PostProcessVolume
	{
		struct PostProcessEffectPair
		{
			std::shared_ptr<PostProcessEffect> effect;
			bool isUse;
		};

		std::unordered_map<std::string, std::shared_ptr<PostProcessEffectPair>> effects;

		PostProcessVolume()
		{
			PostProcessEffectPair effectPair = {
				std::make_shared<VignetteEffect>(), true
			};
			effects["VignetteEffect"] = std::make_shared<PostProcessEffectPair>(effectPair);
		}

		void SetEffectUse(std::string name, bool isUse)
		{
			effects[name]->isUse = isUse;
		}

		std::shared_ptr<PostProcessEffectPair> GetPostProcessEffect(std::string name)
		{
			return effects[name];
		}

		void ApplyEffects(std::unique_ptr<Framebuffer>& curFramebuffer, std::unique_ptr<ScreenBuffer>& screenBuffer)
		{
			for (auto& effect : effects)
			{
				if (effect.second->isUse)
					effect.second->effect->ApplyEffect(curFramebuffer, screenBuffer);
			}
		}
	};
}
