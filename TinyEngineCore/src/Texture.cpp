#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include "stb_image.h"
#include <iostream>
#include <glad/glad.h>

namespace TinyEngine
{
	Texture::Texture(const char* texturePath)
	{
		InitializeTexture(texturePath);
	}
	Texture::~Texture()
	{
		std::cout << "Texture Destroyed: " << this->ID << std::endl;
		Destroy();
		this->ID = 0;

	}
	void Texture::SetWarpMode(int warpMode)
	{
		glBindTexture(GL_TEXTURE_2D, this->ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warpMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warpMode);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture::SetFilterMode(int filterMode)
	{
		glBindTexture(GL_TEXTURE_2D, this->ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture::Bind(int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, this->ID);
	}
	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture::Destroy()
	{
		glDeleteTextures(1, &this->ID);
	}
	void Texture::InitializeTexture(const char* texturePath)
	{
		stbi_set_flip_vertically_on_load(true);
		glGenTextures(1, &this->ID);
		glBindTexture(GL_TEXTURE_2D, this->ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int nrChannels;
		unsigned char* data = stbi_load(texturePath, &this->width, &this->height, &nrChannels, 0);
		unsigned int textureFormat = GL_RGBA;
		if (nrChannels == 1) textureFormat = GL_RED;
		else if (nrChannels == 3) textureFormat = GL_RGB;
		else if (nrChannels == 4) textureFormat = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, this->width, this->height, 0, textureFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

