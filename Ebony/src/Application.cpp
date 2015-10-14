#include "Application.h"

#include <string>
#include <stdexcept>

#include <GL/glew.h>

#include "utils/io.h"

using namespace std;
using namespace ebony;

namespace ebony {

	Application::Application() :
		_model("assets/models/suzanne.cobj")
	{
		_program = make_shared<gl::Program>();
		glClearColor(0, 0, 0, 1);
		
		string xmlSource, error;
		
		readFile("assets/programs/default.xml", xmlSource);
		
		if (!linkProgramFromXml(*_program, xmlSource, error)) {
			throw runtime_error(error);
		}
		
		glUseProgram(*_program);
		
		_uniformMvp = glGetUniformLocation(*_program, "uMvp");
		
		_pipeline.perspective(70, 16, 9, 0.0001f, 1000, 0.7f, 0.01f);
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

		_pipeline.lookat(glm::vec3(cos(_time + et) * 3, sin(_time + et) * 3, 1.5f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(0, 0, 1));
		
		_pipeline.identity();
		
		for (int i = 0; i < 2; ++i) {
			glColorMask(masks[i][0], masks[i][1], masks[i][2], GL_FALSE);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glUniformMatrix4fv(_uniformMvp, 1, GL_FALSE, glm::value_ptr(_pipeline.getMvp(eyes[i])));
			
			_model.draw();
		}
	}

}
