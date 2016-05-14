#include "stdafx.h"

#include "Ebony.h"

#include "input/InputHandler.h"
#include "utils/assert.h"

namespace ebony {

Ebony *Ebony::_instance = nullptr;

Ebony::Ebony(int width, int height)
	: _width(width), _height(height)
{
	ASSERT(width > 0 && height > 0, "Invalid window size");
}

Ebony::~Ebony()
{
}

void Ebony::init(const char *appName, int width, int height)
{
	ASSERT(!_instance, "Double Ebony initialization");

	_instance = new Ebony(width, height);
	_instance->initSDL(appName);

	input::InputHandler::init();
}

void Ebony::quit()
{
	_instance->quitSDL();

	delete _instance;
	_instance = nullptr;
}

Ebony *Ebony::getInstance()
{
	ASSERT(_instance, "Getting Ebony instance before initialization");

	return _instance;
}

void Ebony::initSDL(const char *appName)
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

	_window = SDL_CreateWindow(appName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL);
	_glContext = SDL_GL_CreateContext(_window);

	if (!gladLoadGL()) {
		ASSERT(false, "Error loading OpenGL");
	}

	SDL_GL_MakeCurrent(_window, _glContext);
	SDL_GL_SetSwapInterval(1);

	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Ebony::quitSDL()
{
	SDL_GL_DeleteContext(_glContext);
	SDL_DestroyWindow(_window);
	SDL_Quit();

	_window = nullptr;
}

}
