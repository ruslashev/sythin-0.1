#ifndef GUI_HPP
#define GUI_HPP

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "imgui/imgui.h"

#include "audio_master.hpp"
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
	unsigned int freq_req, freq_obt,
				 samples_req, samples_obt;
	AudioMaster *audio_master;

	GUI();
	~GUI();

	void MainLoop();
	void Draw();
};

#endif

