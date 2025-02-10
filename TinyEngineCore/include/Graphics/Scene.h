#pragma once
#include "Geometry/Model.h"
#include "Geometry/Transform.h"
#include "Graphics/Material.h"
#include "Graphics/Camera.h"
#include "Graphics/Skybox.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace TinyEngine
{
	struct GameObject
	{
		std::shared_ptr<Model> model;
		std::vector<Transform> transforms;
		std::shared_ptr<Material> material;

		void Render(Camera camera)
		{
			glm::mat4 modelMatrix = transforms.size() == 1 ? transforms[0].GetModelMatrix() : glm::mat4(1.0f);
			glm::mat4 viewMatrix = camera.GetViewMatrix();
			glm::mat4 projectionMatrix = camera.GetProjectionMatrix(gGLContext->GetWindowAspect());
			material->SetMatrices(modelMatrix, viewMatrix, projectionMatrix);
			material->SetUniform();
			model->Draw(material, gResourceManager->GetTextureMap(), transforms);
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
		void SetSkybox(Skybox skybox)
		{
			this->skybox = std::make_shared<Skybox>(skybox);
		}
		void SetSkybox(std::shared_ptr<Skybox>& skybox)
		{
			this->skybox = std::move(skybox);
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
		void Render()
		{
			for (auto& go : gameObjects)
			{
				go.second.Render(*camera);
			}
			skybox->DrawSkybox(*camera, gResourceManager->GetShader("Skybox Shader")); // TODO: ��ɫ��������
		}
	public:
		std::shared_ptr<Camera> camera;
		GameObjectMap gameObjects;
		std::shared_ptr<Skybox> skybox;
	};
}