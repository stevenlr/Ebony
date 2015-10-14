#include <iostream>
#include <memory>
#include <cstdint>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stb_image.h>

#include "graphics/TransformPipeline.h"
#include "graphics/opengl/opengl.h"
#include "graphics/StaticModel.h"
#include "Application.h"

#define EBONY_OUTPUT_FPS

using namespace std;
using namespace ebony;

int run()
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
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, 1280, 720);

	bool running = true;
	SDL_Event event;
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

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					running = false;
				}
			}
		}

		while (timeAccumulator >= msPerFrame && framesSimulated < 4) {
			app.update(dt);
			timeAccumulator -= msPerFrame;
			++framesSimulated;
#ifdef EBONY_OUTPUT_FPS
			++simulatedFrames;
#endif
		}

		timeAccumulator = timeAccumulator % msPerFrame;
		renderExtrapolationTime = timeAccumulator / 1000.0f;

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

int main(int argc, char *argv[])
{
	try {
		return run();
	} catch (exception e) {
		cerr << e.what() << endl;
	}
}