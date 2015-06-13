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
		if (_event.type == SDL_QUIT) {
			_quit = true;
		} else if (_event.type == SDL_KEYUP) {
			if (_event.key.keysym.sym == SDLK_ESCAPE ||
					_event.key.keysym.sym == SDLK_q)
				_quit = true;
			else if (_event.key.keysym.sym == SDLK_a)
				audio_master->audio_waves[0].Playing = false;
			else if (_event.key.keysym.sym == SDLK_s)
				audio_master->audio_waves[1].Playing = false;
		} else if (_event.type == SDL_KEYDOWN) {
			if (_event.key.keysym.sym == SDLK_a)
				audio_master->audio_waves[0].Playing = true;
			else if (_event.key.keysym.sym == SDLK_s)
				audio_master->audio_waves[1].Playing = true;
		}
	}
}

void GUI::MainLoop()
{
	unsigned int old_time = 0, new_time;

	while (!_quit) {
		handleInput();

		new_time = SDL_GetTicks();
		unsigned int time_difference = new_time - old_time;

		opengl.Update(time_difference, new_time);
		opengl.Draw();
		Draw();

		SDL_GL_SwapWindow(_window);
	}
}

void GUI::Draw()
{
	ImVec4 clear_col = ImColor(222, 222, 222);
	glClearColor(clear_col.x, clear_col.y, clear_col.z, 1);
	glClear(GL_COLOR_BUFFER_BIT);

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

	ImGui::Text("Frequency requested: %7d   Samples requested: %6d",
			freq_req, samples_req);
	ImGui::Text("Frequency obtained:  %7d   Samples obtained:  %6d",
			freq_obt, samples_obt);
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	int i = 0;
	for (auto &wave : audio_master->audio_waves) {
		i++;
		ImVec4 color = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		if (wave.Playing)
			color = ImVec4(1., 0.6f, 0.6f, 1.00f);
		ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::Text("wave #%d\n%s", i, wave.Name.c_str());
		ImGui::PopStyleColor();

		ImGui::SameLine();
		float values[400];
		int j = 0;
		for (auto &v : wave.values) {
			values[j] = v;
			j++;
		}
		ImGui::PlotLines("",
				values, 400, 0, "", -127, 127, ImVec2(0,90));
	}

	ImGui::End();

	ImGui::Render();
}

GUI::~GUI()
{
	SDL_GL_DeleteContext(_gl_context);
	SDL_DestroyWindow(_window);
	SDL_CloseAudio();
	SDL_Quit();
}

