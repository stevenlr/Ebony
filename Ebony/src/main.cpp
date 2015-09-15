#include <iostream>
#include <memory>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stb_image.h>

#include "graphics/TransformPipeline.h"
#include "graphics/opengl/opengl.h"
#include "utils/io.h"
#include "utils/perlin.h"
#include "utils/maths.h"
#include "utils/Pool.h"

#include "ecs/ecs.h"

using namespace std;
using namespace ebony;

void testECS()
{
	shared_ptr<ecs::EntityManager> manager = ecs::EntityManager::makeInstance();

	ecs::Entity e1 = manager->create();
	ecs::Entity e2 = manager->create();
	ecs::Entity e3 = e1;

	cout << e1.isValid() << endl;
	cout << e2.isValid() << endl;
	cout << e3.isValid() << endl;

	e1.destroy();

	cout << e1.isValid() << endl;
	cout << e2.isValid() << endl;
	cout << e3.isValid() << endl;

	e3 = manager->create();

	cout << e1.isValid() << endl;
	cout << e2.isValid() << endl;
	cout << e3.isValid() << endl;

	Pool<int> pool;

	int *a = pool.allocate();

	*a = 42;

	cout << "ok" << endl;
}

int main(int argc, char *argv[])
{
	/*SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER);

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

	bool running = true;*/

	testECS();
	/*
	while (running) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();*/

	return 0;
}
