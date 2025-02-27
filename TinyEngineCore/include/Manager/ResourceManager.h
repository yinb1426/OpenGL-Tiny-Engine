#pragma once
#include "Graphics/Shader.h"
#include "Graphics/Material.h"
#include "Geometry/Model.h"
#include "Graphics/Texture2D.h"
#include "Graphics/Cubemap.h"
#include "Config.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace TinyEngine
{
	using ShaderMap = std::unordered_map<std::string, std::shared_ptr<Shader>>;
	using MaterialMap = std::unordered_map<std::string, std::shared_ptr<Material>>;
	using ModelMap = std::unordered_map<std::string, std::shared_ptr<Model>>;
	using TextureMap = std::unordered_map<std::string, std::shared_ptr<Texture2D>>;
	using CubemapMap = std::unordered_map<std::string, std::shared_ptr<Cubemap>>;

	class ResourceManager
	{
	public:
		ResourceManager() {}
		ResourceManager(std::string jsonPath)
		{
			LoadResources(jsonPath);
		}
		~ResourceManager() {}
		void LoadResources(std::string jsonPath)
		{
			std::ifstream paramsFile(jsonPath, std::ios::in);
			Json resourcesJson = Json::parse(paramsFile);

			// Load Shaders
			const auto& shadersJson = resourcesJson["shaders"];
			for (auto& iterator : shadersJson.items())
			{
				auto& shaderJson = iterator.value();
				std::string shaderName = shaderJson["name"].get<std::string>();
				std::string shaderVSPathString = std::string(SHADER_ROOT_PATH).append(shaderJson["vs"].get<std::string>());
				std::string shaderFSPathString = std::string(SHADER_ROOT_PATH).append(shaderJson["fs"].get<std::string>());
				const char* shaderVSPath = shaderVSPathString.c_str();
				const char* shaderFSPath = shaderFSPathString.c_str();
				this->AddShader(shaderName, std::make_shared<Shader>(shaderVSPath, shaderFSPath));
			}

			// Load Textures
			const auto& texturesJson = resourcesJson["textures"];
			for (auto& iterator : texturesJson.items())
			{
				auto& textureJson = iterator.value();
				std::string textureName = textureJson["name"].get<std::string>();
				std::string texturePathString = std::string(TEXTURE_ROOT_PATH).append(textureJson["path"].get<std::string>());
				const char* texturePath = texturePathString.c_str();
				this->AddTexture(textureName, std::make_shared<Texture2D>(texturePath));
			}

			// Load Models
			const auto& modelsJson = resourcesJson["models"];
			for (auto& iterator : modelsJson.items())
			{
				auto& modelJson = iterator.value();
				std::string modelName = modelJson["name"].get<std::string>();
				std::string modelPathString = std::string(MODEL_ROOT_PATH).append(modelJson["path"].get<std::string>());
				const char* modelPath = modelPathString.c_str();
				this->AddModel(modelName, std::make_shared<Model>(modelPath));
			}

			// Load Materials
			const auto& materialsJson = resourcesJson["materials"];
			for (auto& iterator : materialsJson.items())
			{
				auto& materialJson = iterator.value();
				std::string materialName = materialJson["name"].get<std::string>();
				std::string materialPathString = std::string(MATERIAL_ROOT_PATH).append(materialJson["path"].get<std::string>());
				std::string materialShaderName = materialJson["shader"].get<std::string>();
				const char* materialPath = materialPathString.c_str();
				this->AddMaterial(materialName, std::make_shared<Material>(materialPath, this->GetShader(materialShaderName)));
			}

			// Load Cubemaps
			const auto& cubemapsJson = resourcesJson["cubemaps"];
			for (auto& iterator : cubemapsJson.items())
			{
				auto& cubemapJson = iterator.value();
				std::string cubemapName = cubemapJson["name"].get<std::string>();
				std::vector<std::string> cubemapFaces = cubemapJson["path"].get<std::vector<std::string>>();
				for(auto& cubemapFace : cubemapFaces)
					cubemapFace = std::string(CUBEMAP_ROOT_PATH).append(cubemapFace);
				this->AddCubemap(cubemapName, std::make_shared<Cubemap>(cubemapFaces));
			}
		}

		void AddShader(std::string name, std::shared_ptr<Shader>&& shader)
		{
			this->shaders[name] = std::move(shader);
		}
		const std::shared_ptr<Shader> GetShader(std::string name)
		{
			return this->shaders[name];
		}
		void AddTexture(std::string name, std::shared_ptr<Texture2D>&& texture)
		{
			this->textures[name] = std::move(texture);
		}
		const std::shared_ptr<Texture2D> GetTexture(std::string name)
		{
			return this->textures[name];
		}
		void AddModel(std::string name, std::shared_ptr<Model>&& model)
		{
			this->models[name] = std::move(model);
			for (auto& loadedTexture : this->models[name]->GetLoadedTextures())
			{
				const char* texturePath = loadedTexture.path.c_str();
				this->AddTexture(loadedTexture.name, std::make_shared<Texture2D>(texturePath));
			}
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
		void AddCubemap(std::string name, std::shared_ptr<Cubemap>&& cubemap)
		{
			this->cubemaps[name] = std::move(cubemap);
		}
		const std::shared_ptr<Cubemap> GetCubemap(std::string name)
		{
			return this->cubemaps[name];
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
		CubemapMap cubemaps;
	};

	extern std::unique_ptr<ResourceManager> gResourceManager;
}