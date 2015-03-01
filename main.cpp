#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <math.h>
#include "audio_master.hpp"

int main()
{
	SDL_Init(SDL_INIT_AUDIO);

	const int RequestedFrequency = 44100;
	const int RequestedSamples = 4096;
	AudioMaster audio_master(RequestedFrequency, RequestedSamples);

	SDL_PauseAudio(0);

	SDL_Event event;
	bool running = true;
	while (running) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = false;
							break;
						default: break;
					}
					break;
				case SDL_QUIT:
					running = false;
					break;
			}
			SDL_Delay(1);
		}
		SDL_Delay(1);
	}

	SDL_CloseAudio();
	SDL_Quit();

	return 0;
}

