#ifndef EBONY_APPLICATION_H_
#define EBONY_APPLICATION_H_

#include "graphics/opengl/opengl.h"
#include "graphics/TransformPipelineStereo.h"
#include "graphics/StaticModel.h"

#include <memory>

namespace ebony {

	class Application {
	public:
		Application();
		
		void update(float dt);
		void draw(float extrapolationTime);
			
	private:
		StaticModel _model;
		std::shared_ptr<gl::Program> _program;
		TransformPipelineStereo _pipeline;
		float _time = 0;
		GLuint _uniformMvp;
	};

}

#endif
