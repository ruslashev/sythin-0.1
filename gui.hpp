#ifndef GUI_HPP
#define GUI_HPP

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "imgui/imgui.h"

#include "audio_master.hpp"
#include "opengl/arraybuffer.hpp"
#include "opengl/vertexarray.hpp"
#include "opengl/shaders.hpp"
#include "opengl/opengl.hpp"

class GUI
{
	SDL_Window *_window;
	SDL_GLContext _gl_context;
	SDL_Event _event;
	bool _quit;

	OpenGL opengl;

	void handleInput();
public:
	unsigned int sampFreq, samples;
	AudioMaster *audio_master;

	GUI();
	~GUI();

	void MainLoop();
	void Draw();
};

#endif

