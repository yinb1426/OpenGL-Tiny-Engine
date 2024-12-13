#pragma once
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

namespace TinyEngine
{
	using Json = nlohmann::json;
	using TextureMap = std::unordered_map<std::string, std::shared_ptr<Texture>>;
	class Material
	{
	public:
		Material(const char* jsonPath, std::shared_ptr<Shader> shader)
		{
			std::ifstream paramsFile(jsonPath, std::ios::in);
			this->paramsJson = Json::parse(paramsFile);
			this->name = this->paramsJson["name"].get<std::string>();
			// this->name = materialName.c_str();
			this->shader = shader;
		}
		~Material() {}
		void SetMatrices(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
		{
			shader->Use();
			shader->SetUniform("model", modelMatrix);
			shader->SetUniform("view", viewMatrix);
			shader->SetUniform("projection", projectionMatrix);
			shader->Unuse();
		}
		void SetUniform(TextureMap textureMap)
		{
			shader->Use();
			const auto& params = paramsJson["params"];
			const auto& textures = paramsJson["textures"];
			for (auto& iterator : params.items())
			{
				auto& paramContent = iterator.value();
				std::string paramNameString = paramContent["name"].get<std::string>();
				const char* paramName = paramNameString.c_str();
				std::string paramType = paramContent["type"].get<std::string>();
				const Json& paramValue = paramContent["value"];
				
				std::vector<float> values;
				if (paramValue.is_array())
					values = paramValue.get<std::vector<float>>();
				else
					values = { paramValue.get<float>() };
				shader->SetUniform(paramName, values);
			}
			int curIndex = 0;
			for (auto& iterator : textures.items())
			{
				auto& texturesContent = iterator.value();
				std::string texturesNameString = texturesContent["name"].get<std::string>();
				const char* texturesName = texturesNameString.c_str();
				std::string texturesPath = texturesContent["path"].get<std::string>();
				std::shared_ptr<Texture> curTexture = textureMap[texturesPath];
				shader->SetUniform(texturesName, curIndex);
				curTexture->Bind(curIndex++);
			}
		}
		const std::shared_ptr<Shader> GetShader()
		{
			return this->shader;
		}
	private:
		std::string name;
		std::shared_ptr<Shader> shader;
		Json paramsJson;
	};
}