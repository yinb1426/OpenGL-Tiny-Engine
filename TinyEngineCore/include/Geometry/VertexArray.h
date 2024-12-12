#pragma once

#include "Graphics/Buffer.h"
#include <glad/glad.h>
#include <vector>
#include <memory>
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

	class VertexArray
	{
	public:
		VertexArray() {}
		VertexArray(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
		{
			numPoints = static_cast<unsigned int>(indices.size());
			SetupMesh(vertices, indices);
		}
		~VertexArray() {}

		void Bind()
		{
			glBindVertexArray(this->VAO);
		}

		void Unbind()
		{
			glBindVertexArray(0);
		}

		void Draw()
		{
			glBindVertexArray(this->VAO);
			glDrawElements(GL_TRIANGLES, numPoints, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	private:
		void SetupMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
		{
			glGenVertexArrays(1, &this->VAO);
			VBO.CreateBuffer();
			EBO.CreateBuffer();

			glBindVertexArray(this->VAO);
			VBO.BindBufferData(BufferType::Vertex, DataUsage::Static, &vertices[0], vertices.size() * sizeof(Vertex));
			EBO.BindBufferData(BufferType::Index, DataUsage::Static, &indices[0], indices.size() * sizeof(unsigned int));

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
		unsigned int VAO;
		Buffer VBO, EBO;
		unsigned int numPoints;
	};
}
