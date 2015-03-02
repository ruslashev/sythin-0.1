#include "gui.hpp"

void OpenGL::Construct()
{
	glewExperimental = GL_TRUE;
	GLenum glewstatus = glewInit();
	if (glewstatus != GLEW_OK)
		errorf((const char*)glewGetErrorString(glewstatus));
	
	initResources();
}

void OpenGL::initResources()
{
	vao.Construct();

	std::vector<GLfloat> vertices;
	for (int i = 0; i < 100; i++) {
		const float x = (float)i/100.f;
		vertices.push_back(x - 0.5f);
		vertices.push_back(sinf(x*M_PI*2) - 0.5f);
	}
	vertex_buffer.Construct();
	vertex_buffer.Upload(vertices);

	shader_vert.Construct("shader.vs", GL_VERTEX_SHADER);
	shader_frag.Construct("shader.fs", GL_FRAGMENT_SHADER);

	shader_program.Construct(&shader_vert, &shader_frag);
	shader_program.BindAttribute(&vertex_buffer, "position",
			2, GL_FLOAT, GL_FALSE, 0, 0);
}

void OpenGL::Update(unsigned int dt, unsigned int t)
{
}

void OpenGL::Draw()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_LINES, 0, 100);
}

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

