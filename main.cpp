#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <math.h>

// const int Amplitude = 28000;
const int DesiredFrequency = 44100;
const int DesiredSamples = 4096;
unsigned int SampleFrequency = 0;

unsigned int phase = 0;

void audio_callback(void *userdata, Uint8 *stream, int length)
{
	unsigned int bytes_per_period = SampleFrequency / 1000;

	for (int i = 0; i < length; i++) {
		int value = (int)(150*sin(phase*2*M_PI / bytes_per_period));
		if (value > 127)
			value = 127;
        if (value < -128)
			value = -128;
		stream[i] = value;
		// phase = (++phase) % bytes_per_period;
		phase++;
		phase %= bytes_per_period;
	}
}

int main()
{
	SDL_Init(SDL_INIT_AUDIO);

	SDL_AudioSpec desired, obtained;

	desired.freq = DesiredFrequency;
	desired.format = AUDIO_S8;
	desired.channels = 1;
	desired.samples = DesiredSamples;
	desired.callback = audio_callback;
	desired.userdata = NULL;

	if (SDL_OpenAudio(&desired, &obtained) < 0)
		return 2;

	SampleFrequency = obtained.freq;

	printf("Frequency: %d\n", SampleFrequency);
	if (DesiredSamples != obtained.samples)
		printf("Samples requested: %d\nSamples obtained: %d\n",
				DesiredSamples, obtained.samples);

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

