#include <iostream>
#include <memory>
#include <cstdint>

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "input/InputHandler.h"

#define EBONY_OUTPUT_FPS

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

	if (!gladLoadGL()) {
		return 1;
	}

	SDL_GL_MakeCurrent(window, glContext);
	SDL_GL_SetSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, 1280, 720);

	InputHandler::init();
	InputHandler *inputHandler = InputHandler::getInstance();

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
	
	while (running) {
		startTime = SDL_GetTicks();
		timeAccumulator += startTime - lastTime;
		framesSimulated = 0;

		inputHandler->update();

		if (inputHandler->isQuitRequested()) {
			running = false;
		}

		while (timeAccumulator >= msPerFrame && framesSimulated < 4) {
			// update dt
			timeAccumulator -= msPerFrame;
			++framesSimulated;
#ifdef EBONY_OUTPUT_FPS
			++simulatedFrames;
#endif
		}

		timeAccumulator = timeAccumulator % msPerFrame;
		renderExtrapolationTime = timeAccumulator / 1000.0f;

		// draw renderExtrapolationTime

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
