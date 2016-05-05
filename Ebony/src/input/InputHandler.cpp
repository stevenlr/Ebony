#include "input/InputHandler.h"

#include <stdexcept>

#include <SDL2/SDL.h>

using namespace std;

namespace ebony { namespace input {

InputHandler *InputHandler::_instance = nullptr;

InputHandler::InputHandler()
{
	_mouseButtons.emplace(MouseButtons::Left,	ButtonState());
	_mouseButtons.emplace(MouseButtons::Right,	ButtonState());
	_mouseButtons.emplace(MouseButtons::Middle,	ButtonState());

	_keyboardAliases[SDLK_ESCAPE]	= KeyboardKeys::Quit;
	_keyboardAliases[SDLK_z]		= KeyboardKeys::Forward;
	_keyboardAliases[SDLK_s]		= KeyboardKeys::Backward;
	_keyboardAliases[SDLK_q]		= KeyboardKeys::Left;
	_keyboardAliases[SDLK_d]		= KeyboardKeys::Right;
	_keyboardAliases[SDLK_SPACE]	= KeyboardKeys::Up;
	_keyboardAliases[SDLK_LSHIFT]	= KeyboardKeys::Down;

	for (const pair<SDL_Keycode, KeyboardKeys::Aliases> &key : _keyboardAliases) {
		_keyboardButtons.emplace(key.second, ButtonState());
	}
}

void InputHandler::init()
{
	_instance = new InputHandler();
}

InputHandler *InputHandler::getInstance()
{
	ASSERT(_instance, "Trying to access input handler before initialization");
	return _instance;
}

void InputHandler::update()
{
	static SDL_Event event;

	_mouseMotionX		= 0;
	_mouseMotionY		= 0;
	_mouseWheelScrollX	= 0;
	_mouseWheelScrollY	= 0;
	_quit				= false;

	for (const pair<MouseButtons::Aliases, ButtonState> &button : _mouseButtons) {
		_mouseButtons[button.first].reset();
	}

	for (const pair<KeyboardKeys::Aliases, ButtonState> &key : _keyboardButtons) {
		_keyboardButtons[key.first].reset();
	}

	MouseButtons::Aliases button;
	map<SDL_Keycode, KeyboardKeys::Aliases>::iterator aliasPair;

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
		case SDL_MOUSEBUTTONDOWN:
			if (!isMouseButtonValid(event.button.button, &button)) {
				break;
			}

			_mouseButtons[button].press();
			break;
		case SDL_MOUSEBUTTONUP:
			if (!isMouseButtonValid(event.button.button, &button)) {
				break;
			}

			_mouseButtons[button].release();
			break;
		case SDL_MOUSEWHEEL:
			_mouseWheelScrollX += event.wheel.x;
			_mouseWheelScrollY += event.wheel.y;
			break;
		case SDL_KEYDOWN:
			aliasPair = _keyboardAliases.find(event.key.keysym.sym);

			if (aliasPair == _keyboardAliases.cend()) {
				break;
			}

			_keyboardButtons.find(aliasPair->second)->second.press();
			break;
		case SDL_KEYUP:
			aliasPair = _keyboardAliases.find(event.key.keysym.sym);

			if (aliasPair == _keyboardAliases.cend()) {
				break;
			}

			_keyboardButtons.find(aliasPair->second)->second.release();
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
	if (x) {
		*x = _mouseX;
	}

	if (y) {
		*y = _mouseY;
	}
}

void InputHandler::getMouseMotion(int *x, int *y) const
{
	if (x) {
		*x = _mouseMotionX;
	}

	if (y) {
		*y = _mouseMotionY;
	}
}

void InputHandler::getMouseWheelScroll(int *x, int *y) const
{
	if (x) {
		*x = _mouseWheelScrollX;
	}

	if (y) {
		*y = _mouseWheelScrollY;
	}
}

bool InputHandler::wasMouseButtonPressed(const MouseButtons::Aliases button) const
{
	return _mouseButtons.at(button).pressed;
}

bool InputHandler::wasMouseButtonReleased(const MouseButtons::Aliases button) const
{
	return _mouseButtons.at(button).released;
}

bool InputHandler::isMouseButtonDown(const MouseButtons::Aliases button) const
{
	return _mouseButtons.at(button).isDown;
}

bool InputHandler::wasKeyPressed(const KeyboardKeys::Aliases alias) const
{
	return _keyboardButtons.at(alias).pressed;
}

bool InputHandler::wasKeyReleased(const KeyboardKeys::Aliases alias) const
{
	return _keyboardButtons.at(alias).released;
}

bool InputHandler::isKeyDown(const KeyboardKeys::Aliases alias) const
{
	return _keyboardButtons.at(alias).isDown;
}

}}
