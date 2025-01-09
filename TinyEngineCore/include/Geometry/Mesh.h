#pragma once
#include "VertexArray.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace TinyEngine
{
	using TextureMap = std::unordered_map<std::string, std::shared_ptr<Texture>>;

	struct MeshTexture
	{
		std::string name;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures) : 
			vertices(vertices), indices(indices), textures(textures), vertexArray(VertexArray(vertices, indices))
		{}

		void Draw(std::shared_ptr<Material> material, TextureMap textureMap)
		{
			std::shared_ptr<Shader> shader = material->GetShader();
			shader->Use();

			// bind appropriate textures
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			int index = 0;
			for (index = 0; index < textures.size(); index++)
			{
				std::string number;
				std::string type = textures[index].type;
				if (type == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (type == "texture_specular")
					number = std::to_string(specularNr++); // transfer unsigned int to string
				else if (type == "texture_normal")
					number = std::to_string(normalNr++); // transfer unsigned int to string
				else if (type == "texture_height")
					number = std::to_string(heightNr++); // transfer unsigned int to string

				shader->SetUniform((type + number).c_str(), index);
				textureMap[textures[index].name]->Bind(index);
			}
			const auto& paramsJson = material->GetJson();
			if (paramsJson.contains("textures"))
			{
				const auto& textures = paramsJson["textures"];
				for (auto& iterator : textures.items())
				{
					auto& texturesContent = iterator.value();
					std::string texturesNameString = texturesContent["name"].get<std::string>();
					const char* texturesName = texturesNameString.c_str();
					std::string texturesPath = texturesContent["path"].get<std::string>();
					std::shared_ptr<Texture> curTexture = textureMap[texturesPath];
					shader->SetUniform(texturesName, index);
					curTexture->Bind(index);
					index++;
				}
			}
			vertexArray.Draw();
			shader->Unuse();
		}

		void DrawInstanced(std::shared_ptr<Material> material, TextureMap textureMap, std::vector<glm::mat4> modelMatrices)
		{
			std::shared_ptr<Shader> shader = material->GetShader();
			shader->Use();

			// bind appropriate textures
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			int index = 0;
			for (index = 0; index < textures.size(); index++)
			{
				std::string number;
				std::string type = textures[index].type;
				if (type == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (type == "texture_specular")
					number = std::to_string(specularNr++); // transfer unsigned int to string
				else if (type == "texture_normal")
					number = std::to_string(normalNr++); // transfer unsigned int to string
				else if (type == "texture_height")
					number = std::to_string(heightNr++); // transfer unsigned int to string

				// now set the sampler to the correct texture unit
				shader->SetUniform((type + number).c_str(), index);
				textureMap[textures[index].name]->Bind(index);
			}
			const auto& paramsJson = material->GetJson();
			if (paramsJson.contains("textures"))
			{
				const auto& textures = paramsJson["textures"];
				for (auto& iterator : textures.items())
				{
					auto& texturesContent = iterator.value();
					std::string texturesNameString = texturesContent["name"].get<std::string>();
					const char* texturesName = texturesNameString.c_str();
					std::string texturesPath = texturesContent["path"].get<std::string>();
					std::shared_ptr<Texture> curTexture = textureMap[texturesPath];
					shader->SetUniform(texturesName, index);
					curTexture->Bind(index);
					index++;
				}
			}

			// Create instancedMatricesBuffer
			Buffer matricesBuffer(BufferType::Vertex, DataUsage::Static, &modelMatrices[0], sizeof(glm::mat4) * modelMatrices.size());

			vertexArray.Bind();
			vertexArray.SetAtribute(7, 4, GL_FLOAT, sizeof(glm::mat4), 0);
			vertexArray.SetAtribute(8, 4, GL_FLOAT, sizeof(glm::mat4), sizeof(glm::vec4));
			vertexArray.SetAtribute(9, 4, GL_FLOAT, sizeof(glm::mat4), 2 * sizeof(glm::vec4));
			vertexArray.SetAtribute(10, 4, GL_FLOAT, sizeof(glm::mat4), 3 * sizeof(glm::vec4));

			glVertexAttribDivisor(7, 1);
			glVertexAttribDivisor(8, 1);
			glVertexAttribDivisor(9, 1);
			glVertexAttribDivisor(10, 1);
			vertexArray.Unbind();
			vertexArray.DrawInstanced(modelMatrices.size());
			shader->Unuse();
		}
	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<MeshTexture> textures;
		VertexArray vertexArray;
	};
}