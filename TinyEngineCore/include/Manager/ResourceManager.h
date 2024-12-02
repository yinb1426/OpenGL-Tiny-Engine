#pragma once
#include "Graphics/Shader.h"
#include "Graphics/Material.h"
#include "Geometry/Model.h"
#include "Graphics/Texture.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace TinyEngine
{
	using ShaderMap = std::unordered_map<std::string, std::shared_ptr<Shader>>;
	using MaterialMap = std::unordered_map<std::string, std::shared_ptr<Material>>;
	using ModelMap = std::unordered_map<std::string, std::shared_ptr<Model>>;
	using TextureMap = std::unordered_map<std::string, std::shared_ptr<Texture>>;

	class ResourceManager
	{
	public:
		ResourceManager() {}
		~ResourceManager() {}
		void AddShader(std::string name, std::shared_ptr<Shader>&& shader)
		{
			this->shaders[name] = std::move(shader);
		}
		const std::shared_ptr<Shader> GetShader(std::string name)
		{
			return this->shaders[name];
		}
		void AddTexture(std::string name, std::shared_ptr<Texture>&& texture)
		{
			this->textures[name] = std::move(texture);
		}
		const std::shared_ptr<Texture> GetTexture(std::string name)
		{
			return this->textures[name];
		}
		void AddModel(std::string name, std::shared_ptr<Model>&& model)
		{
			for (auto& loadedTexture : model->GetLoadedTextures())
			{
				const char* texturePath = loadedTexture.path.c_str();
				this->AddTexture(loadedTexture.name, std::make_shared<Texture>(texturePath));
			}
			this->models[name] = std::move(model);
		}
		const std::shared_ptr<Model> GetModel(std::string name)
		{
			return this->models[name];
		}
		void AddMaterial(std::string name, std::shared_ptr<Material>&& material)
		{
			this->materials[name] = std::move(material);
		}
		const std::shared_ptr<Material> GetMaterial(std::string name)
		{
			return this->materials[name];
		}
		const TextureMap GetTextureMap()
		{
			return this->textures;
		}
	protected:
		ShaderMap shaders;
		MaterialMap materials;
		ModelMap models;
		TextureMap textures;
	};

	extern std::unique_ptr<ResourceManager> gResourceManager;
}