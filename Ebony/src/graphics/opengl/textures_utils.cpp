#include "stdafx.h"
#include "graphics/opengl/textures_utils.h"

#include <functional>

#include "utils/assert.h"
#include "graphics/opengl/opengl.h"

namespace ebony { namespace gl {

namespace {

using unique_ptr_stbi = std::unique_ptr<uint8_t[], std::function<decltype(stbi_image_free)>>;

}

bool getImageData(const std::string &filename, std::string *error,
				  unique_ptr_stbi &data,
				  int *width, int *height, int *components,
				  GLenum *internalFormat, GLenum *format)
{
	ASSERT(width && height && components && internalFormat && format, "");

	data.reset(stbi_load(filename.c_str(), width, height, components, 0));

	if (!data) {
		if (error) {
			*error = "Couldn't open texture ";
			*error += filename;
		}

		return false;
	}

	if (*width <= 0 || *height <= 0 || *components <= 0 || *components > 4) {
		if (error) {
			*error = "Invalid texture ";
			*error += filename;
		}

		return false;
	}

	switch (*components) {
		case 1:
			*internalFormat = GL_R8;
			*format = GL_RED;
			break;
		case 2:
			*internalFormat = GL_RG8;
			*format = GL_RG;
			break;
		case 3:
			*internalFormat = GL_RGB8;
			*format = GL_RGB;
			break;
		case 4:
			*internalFormat = GL_RGBA8;
			*format = GL_RGBA;
			break;
	}

	return true;
}

bool loadTextureFromFile(const Texture &texture, const std::string &filename, std::string *error)
{
	int width, height, components;
	GLenum internalFormat, format;
	unique_ptr_stbi data(static_cast<uint8_t *>(nullptr), stbi_image_free);

	if (!getImageData(filename, error, data, &width, &height, &components, &internalFormat, &format)) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data.get());
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool loadCubemapFromDirectory(const Texture &texture, const std::string &dirname, std::string *error)
{
	static const std::string sidesStr[6] = {
		std::string("posx.png"), std::string("posy.png"), std::string("posz.png"),
		std::string("negx.png"), std::string("negy.png"), std::string("negz.png")
	};

	static const GLuint sidesGl[6] {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	int width, height, components;
	GLenum internalFormat, format;
	unique_ptr_stbi data(static_cast<uint8_t *>(nullptr), stbi_image_free);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	for (int i = 0; i < 6; ++i) {
		std::string filename = dirname + "/" + sidesStr[i];

		if (!getImageData(filename, error, data, &width, &height, &components, &internalFormat, &format)) {
			return false;
		}

		glTexImage2D(sidesGl[i], 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data.get());
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
