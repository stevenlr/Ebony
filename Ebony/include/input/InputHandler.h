#ifndef EBONY_INPUT_INPUT_HANDLER_H_
#define EBONY_INPUT_INPUT_HANDLER_H_

namespace ebony {

class InputHandler {
public:
	static void init();
	static InputHandler *getInstance();

	void update();

	bool isQuitRequested() const;
	void getMousePosition(int *x, int *y) const;
	void getMouseMotion(int *x, int *y) const;

private:
	InputHandler();

	static InputHandler *_instance;
	bool _quit = false;
	int _mouseX;
	int _mouseY;
	int _mouseMotionX;
	int _mouseMotionY;
};

};

#endif
