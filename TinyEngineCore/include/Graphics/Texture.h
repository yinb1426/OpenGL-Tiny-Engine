#pragma once
#include <3rd/glad/glad.h>

namespace TinyEngine
{
	class Texture
	{
	public:
		Texture(const char* texturePath);
		~Texture();
		void SetWrapMode(int wrapMode);
		void SetWrapMode(GLenum axis, int wrapMode);
		void SetFilterMode(int filterMode);
		void SetMinFilterMode(int filterMode);
		void SetMagFilterMode(int filterMode);
		void GenerateMipmap();
		void Bind(int slot) const;
		void Unbind() const;
		void Destroy();
	private:
		void InitializeTexture(const char* texturePath);
	private:
		int width;
		int height;
		unsigned int ID;
		const char* path;
	};
}
