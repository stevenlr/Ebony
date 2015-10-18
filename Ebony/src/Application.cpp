#include "Application.h"

#include <string>
#include <stdexcept>
#include <iostream>

#include <GL/glew.h>

#include "utils/io.h"
#include "graphics/opengl/textures_utils.h"

using namespace std;
using namespace ebony;

namespace ebony {

	Application::Application()
	{
		_program = make_shared<gl::Program>();
		glClearColor(0, 0, 0, 1);
		
		string xmlSource, error;
		string modelNames[] = {"floor", "table", "suzanne", "cone", "torus"};
		
		readFile("assets/programs/default.xml", xmlSource);
		
		if (!linkProgramFromXml(*_program, xmlSource, error)) {
			throw runtime_error(error);
		}
		
		glUseProgram(*_program);
		_uniformMvp = glGetUniformLocation(*_program, "uMvp");
		_uniformSampler = glGetUniformLocation(*_program, "uSampler");
		
		_pipeline.perspective(70, 16, 9, 0.0001f, 1000, 3.0f, 0.01f);

		for (int i = 0; i < 5; ++i) {
			shared_ptr<gl::Texture> texture = make_shared<gl::Texture>();

			_models.push_back(StaticModel("assets/models/" + modelNames[i] + ".cobj"));
			_textures.push_back(texture);

			if (!gl::loadTextureFromFile(*texture, "assets/textures/" + modelNames[i] + ".png", error)) {
				throw runtime_error(error);
			}
			
			gl::generateMipmaps(*texture);
		}

		_sampler = make_shared<gl::Sampler>();
		glSamplerParameteri(*_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(*_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(*_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(*_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	
	void Application::update(float dt)
	{
		_time += dt;
	}
	
	void Application::draw(float et)
	{
		static TransformPipelineStereo::Eye eyes[2] = {TransformPipelineStereo::Left,
													   TransformPipelineStereo::Right};
		static GLenum masks[2][3] = {{GL_TRUE, GL_FALSE, GL_FALSE},
									 {GL_FALSE, GL_TRUE, GL_TRUE}};

		_pipeline.lookat(glm::vec3(cos((_time + et) * 0.5f) * 8, sin((_time + et) * 0.5f) * 8, 2.0f),
						 glm::vec3(0, 0, 1.5),
						 glm::vec3(0, 0, 1));
		
		_pipeline.identity();

		glBindSampler(0, *_sampler);
		glActiveTexture(GL_TEXTURE0);
		
		for (int i = 0; i < 2; ++i) {
			glColorMask(masks[i][0], masks[i][1], masks[i][2], GL_FALSE);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glUniformMatrix4fv(_uniformMvp, 1, GL_FALSE, glm::value_ptr(_pipeline.getMvp(eyes[i])));
			
			for (int i = 0; i < 5; ++i) {
				glBindTexture(GL_TEXTURE_2D, *_textures[i]);
				_models[i].draw();
			}
		}
	}

}
