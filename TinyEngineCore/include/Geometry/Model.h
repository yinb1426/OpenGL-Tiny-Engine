#pragma once
#include "Geometry/Mesh.h"
#include <Geometry/Transform.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
using namespace std;
namespace TinyEngine
{
	using TextureMap = std::unordered_map<std::string, std::shared_ptr<Texture>>;
	class Model
	{
	public:
		Model(const char* path)
		{
			LoadModel(path);
		}

		~Model() {}

		void Draw(std::shared_ptr<Shader> shader, TextureMap textureMap, std::vector<Transform> transforms)
		{
			for (auto& mesh : meshes)
			{
				if (transforms.size() == 1)
					mesh.Draw(shader, textureMap);
				else
				{
					std::vector<glm::mat4> modelMatrices;
					for (auto& transform : transforms)
						modelMatrices.push_back(transform.GetModelMatrix());
					mesh.DrawInstanced(shader, textureMap, modelMatrices);
				}
			}
		}

		std::vector<MeshTexture> GetLoadedTextures() const
		{
			return this->loadedTextures;
		}
	private:
		void LoadModel(const char* path)
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
				throw std::runtime_error(importer.GetErrorString());
			std::string pathString = std::string(path);
			directory = pathString.substr(0, pathString.find_last_of("/"));
			ProcessNode(scene->mRootNode, scene);
		}
		void ProcessNode(aiNode* node, const aiScene* scene)
		{
			// 加载当前节点下的每一个Mesh
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(ProcessMesh(mesh, scene));
			}

			// 处理当前接节点下的子节点
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				ProcessNode(node->mChildren[i], scene);
			}
		}
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<MeshTexture> textures;

			// vertex
			for (int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				glm::vec3 vector;
				// position
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.position = vector;
				// normal
				if (mesh->HasNormals())
				{
					vector.x = mesh->mNormals[i].x;
					vector.y = mesh->mNormals[i].y;
					vector.z = mesh->mNormals[i].z;
					vertex.normal = vector;
				}
				// texture coordinates
				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					glm::vec2 vec;
					// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
					// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.texCoords = vec;
					// tangent
					vector.x = mesh->mTangents[i].x;
					vector.y = mesh->mTangents[i].y;
					vector.z = mesh->mTangents[i].z;
					vertex.tangent = vector;
					// bitangent
					vector.x = mesh->mBitangents[i].x;
					vector.y = mesh->mBitangents[i].y;
					vector.z = mesh->mBitangents[i].z;
					vertex.bitangent = vector;
				}
				else
					vertex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex);
			}

			// indices
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}
			// textures
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// 1. diffuse maps
			std::vector<MeshTexture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. specular maps
			std::vector<MeshTexture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			// 3. normal maps
			std::vector<MeshTexture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			// 4. height maps
			std::vector<MeshTexture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

			// return a mesh object created from the extracted mesh data
			return Mesh(vertices, indices, textures);
		}
		std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
		{
			std::vector<MeshTexture> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				bool skip = false;
				for (unsigned int j = 0; j < this->loadedTextures.size(); j++)
				{
					if (std::strcmp(this->loadedTextures[j].path.data(), str.C_Str()) == 0)
					{
						textures.push_back(this->loadedTextures[j]);
						skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
						break;
					}
				}
				if (!skip)
				{   // if texture hasn't been loaded already, load it
					MeshTexture texture;
					std::string fileName = std::string(str.C_Str());
					texture.name = fileName.substr(0, fileName.find_last_of('.'));
					texture.type = typeName;
					texture.path = this->directory + "/" + fileName;
					textures.push_back(texture);
					this->loadedTextures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
				}
			}
			return textures;
		}
	private:
		std::vector<Mesh> meshes;
		std::vector<MeshTexture> loadedTextures;
		std::string directory;
	};
}