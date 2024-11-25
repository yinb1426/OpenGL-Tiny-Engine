#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "Shader.h"
#include "Texture.h"
namespace TinyEngine
{
	using ShaderMap = std::unordered_map<std::string, std::shared_ptr<Shader>>;
	// using ModelMap = std::unordered_map<std::string, std::shared_ptr<Model>>;
	using TextureMap = std::unordered_map<std::string, std::shared_ptr<Texture>>;
}