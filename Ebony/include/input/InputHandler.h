#ifndef EBONY_INPUT_INPUT_HANDLER_H_
#define EBONY_INPUT_INPUT_HANDLER_H_

#include <map>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_keycode.h>

namespace ebony { namespace input {

namespace MouseButtons {
enum Aliases {
	Left = SDL_BUTTON_LEFT,
	Right = SDL_BUTTON_RIGHT,
	Middle = SDL_BUTTON_MIDDLE
};
}

namespace KeyboardKeys {
enum Aliases {
	Quit,
	Forward,
	Backward,
	Left,
	Right,
	Up,
	Down
};
}

class InputHandler {
	struct ButtonState {
		bool pressed = false;
		bool released = false;
		bool isDown = false;

		inline void reset()
		{
			pressed = false;
			released = false;
		}

		inline void press()
		{
			pressed = true;
			isDown = true;
		}

		inline void release()
		{
			released = true;
			isDown = false;
		}
	};

public:
	static void init();
	static InputHandler *getInstance();

	void update();

	bool isQuitRequested() const;

	void getMousePosition(int *x, int *y) const;
	void getMouseMotion(int *x, int *y) const;
	void getMouseWheelScroll(int *x, int *y) const;

	bool wasMouseButtonPressed(const MouseButtons::Aliases button) const;
	bool wasMouseButtonReleased(const MouseButtons::Aliases button) const;
	bool isMouseButtonDown(const MouseButtons::Aliases button) const;

	bool wasKeyPressed(const KeyboardKeys::Aliases alias) const;
	bool wasKeyReleased(const KeyboardKeys::Aliases alias) const;
	bool isKeyDown(const KeyboardKeys::Aliases alias) const;

private:
	InputHandler();

	inline static bool isMouseButtonValid(const Uint8 sdlButton, MouseButtons::Aliases *button)
	{
		static const Uint8 mouseButtonMask = SDL_BUTTON_LMASK | SDL_BUTTON_RMASK | SDL_BUTTON_MMASK;

		if (!(sdlButton & mouseButtonMask)) {
			return false;
		}

		*button = static_cast<MouseButtons::Aliases>(sdlButton);
		return true;
	}

	static InputHandler *_instance;

	bool _quit = false;

	int _mouseX;
	int _mouseY;
	int _mouseMotionX;
	int _mouseMotionY;
	int _mouseWheelScrollX;
	int _mouseWheelScrollY;

	std::map<MouseButtons::Aliases, ButtonState> _mouseButtons;
	std::map<KeyboardKeys::Aliases, ButtonState> _keyboardButtons;
	std::map<SDL_Keycode, KeyboardKeys::Aliases> _keyboardAliases;
};

}}

#endif
