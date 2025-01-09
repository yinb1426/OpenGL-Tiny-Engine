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
		void SetUniform()
		{
			shader->Use();
			if (paramsJson.contains("params"))
			{
				const auto& params = paramsJson["params"];
				for (auto& iterator : params.items())
				{
					auto& paramContent = iterator.value();
					std::string paramNameString = paramContent["name"].get<std::string>();
					const char* paramName = paramNameString.c_str();
					const Json& paramValue = paramContent["value"];

					std::vector<float> values;
					if (paramValue.is_array())
						values = paramValue.get<std::vector<float>>();
					else
						values = { paramValue.get<float>() };
					shader->SetUniform(paramName, values);
				}
			}
			shader->Unuse();
		}
		const std::shared_ptr<Shader> GetShader()
		{
			return this->shader;
		}

		const Json GetJson()
		{
			return this->paramsJson;
		}
	private:
		std::shared_ptr<Shader> shader;
		Json paramsJson;
	};
}