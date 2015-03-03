#include "opengl.hpp"

static size_t max_vbo_size = 20000;

void OpenGL::Construct()
{
	glewExperimental = GL_TRUE;
	GLenum glewstatus = glewInit();
	if (glewstatus != GLEW_OK)
		errorf((const char*)glewGetErrorString(glewstatus));

	initResources();
	init_imgui();
}

void OpenGL::initResources()
{
	vao.Construct();

	vertex_buffer.Construct();
	vertex_buffer.Bind();
	glBufferData(GL_ARRAY_BUFFER, max_vbo_size, NULL, GL_DYNAMIC_DRAW);

	shader_vert.Construct("shader.vs", GL_VERTEX_SHADER);
	shader_frag.Construct("shader.fs", GL_FRAGMENT_SHADER);

	shader_program.Construct(&shader_vert, &shader_frag);
	shader_program.UseThisProgram();

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	shader_program.BindAttribute(&vertex_buffer, "position", 2, GL_FLOAT,
			GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
	shader_program.BindAttribute(&vertex_buffer, "uv", 2, GL_FLOAT,
			GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
	shader_program.BindAttribute(&vertex_buffer, "color", 4, GL_UNSIGNED_BYTE,
			GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF
}

void OpenGL::init_imgui()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(Window_Width, Window_Height);
	io.RenderDrawListsFn = imgui_render_draw_lists;
	io.UserData = this;
	io.IniFilename = NULL;

	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	GLuint tex_id;
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, pixels);
	io.Fonts->TexID = (void*)(intptr_t)tex_id;
}

void OpenGL::Update(unsigned int dt, unsigned int t)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = (float)dt/1000.f;
	io.DisplaySize = ImVec2(Window_Width, Window_Height);

	int mouse_x, mouse_y;
	Uint32 mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
	io.MousePos = ImVec2(mouse_x, mouse_y);
	io.MouseDown[0] = mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = !!(mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT));

	ImGui::NewFrame();
}

void OpenGL::Draw()
{
	glViewport(0, 0, Window_Width, Window_Height);
	glClear(GL_COLOR_BUFFER_BIT);
}

OpenGL::~OpenGL()
{
	ImGui::Shutdown();
}

void imgui_render_draw_lists(ImDrawList** const cmd_lists, int count)
{
	if (count == 0)
		return;

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	OpenGL *opengl = (OpenGL*)ImGui::GetIO().UserData;
	opengl->shader_program.UseThisProgram();

	const float w = Window_Width; // alias & cast
	const float h = Window_Height;
	const glm::mat4 proj_ortho_mat = {
		{  2.f/w, 0.f,     0.f, 0.f },
		{  0.f,   2.f/-h,  0.f, 0.f },
		{  0.f,   0.f,    -1.f, 0.f },
		{ -1.f,   1.f,     0.f, 1.f },
	};
	opengl->shader_program.UpdateMatrix(proj_ortho_mat);
    glUniform1i(opengl->shader_program.texture_location, 0);

	size_t vertex_count = 0;
	for (int n = 0; n < count; n++)
		vertex_count += cmd_lists[n]->vtx_buffer.size();

	opengl->vertex_buffer.Bind();

	size_t needed_buffer_size = vertex_count * sizeof(ImDrawVert);
	if (needed_buffer_size > max_vbo_size) {
		max_vbo_size = needed_buffer_size + 5000; // Grow buffer
		glBufferData(GL_ARRAY_BUFFER, max_vbo_size, NULL, GL_STREAM_DRAW);
	}

	// Copy and convert all vertices into a single contiguous buffer
	unsigned char *buffer_data =
		(unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (!buffer_data)
		return;
	for (int n = 0; n < count; n++) {
		const ImDrawList *list = cmd_lists[n];
		const size_t buf_len = list->vtx_buffer.size()*sizeof(ImDrawVert);
		memcpy(buffer_data,
				&list->vtx_buffer[0],
				buf_len);
		buffer_data += buf_len;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	opengl->vertex_buffer.Unbind();

	int cmd_offset = 0;
	for (int n = 0; n < count; n++) {
		const ImDrawList* list = cmd_lists[n];
		int vtx_offset = cmd_offset;
		const ImDrawCmd* pcmd_end = list->commands.end();
		for (const ImDrawCmd* pcmd = list->commands.begin();
				pcmd != pcmd_end;
				pcmd++) {
			glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->texture_id);
			glScissor(
					(int)pcmd->clip_rect.x,
					(int)(Window_Height - pcmd->clip_rect.w),
					(int)(pcmd->clip_rect.z - pcmd->clip_rect.x),
					(int)(pcmd->clip_rect.w - pcmd->clip_rect.y));
			glDrawArrays(GL_TRIANGLES, vtx_offset, pcmd->vtx_count);
			vtx_offset += pcmd->vtx_count;
		}
		cmd_offset = vtx_offset;
	}

	opengl->shader_program.DontUseThisProgram();
	glDisable(GL_SCISSOR_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

