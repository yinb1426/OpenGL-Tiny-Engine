#pragma once
#include "Shader.h"
#include "Texture.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace TinyEngine
{
	using ShaderMap = std::unordered_map<std::string, std::shared_ptr<Shader>>;
	// using ModelMap = std::unordered_map<std::string, std::shared_ptr<Model>>;
	using TextureMap = std::unordered_map<std::string, std::shared_ptr<Texture>>;
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
	protected:
		ShaderMap shaders;
		// ModelMap models;
		TextureMap textures;
	};
}