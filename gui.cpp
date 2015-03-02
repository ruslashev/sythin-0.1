#include "gui.hpp"

GUI::GUI()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		errorf("Failed to initialize SDL");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	_window = SDL_CreateWindow("Sythin",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			Window_Width, Window_Height,
			SDL_WINDOW_OPENGL);
	assertf(_window != NULL, "Failed to create window");

	_gl_context = SDL_GL_CreateContext(_window);

	_quit = false;

	opengl.Construct();
}

void GUI::MainLoop()
{
	_old_time = 0;

	while (!_quit) {
		handleInput();
		_new_time = SDL_GetTicks();
		unsigned int time_difference = _new_time - _old_time;

		opengl.Update(time_difference, _new_time);
		opengl.Draw();

		SDL_GL_SwapWindow(_window);
	}
}

void GUI::handleInput()
{
	if (SDL_PollEvent(&_event)) {
		if (_event.type == SDL_QUIT)
			_quit = true;
		if (_event.type == SDL_KEYUP)
			if (_event.key.keysym.sym == SDLK_ESCAPE ||
					_event.key.keysym.sym == SDLK_q)
			_quit = true;
	}
}

GUI::~GUI()
{
	SDL_GL_DeleteContext(_gl_context);
	SDL_DestroyWindow(_window);
	SDL_CloseAudio();
	SDL_Quit();
}

