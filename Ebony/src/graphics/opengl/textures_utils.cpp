#include "graphics/opengl/textures_utils.h"

#include <cstdint>

#include <stb_image.h>

using namespace std;

namespace ebony { namespace gl {

	bool loadTextureFromFile(const Texture &texture, const std::string &filename, std::string &error)
	{
		int width, height, components;

		uint8_t *data = stbi_load(filename.c_str(), &width, &height, &components, 0);

		if (!data) {
			error = "Couldn't open texture ";
			error += filename;
			return false;
		}

		if (width <= 0 || height <= 0 || components <= 0 || components > 4) {
			error = "Invalid texture ";
			error += filename;
			return false;
		}

		GLenum internalFormat, format;

		switch (components) {
			case 1:
				internalFormat = GL_R8;
				format = GL_R;
				break;
			case 2:
				internalFormat = GL_RG8;
				format = GL_RG;
				break;
			case 3:
				internalFormat = GL_RGB8;
				format = GL_RGB;
				break;
			case 4:
				internalFormat = GL_RGBA8;
				format = GL_RGBA;
				break;
		}

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}

	void generateMipmaps(const Texture &texture)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}}