#include "input/InputHandler.h"

#include <stdexcept>

#include <SDL2/SDL.h>

using namespace std;

namespace ebony {

InputHandler *InputHandler::_instance = nullptr;

InputHandler::InputHandler()
{
}

void InputHandler::init()
{
	_instance = new InputHandler();
}

InputHandler *InputHandler::getInstance()
{
	if (!_instance) {
		throw runtime_error("Trying to access input handler before initialization");
	}

	return _instance;
}

void InputHandler::update()
{
	static SDL_Event event;

	_mouseMotionX = 0;
	_mouseMotionY = 0;
	_quit = false;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_MOUSEMOTION:
			_mouseX = event.motion.x;
			_mouseY = event.motion.y;
			_mouseMotionX += event.motion.xrel;
			_mouseMotionY += event.motion.yrel;
			break;
		case SDL_QUIT:
			_quit = true;
			break;
		}
	}
}

bool InputHandler::isQuitRequested() const
{
	return _quit;
}

void InputHandler::getMousePosition(int *x, int *y) const
{
	*x = _mouseX;
	*y = _mouseY;
}

void InputHandler::getMouseMotion(int *x, int *y) const
{
	*x = _mouseMotionX;
	*y = _mouseMotionY;
}

}
