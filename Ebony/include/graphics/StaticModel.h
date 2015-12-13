#ifndef EBONY_GRAPHICS_STATIC_MODEL_H_
#define EBONY_GRAPHICS_STATIC_MODEL_H_

#include <memory>
#include <string>

#include "opengl/Object.h"

namespace ebony {
	
class StaticModel {
public:
	StaticModel(const std::string &filename);
	StaticModel(const StaticModel &model) = default;
	~StaticModel() = default;

	void draw(int n = 1) const;

private:
	std::shared_ptr<gl::VertexArray> _vao;
	std::shared_ptr<gl::Buffer> _vbo;
	int _nbFaces;
};

}

#endif
