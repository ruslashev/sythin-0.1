#ifndef GUI_HPP
#define GUI_HPP

#include <SDL2/SDL.h>
#include <stdexcept>

const unsigned int Window_Width = 1000;
const unsigned int Window_Height = 500;

class GUI
{
	SDL_Window *_window;
	SDL_Renderer *_renderer;
	SDL_Texture *_texture;
	SDL_Event _event;
	bool _quit;
	unsigned int _old_time, _new_time;

	Uint32 _pixels[Window_Width*Window_Height];

	void handleInput();
public:
	GUI();
	~GUI();

	void MainLoop();
	void DrawPixels();
	void WritePixel(unsigned int x, unsigned int y, Uint32 color);
	void Clear();
};

#endif

