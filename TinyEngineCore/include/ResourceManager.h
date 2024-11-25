#pragma once
#include "Common.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace TinyEngine
{
	class ResourceManager
	{
	public:
		ResourceManager() {}
		~ResourceManager() {}
		void AddShader(std::string name, std::shared_ptr<Shader>& shader)
		{
			this->shaders[name] = std::move(shader);
		}
		const std::shared_ptr<Shader> GetShader(std::string name)
		{
			return this->shaders[name];
		}
		void AddTexture(std::string name, std::shared_ptr<Texture>& texture)
		{
			this->textures[name] = std::move(texture);
		}
		const std::shared_ptr<Texture> GetTexture(std::string name)
		{
			return this->textures[name];
		}
		const TextureMap GetTextureMap()
		{
			return this->textures;
		}
	protected:
		ShaderMap shaders;
		// ModelMap models;
		TextureMap textures;
	};
}