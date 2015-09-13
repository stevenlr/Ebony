#ifndef EBONY_GRAPHICS_OPENGL_ENUMS_H_
#define EBONY_GRAPHICS_OPENGL_ENUMS_H_

#include <GL/glew.h>

namespace ebony { namespace gl {

	enum ShaderType : GLenum {
		VertexShader = GL_VERTEX_SHADER,
		FragmentShader = GL_FRAGMENT_SHADER,
		GeometryShader = GL_GEOMETRY_SHADER
	};

}
}

#endif
