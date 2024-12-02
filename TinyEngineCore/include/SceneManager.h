#pragma once
#include "Scene.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace TinyEngine
{
	class SceneManager
	{
	public:
		SceneManager() : activeSceneName("") {}
		~SceneManager()
		{
			scenes.clear();
			activeSceneName = "";
			std::cout << "Scene Manager Destroyed!" << std::endl;
		}
		void AddScene(std::string name, std::shared_ptr<Scene>& scene)
		{
			scenes[name] = std::move(scene);
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
		void Render(float windowAspect, TextureMap textures)
		{
			scenes[activeSceneName]->Render(windowAspect, textures);
		}
	private:
		std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
		std::string activeSceneName;
	};
}