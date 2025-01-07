#pragma once
#include "Graphics/PostProcessEffects/VignetteEffect.h"
#include "Graphics/PostProcessEffects/BloomEffect.h"
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
			bool isEnabled;
		};

		std::unordered_map<std::string, std::shared_ptr<PostProcessEffectPair>> effects;

		PostProcessVolume()
		{
			effects["BloomEffect"] = std::make_shared<PostProcessEffectPair>(PostProcessEffectPair({ std::make_shared<BloomEffect>(), true }));
			effects["VignetteEffect"] = std::make_shared<PostProcessEffectPair>(PostProcessEffectPair({ std::make_shared<VignetteEffect>(), true }));
		}

		void SetEffectEnabled(std::string name, bool isEnabled)
		{
			effects[name]->isEnabled = isEnabled;
		}

		std::shared_ptr<PostProcessEffectPair> GetPostProcessEffect(std::string name)
		{
			return effects[name];
		}

		void ApplyEffects(std::shared_ptr<Framebuffer> curFramebuffer, std::shared_ptr<ScreenBuffer> screenBuffer)
		{
			for (auto& effect : effects)
			{
				if (effect.second->isEnabled)
					effect.second->effect->ApplyEffect(curFramebuffer, screenBuffer);
			}
		}
	};
}
