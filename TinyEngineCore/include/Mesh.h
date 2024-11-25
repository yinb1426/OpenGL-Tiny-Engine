#pragma once
#include "Buffer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#define MAX_BONE_INFLUENCE 4

namespace TinyEngine
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		int boneIDs[MAX_BONE_INFLUENCE];		//bone indexes which will influence this vertex
		float weights[MAX_BONE_INFLUENCE];	//weights from each bone
	};

	struct MeshTexture
	{
		std::string name;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;
			SetupMesh();
		}
		void Draw()
		{
			glBindVertexArray(this->VAO);
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE0);
		}
	private:
		void SetupMesh()
		{
			glGenVertexArrays(1, &this->VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(this->VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			// vertex Positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			// vertex texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
			// vertex tangent
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
			// vertex bitangent
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
			// ids
			glEnableVertexAttribArray(5);
			glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));
			// weights
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));
			glBindVertexArray(0);
		}
	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<MeshTexture> textures;
		unsigned int VAO;
		unsigned int VBO, EBO; // 之后使用Buffer替换
	};
}