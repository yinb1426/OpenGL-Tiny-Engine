#pragma once
#include "Model.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"
#include <memory>
#include <string>
#include <unordered_map>
namespace TinyEngine
{
	struct GameObject
	{
		std::shared_ptr<Model> model;
		Transform transform;
		std::shared_ptr<Material> material;

		void Render(Camera camera, float windowAspect, TextureMap textureMap)
		{
			glm::mat4 modelMatrix = transform.GetModelMatrix();
			glm::mat4 viewMatrix = camera.GetViewMatrix();
			glm::mat4 projectionMatrix = camera.GetProjectionMtarix(windowAspect);
			material->SetMatrices(modelMatrix, viewMatrix, projectionMatrix);
			material->SetUniform(textureMap);
			model->Draw(material->GetShader(), textureMap);
		}
	};
	using GameObjectMap = std::unordered_map<std::string, GameObject>;
	class Scene
	{
	public:
		Scene(Camera camera) : camera(camera) {}
		Scene(Camera camera, GameObjectMap gameObjects) : camera(camera), gameObjects(gameObjects) {}
		~Scene() {}
		void AddGameObject(std::string name, GameObject go)
		{
			if (this->gameObjects.find(name) == this->gameObjects.end())
				this->gameObjects[name] = go;
			else throw::runtime_error("GameObject" + name + "Already Exist!");
		}
		void DeleteGameObject(std::string name)
		{
			if (this->gameObjects.find(name) != this->gameObjects.end())
				this->gameObjects.erase(name);
		}
		void Render(float windowAspect, TextureMap textures)  //windowAspect/texturesӦ����ô��ȥ
		{
			for (auto& go : gameObjects)
			{
				go.second.Render(camera, windowAspect, textures);
			}
		}
	public:
		Camera camera;
		GameObjectMap gameObjects;
	};
}