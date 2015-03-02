#ifndef OPENGL_MASTER
#define OPENGL_MASTER

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "arraybuffer.hpp"
#include "vertexarray.hpp"
#include "shader.hpp"
#include "shaderprogram.hpp"

#include "../imgui/imgui.h"

const unsigned int Window_Width = 1000;
const unsigned int Window_Height = 500;

class OpenGL
{
	VertexArray vao;
	Shader shader_vert, shader_frag;

	void initResources();
	void load_fonts();
	void init_imgui();
public:
	ShaderProgram shader_program;
	ArrayBuffer vertex_buffer;

	~OpenGL();

	void Construct();
	void Update(unsigned int dt, unsigned int t);
	void Draw();
};

void imgui_render_draw_lists(ImDrawList** const cmd_lists, int count);

#endif

