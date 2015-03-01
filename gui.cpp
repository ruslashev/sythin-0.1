#include "gui.hpp"

GUI::GUI()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0)
		throw std::runtime_error("Failed to initialize SDL");

	_window = SDL_CreateWindow("Sythin",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			Window_Width, Window_Height,
			0);
	if (_window == NULL)
		throw std::runtime_error("Failed to create window");

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if (_renderer == NULL)
		throw std::runtime_error("Failed to create renderer");

	_texture = SDL_CreateTexture(_renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STREAMING, Window_Width, Window_Height);

	_quit = false;

	Clear();
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

void GUI::MainLoop()
{
	_old_time = 0;

	while (!_quit) {
		handleInput();
		_new_time = SDL_GetTicks();
		// unsigned int time_difference = _new_time - _old_time;
		DrawPixels();
	}
}

GUI::~GUI()
{
	SDL_DestroyTexture(_texture);
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_CloseAudio();
	SDL_Quit();
}

void GUI::DrawPixels() {
	SDL_UpdateTexture(_texture, NULL, _pixels, Window_Width*sizeof(Uint32));
	SDL_RenderClear(_renderer);
	SDL_RenderCopy(_renderer, _texture, NULL, NULL);
	SDL_RenderPresent(_renderer);
}

void GUI::WritePixel(unsigned int x, unsigned int y, Uint32 color)
{
	if ((x >= Window_Width) || (y >= Window_Height))
		throw std::runtime_error("Attempt to write pixels outside of bounds");

	_pixels[y*Window_Width + x] = color;
}

void GUI::Clear()
{
	for (unsigned int y = 0; y < Window_Height; y++)
		for (unsigned int x = 0; x < Window_Width; x++)
			WritePixel(x, y, 0xFFFFFFFF);
}
