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

void GUI::handleInput()
{
	while (SDL_PollEvent(&_event)) {
		if (_event.type == SDL_QUIT)
			_quit = true;
		if (_event.type == SDL_KEYUP)
			if (_event.key.keysym.sym == SDLK_ESCAPE ||
					_event.key.keysym.sym == SDLK_q)
			_quit = true;
	}
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
		Draw();

		SDL_GL_SwapWindow(_window);
	}
}

void GUI::Draw()
{
	ImVec4 clear_col = ImColor(222, 222, 222);
	glClearColor(clear_col.x, clear_col.y, clear_col.z, 1);

	bool shown = true;

	const int padding = 4;
	const ImVec2 size = ImVec2(Window_Width-padding*2, Window_Height-padding*2);
	ImGui::Begin("Main Window", &shown, size, 0.65f,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove);
	const ImVec2 position = ImVec2(padding, padding);
	ImGui::SetWindowPos(position);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);

	ImGui::Text("sythin");
	ImGui::Spacing();

	unsigned int freq_req = 123456,
				 freq_obt = 123456,
				 samp_req = 1234,
				 samp_obt = 1234;
	ImGui::BeginChild("column", ImVec2(ImGui::GetWindowWidth() * 2.f/3.f, 38));
	ImGui::Columns(4, "requested-obtained", false);
	ImGui::Text("Frequency requested");
	ImGui::NextColumn();
	ImGui::Text("%d", freq_req);
	ImGui::NextColumn();
	ImGui::Text("Samples requested");
	ImGui::NextColumn();
	ImGui::Text("%d", samp_req);
	ImGui::NextColumn();
	ImGui::Text("Frequency obtained");
	ImGui::NextColumn();
	ImGui::Text("%d", freq_obt);
	ImGui::NextColumn();
	ImGui::Text("Samples obtained");
	ImGui::NextColumn();
	ImGui::Text("%d", samp_obt);
	ImGui::Columns(1);
	ImGui::EndChild();
	ImGui::Separator();

	static ImVector<float> values;
	if (values.empty()) {
		values.resize(100);
		memset(&values.front(), 0, values.size()*sizeof(float));
	}
	static size_t values_offset = 0;
	static float last_time = -1.0f;
	if (ImGui::GetTime() > last_time + 1.0f/60.0f) {
		last_time = ImGui::GetTime();
		static float phase = 0.0f;
		values[values_offset] = cosf(phase);
		values_offset = (values_offset+1) % values.size();
		phase += 0.10f*values_offset;
	}
	ImGui::PlotLines("", &values.front(), (int)values.size(), (int)values_offset, "wave #1 sine", -1.0f, 1.0f, ImVec2(0,80));

	ImGui::End();

	ImGui::ShowTestWindow(&shown);

	ImGui::Render();
}

GUI::~GUI()
{
	SDL_GL_DeleteContext(_gl_context);
	SDL_DestroyWindow(_window);
	SDL_CloseAudio();
	SDL_Quit();
}

