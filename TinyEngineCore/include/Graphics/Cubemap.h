#pragma once

#include "Graphics/Texture2D.h"
#include <vector>

namespace TinyEngine
{
	class Cubemap
	{
	public:
		Cubemap(const std::vector<std::string>& faces) : ID(0)
		{
			InitializeCubemap(faces);
		}
		~Cubemap()
		{
			std::cout << "Cubemap Destroyed: " << this->ID << std::endl;
			Destroy();
			this->ID = 0;
		}
		void Bind(int slot) const
		{
			if (slot < 0 || slot > GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
				throw std::invalid_argument("Slot Number Error: " + std::to_string(slot));
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
		}
		void Unbind() const
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
		void Destroy()
		{
			if(this->ID)
				glDeleteTextures(1, &this->ID);
		}
	private:
		void InitializeCubemap(const std::vector<std::string>& faces)
		{			
			glGenTextures(1, &this->ID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
			int width, height, nrChannels;
			for (unsigned int i = 0; i < faces.size(); i++)
			{
				stbi_set_flip_vertically_on_load(false);
				unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
				unsigned int textureFormat = GL_RGB;
				if (nrChannels == 1) textureFormat = GL_RED;
				else if (nrChannels == 3) textureFormat = GL_RGB;
				else if (nrChannels == 4) textureFormat = GL_RGBA;
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	private:
		unsigned int ID;
	};
}