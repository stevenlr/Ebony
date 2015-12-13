#ifndef EBONY_GRAPHICS_OPENGL_SHADERS_UTILS_H_
#define EBONY_GRAPHICS_OPENGL_SHADERS_UTILS_H_

#include <string>

#include "Object.h"

namespace ebony { namespace gl {

bool compileShaderFromSource(const Shader &shader, const std::string &source, std::string &error);
bool linkProgramFromXml(const Program &program, const std::string &xmlSource, std::string &error);

}}

#endif
