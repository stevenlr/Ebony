#include <iostream>
#include <memory>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stb_image.h>

#include "graphics/TransformPipeline.h"
#include "graphics/opengl/opengl.h"
#include "utils/io.h"

using namespace std;
using namespace ebony;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window *window = SDL_CreateWindow("Ebony", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		return 1;
	}

	SDL_GL_MakeCurrent(window, glContext);
	SDL_GL_SetSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glClearColor(0, 0, 0, 1);

	bool running = true;

	float triangleData[] = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0};

	shared_ptr<gl::VertexArray> vao(new gl::VertexArray());
	shared_ptr<gl::Buffer> posBuffer(new gl::Buffer());

	glBindVertexArray(*vao);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, *posBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, triangleData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	string source, error;
	shared_ptr<gl::Program> program(new gl::Program());

	readFile("assets/programs/default.xml", source);

	if (!gl::linkProgramFromXml(*program, source, error)) {
		cerr << error << endl;
	}

	glUseProgram(*program);

	shared_ptr<gl::Sampler> sampler(new gl::Sampler());
	shared_ptr<gl::Texture> texture(new gl::Texture());

	glSamplerParameteri(*sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(*sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(*sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(*sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if (!gl::loadTextureFromFile(*texture, "assets/textures/uvgrid1024-color.png", error)) {
		cerr << error << endl;
	}

	gl::generateMipmaps(*texture);

	TransformPipeline pipeline;

	pipeline.perspective(70, 1280, 720, 0.1f, 1000);

	GLint uniformTexture = glGetUniformLocation(*program, "uTexture");
	GLint uniformMvp = glGetUniformLocation(*program, "uMvp");

	glUniform1i(uniformTexture, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glBindSampler(0, *sampler);

	float t = 0;

	while (running) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		t += 0.01f;

		pipeline.identity().scale(2).translate(-0.5, -0.5, 0);

		pipeline.lookat(glm::vec3(cos(t) * 2, sin(t) * 2, 1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uniformMvp, 1, GL_FALSE, glm::value_ptr(pipeline.getMvp()));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		SDL_GL_SwapWindow(window);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
