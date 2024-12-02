#pragma once
#include "Geometry/Model.h"
#include "Geometry/Transform.h"
#include "Graphics/Material.h"
#include "Graphics/Camera.h"
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

		void Render(Camera camera)
		{
			glm::mat4 modelMatrix = transform.GetModelMatrix();
			glm::mat4 viewMatrix = camera.GetViewMatrix();
			glm::mat4 projectionMatrix = camera.GetProjectionMtarix(gGLContext->GetWindowAspect());
			material->SetMatrices(modelMatrix, viewMatrix, projectionMatrix);
			material->SetUniform(gResourceManager->GetTextureMap());
			model->Draw(material->GetShader(), gResourceManager->GetTextureMap());
		}
	};
	using GameObjectMap = std::unordered_map<std::string, GameObject>;
	class Scene
	{
	public:
		Scene() : camera(std::make_shared<Camera>()) {}
		Scene(Camera camera) : camera(std::make_shared<Camera>(camera)) {}
		Scene(Camera camera, GameObjectMap gameObjects) : camera(std::make_shared<Camera>(camera)), gameObjects(gameObjects) {}
		~Scene() {}
		void SetCamera(Camera camera)
		{
			this->camera = std::make_shared<Camera>(camera);
		}
		void SetCamera(std::shared_ptr<Camera>& camera)
		{
			this->camera = std::move(camera);
		}
		std::shared_ptr<Camera> GetCamera() const
		{
			return this->camera;
		}
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
		void Render()  //windowAspect/texturesӦ����ô��ȥ
		{
			for (auto& go : gameObjects)
			{
				go.second.Render(*camera);
			}
		}
	public:
		std::shared_ptr<Camera> camera;
		GameObjectMap gameObjects;
	};
}