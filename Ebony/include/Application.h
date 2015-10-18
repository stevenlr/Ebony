#ifndef EBONY_APPLICATION_H_
#define EBONY_APPLICATION_H_

#include "graphics/opengl/opengl.h"
#include "graphics/TransformPipelineStereo.h"
#include "graphics/StaticModel.h"

#include <memory>
#include <vector>

namespace ebony {

	class Application {
	public:
		Application();
		
		void update(float dt);
		void draw(float extrapolationTime);
			
	private:
		std::vector<StaticModel> _models;
		std::vector<std::shared_ptr<gl::Texture>> _textures;
		std::shared_ptr<gl::Program> _program;
		std::shared_ptr<gl::Sampler> _sampler;
		TransformPipelineStereo _pipeline;
		float _time = 0;
		GLuint _uniformMvp;
		GLuint _uniformSampler;
	};

}

#endif
