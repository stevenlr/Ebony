#include "graphics/opengl/textures_utils.h"

#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <stb_image.h>

#include "graphics/opengl/opengl.h"

using namespace std;

namespace ebony { namespace gl {

bool getImageData(const string &filename, string &error,
				  uint8_t *&data,
				  int &width, int &height, int &components,
				  GLenum &internalFormat, GLenum &format)
{
	data = stbi_load(filename.c_str(), &width, &height, &components, 0);

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

	switch (components) {
		case 1:
			internalFormat = GL_R8;
			format = GL_RED;
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

	return true;
}

bool loadTextureFromFile(const Texture &texture, const string &filename, string &error)
{
	int width, height, components;
	GLenum internalFormat, format;
	uint8_t *data;

	if (!getImageData(filename, error, data, width, height, components, internalFormat, format)) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	free(data);

	return true;
}

bool loadCubemapFromDirectory(const Texture &texture, const string &dirname, string &error)
{
	static const string sidesStr[6] = {
		string("posx.png"), string("posy.png"), string("posz.png"),
		string("negx.png"), string("negy.png"), string("negz.png")
	};

	static const GLuint sidesGl[6] {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	int width, height, components;
	GLenum internalFormat, format;
	uint8_t *data;

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	for (int i = 0; i < 6; ++i) {
		string filename = dirname + "/" + sidesStr[i];

		if (!getImageData(filename, error, data, width, height, components, internalFormat, format)) {
			return false;
		}

		glTexImage2D(sidesGl[i], 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		free(data);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

void generateMipmaps(const Texture &texture)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

}}
