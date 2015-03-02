#ifndef GUI_HPP
#define GUI_HPP

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "opengl/arraybuffer.hpp"
#include "opengl/vertexarray.hpp"
#include "opengl/shader.hpp"
#include "opengl/shaderprogram.hpp"
#include "opengl/opengl.hpp"

class GUI
{
	SDL_Window *_window;
	SDL_GLContext _gl_context;
	SDL_Event _event;
	bool _quit;
	unsigned int _old_time, _new_time;

	OpenGL opengl;

	void handleInput();
public:
	GUI();
	~GUI();

	void MainLoop();
};

#endif

