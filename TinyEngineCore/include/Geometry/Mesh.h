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

		void Draw(std::shared_ptr<Shader> shader, TextureMap textureMap)
		{
			// bind appropriate textures
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			for (unsigned int i = 0; i < textures.size(); i++)
			{
				std::string number;
				std::string type = textures[i].type;
				if (type == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (type == "texture_specular")
					number = std::to_string(specularNr++); // transfer unsigned int to string
				else if (type == "texture_normal")
					number = std::to_string(normalNr++); // transfer unsigned int to string
				else if (type == "texture_height")
					number = std::to_string(heightNr++); // transfer unsigned int to string

				// now set the sampler to the correct texture unit
				
				shader->SetUniform((type + number).c_str(), (int)i);
				textureMap[textures[i].name]->Bind(i);
			}

			vertexArray.Draw();
		}
	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<MeshTexture> textures;
		VertexArray vertexArray;
	};
}