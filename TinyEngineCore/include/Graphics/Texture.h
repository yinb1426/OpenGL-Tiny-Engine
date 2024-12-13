#pragma once
namespace TinyEngine
{
	class Texture
	{
	public:
		Texture(const char* texturePath);
		~Texture();
		void SetWarpMode(int warpMode);
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
