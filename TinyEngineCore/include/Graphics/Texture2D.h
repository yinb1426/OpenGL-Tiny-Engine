#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include <3rd/stb_image.h>
#include <3rd/glad/glad.h>
#include <iostream>
#include <string>
#include <cstdlib>

namespace TinyEngine
{
	class Texture2D
	{
	public:
		Texture2D(const char* texturePath)
		{
			InitializeTexture2D(texturePath);
		}
		~Texture2D()
		{
			std::cout << "Texture2D Destroyed: " << this->ID << std::endl;
			Destroy();
			this->ID = 0;
		}
		void SetWrapMode(int wrapMode)
		{
			glBindTexture(GL_TEXTURE_2D, this->ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		void SetWrapMode(GLenum axis, int wrapMode)
		{
			glBindTexture(GL_TEXTURE_2D, this->ID);
			glTexParameteri(GL_TEXTURE_2D, axis, wrapMode);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		void SetFilterMode(int filterMode)
		{
			glBindTexture(GL_TEXTURE_2D, this->ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		void SetMinFilterMode(int filterMode)
		{
			glBindTexture(GL_TEXTURE_2D, this->ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		void SetMagFilterMode(int filterMode)
		{
			glBindTexture(GL_TEXTURE_2D, this->ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		void GenerateMipmap()
		{
			glBindTexture(GL_TEXTURE_2D, this->ID);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		void Bind(int slot) const
		{
			if (slot < 0 || slot > GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
				throw std::invalid_argument("Slot Number Error: " + std::to_string(slot));
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, this->ID);
		}
		void Unbind() const
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		void Destroy()
		{
			if (this->ID)
				glDeleteTextures(1, &this->ID);
		}
	private:
		void InitializeTexture2D(const char* texturePath)
		{
			char absolutePath[1024];
			if (_fullpath(absolutePath, texturePath, 1024))
				this->path = absolutePath;

			stbi_set_flip_vertically_on_load(true);

			glGenTextures(1, &this->ID);
			int nrChannels;
			unsigned char* data = stbi_load(texturePath, &this->width, &this->height, &nrChannels, 0);
			unsigned int textureFormat = GL_RGB;
			if (nrChannels == 1) textureFormat = GL_RED;
			else if (nrChannels == 3) textureFormat = GL_RGB;
			else if (nrChannels == 4) textureFormat = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, this->ID);
			glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, this->width, this->height, 0, textureFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	private:
		int width;
		int height;
		unsigned int ID;
		const char* path;
	};
}
