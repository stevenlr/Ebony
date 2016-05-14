#ifndef EBONY_EBONY_H_
#define EBONY_EBONY_H_

#include <SDL2/SDL.h>

namespace ebony {

class Ebony {
public:
	static void init(const char *appName, int width, int height);
	static void quit();

	static Ebony *getInstance();

	inline SDL_Window *getSDLWindow() { return _window; }

private:
	Ebony(int width, int height);
	~Ebony();

	Ebony(const Ebony &e) = delete;
	Ebony &operator=(const Ebony &e) = delete;

	void initSDL(const char *appName);
	void quitSDL();

	static Ebony		*	_instance;

	SDL_Window			*	_window;
	SDL_GLContext			_glContext;

	int						_width;
	int						_height;
};

}

#endif
