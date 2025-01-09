#pragma once
#include "Graphics/PostProcessEffects/VignetteEffect.h"
#include "Graphics/PostProcessEffects/BloomEffect.h"
#include "Graphics/PostProcessEffects/WhiteBalanceEffect.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace TinyEngine
{
	struct PostProcessVolume
	{
		std::unordered_map<std::string, std::shared_ptr<PostProcessEffect>> effects;
		PostProcessVolume(){ }

		void InitializeEffect()
		{
			effects["BloomEffect"] = std::make_shared<BloomEffect>();
			effects["WhiteBalanceEffect"] = std::make_shared<WhiteBalanceEffect>();
			effects["VignetteEffect"] = std::make_shared<VignetteEffect>();
		}

		void AddEffect(std::string name, std::shared_ptr<PostProcessEffect>&& effect)
		{
			effects[name] = effect;
		}

		void SetEffectEnabled(std::string name, bool isEnabled)
		{
			effects[name]->isEnabled = isEnabled;
		}

		std::shared_ptr<PostProcessEffect> GetPostProcessEffect(std::string name)
		{
			return effects[name];
		}

		void ApplyEffects(std::shared_ptr<ScreenBuffer> screenBuffer)
		{
			for (auto& effect : effects)
			{
				if (effect.second->isEnabled)
					effect.second->ApplyEffect(screenBuffer);
			}
		}
	};
}