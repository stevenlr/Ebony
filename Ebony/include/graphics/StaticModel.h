#ifndef EBONY_GRAPHICS_STATIC_MODEL_H_
#define EBONY_GRAPHICS_STATIC_MODEL_H_

#include <memory>
#include <string>

#include "opengl/opengl.h"

namespace ebony {
	
class StaticModel {
public:
	StaticModel(const std::string &filename);
	StaticModel(const StaticModel &model) = default;
	~StaticModel() = default;

	StaticModel &operator=(const StaticModel &model) = default;

	void draw(uint32_t n = 1) const;

private:
	std::unique_ptr<gl::VertexArray>	_vao;
	std::unique_ptr<gl::Buffer>			_vbo;
	uint32_t							_nbFaces;
};

}

#endif
