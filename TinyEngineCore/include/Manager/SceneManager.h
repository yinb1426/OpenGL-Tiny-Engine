#pragma once
#include "Graphics/Scene.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace TinyEngine
{
	class SceneManager
	{
	public:
		SceneManager() : activeSceneName("") 
		{
			LoadScenes();
		}
		~SceneManager()
		{
			scenes.clear();
			activeSceneName = "";
			std::cout << "SceneManager Destroyed!" << std::endl;
		}
		void AddScene(std::string name, std::shared_ptr<Scene>& scene)
		{
			scenes[name] = std::move(scene);
		}
		void LoadScenes()
		{
			if (fs::exists(SCENE_ROOT_PATH) && fs::is_directory(SCENE_ROOT_PATH))
			{
				for (const auto& entry : fs::directory_iterator(SCENE_ROOT_PATH))
				{
					LoadScene(entry.path().string());
				}
			}
		}
		void SetActiveScene(std::string name)
		{
			if (scenes.find(name) != scenes.end())
				activeSceneName = name;
			else
				throw std::invalid_argument("Scene Name:" + name + "Doesn't Exist!");
		}
		std::shared_ptr<Camera> GetActiveSceneCamera()
		{
			return scenes[activeSceneName]->GetCamera();
		}
		void Render()
		{
			scenes[activeSceneName]->Render();
		}
	private:
		void LoadScene(std::string scenePath)
		{
			std::shared_ptr<Scene> scene = std::make_shared<Scene>();

			std::ifstream sceneFile(scenePath, std::ios::in);
			Json sceneJson = Json::parse(sceneFile);
			std::string sceneName = sceneJson["name"].get<std::string>();

			// Camera
			const auto& cameraJson = sceneJson["camera"];
			std::vector<float> cameraPos = cameraJson["position"].get<std::vector<float>>();
			float cameraYaw = cameraJson["yaw"].get<float>();
			float cameraPitch = cameraJson["pitch"].get<float>();
			float cameraZNear = cameraJson["near_plane"].get<float>();
			float cameraZFar = cameraJson["far_plane"].get<float>();

			Camera camera(glm::vec3(0.0f));
			camera.UpdateCameraParameters(glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]), cameraYaw, cameraPitch, cameraZNear, cameraZFar);
			scene->SetCamera(camera);

			const auto& gosJson = sceneJson["game_objects"];
			for (auto& iterator : gosJson.items())
			{
				auto& goJson = iterator.value();
				std::string goName = goJson["name"].get<std::string>();
				std::string goModelName = goJson["model"].get<std::string>();
				std::string goMaterialName = goJson["material"].get<std::string>();
				const auto& goTransformJson = goJson["transform"];
				std::vector<float> goPosition = goTransformJson["position"].get<std::vector<float>>();
				std::vector<float> goRotation = goTransformJson["rotation"].get<std::vector<float>>();
				std::vector<float> goScale = goTransformJson["scale"].get<std::vector<float>>();

				std::shared_ptr<Model> goModel = gResourceManager->GetModel(goModelName);
				std::shared_ptr<Material> goMaterial = gResourceManager->GetMaterial(goMaterialName);
				scene->AddGameObject(goName, {
					goModel,
					Transform(glm::vec3(goPosition[0], goPosition[1], goPosition[2]), glm::vec3(goRotation[0], goRotation[1], goRotation[2]), glm::vec3(goScale[0], goScale[1], goScale[2])),
					goMaterial
				});
			}
			this->AddScene(sceneName, scene);
		}
	private:
		std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
		std::string activeSceneName;
	};

	extern std::unique_ptr<SceneManager> gSceneManager;
}