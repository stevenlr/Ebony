#include <iostream>
#include <memory>
#include <cstdint>
#include <string>

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "input/InputHandler.h"
#include "graphics/opengl/opengl.h"
#include "graphics/StaticModel.h"
#include "graphics/TransformPipeline.h"
#include "utils/io.h"
#include "FPSCamera.h"

#define EBONY_OUTPUT_FPS

using namespace std;
using namespace ebony;

class Application {
public:
	Application()
	{
		string xmlProgram, error;

		if (!readFile("assets/programs/default.xml", xmlProgram)) {
			throw runtime_error("Cannot read program file");
		}

		_program = make_shared<gl::Program>();

		if (!gl::linkProgramFromXml(*_program, xmlProgram, error)) {
			cerr << error << endl;
			throw runtime_error("Couldn't link program");
		}

		_mvpUniform = glGetUniformLocation(*_program, "uMvp");
		_cubemapUniform = glGetUniformLocation(*_program, "uCubemap");

		_model = new StaticModel("assets/models/suzanne.cobj");
		_cube = new StaticModel("assets/models/cube.cobj");

		_cubemap = make_shared<gl::Texture>();

		if (!gl::loadCubemapFromDirectory(*_cubemap, "assets/textures/cubemap_gb", error)) {
			cerr << error << endl;
			throw runtime_error("Couldn't load cubemap");
		}

		_sampler = make_shared<gl::Sampler>();
		glSamplerParameteri(*_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(*_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(*_sampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(*_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(*_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		_transform.perspective(70.0f, 1280, 720, 0.01f, 1000.0f);
	}

	~Application()
	{
		delete _model;
	}

	void update(float dt)
	{
		_time += dt;
		_camera.update(dt);
	}

	void draw(float dt)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, *_cubemap);
		glBindSampler(0, *_sampler);

		glUseProgram(*_program);
		_camera.setTransformPipeline(_transform);
		glUniformMatrix4fv(_mvpUniform, 1, false, glm::value_ptr(_transform.getMvp()));
		glUniform1i(_cubemapUniform, 0);
		_cube->draw();
	}

private:
	shared_ptr<gl::Program> _program;
	shared_ptr<gl::Texture> _cubemap;
	shared_ptr<gl::Sampler> _sampler;
	TransformPipeline _transform;
	StaticModel *_model, *_cube;
	GLuint _mvpUniform, _cubemapUniform;
	FPSCamera _camera;
	float _time = 0;
};

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

	if (!gladLoadGL()) {
		return 1;
	}

	SDL_GL_MakeCurrent(window, glContext);
	SDL_GL_SetSwapInterval(1);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, 1280, 720);

	input::InputHandler::init();
	input::InputHandler *inputHandler = input::InputHandler::getInstance();

	bool running = true;
	uint32_t startTime, lastTime = SDL_GetTicks();
	uint32_t timeAccumulator = 0, frameTime;
	int framesSimulated;
	const uint32_t targetFps = 60;
	const uint32_t msPerFrame = 1000 / targetFps;
	const float dt = 1.0f / targetFps; 
	float renderExtrapolationTime;

#ifdef EBONY_OUTPUT_FPS
	uint32_t fpsCounterTime = 0;
	int renderedFrames = 0;
	int simulatedFrames = 0;
#endif

	Application app;
	
	while (running) {
		startTime = SDL_GetTicks();
		timeAccumulator += startTime - lastTime;
		framesSimulated = 0;

		inputHandler->update();

		if (inputHandler->isQuitRequested() || inputHandler->wasKeyPressed(input::KeyboardKeys::Quit)) {
			running = false;
		}

		while (timeAccumulator >= msPerFrame && framesSimulated < 4) {
			// update dt
			app.update(dt);
			timeAccumulator -= msPerFrame;
			++framesSimulated;
#ifdef EBONY_OUTPUT_FPS
			++simulatedFrames;
#endif
		}

		timeAccumulator = timeAccumulator % msPerFrame;
		renderExtrapolationTime = timeAccumulator / 1000.0f;

		// draw renderExtrapolationTime
		app.draw(renderExtrapolationTime);

#ifdef EBONY_OUTPUT_FPS
		++renderedFrames;
		fpsCounterTime += startTime - lastTime;

		if (fpsCounterTime >= 1000) {
			cout << renderedFrames << " fps, " << simulatedFrames << " ups" << endl;

			fpsCounterTime -= 1000;
			renderedFrames = 0;
			simulatedFrames = 0;
		}
#endif

		lastTime = startTime;
		SDL_GL_SwapWindow(window);

		frameTime = SDL_GetTicks() - startTime;
		
		if (frameTime < msPerFrame) {
			SDL_Delay(msPerFrame - frameTime);
		}
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
