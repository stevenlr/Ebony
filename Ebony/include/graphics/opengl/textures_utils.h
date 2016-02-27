#ifndef EBONY_GRAPHICS_OPENGL_TEXTURE_UTILS_H_
#define EBONY_GRAPHICS_OPENGL_TEXTURE_UTILS_H_

#include <string>

#include "Object.h"

namespace ebony { namespace gl {

bool loadTextureFromFile(const Texture &texture, const std::string &filename, std::string &error);
bool loadCubemapFromDirectory(const Texture &texture, const std::string &filename, std::string &error);
void generateMipmaps(const Texture &texture);

}}

#endif
