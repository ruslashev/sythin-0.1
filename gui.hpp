#ifndef GUI_HPP
#define GUI_HPP

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdexcept>

#include "opengl/arraybuffer.hpp"
#include "opengl/vertexarray.hpp"
#include "opengl/shader.hpp"
#include "opengl/shaderprogram.hpp"

const unsigned int Window_Width = 1000;
const unsigned int Window_Height = 500;

class OpenGL
{
	VertexArray vao;
	Shader shader_vert, shader_frag;
	ShaderProgram shader_program;
	ArrayBuffer vertex_buffer;
	void initResources();
public:
	void Construct();
	void Update(unsigned int dt, unsigned int t);
	void Draw();
};

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

