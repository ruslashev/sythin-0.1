#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <math.h>

const int Amplitude = 28000;
const int Frequency = 44100;
unsigned int time;
int total_length;

void audio_callback(void *userdata, Uint8 *stream, int length)
{
	Sint16 *buffer = (Sint16*)stream;
	length /= 2;
	for (int i = 0; i < length; i++) {
		buffer[i] = Amplitude * sin(2*M_PI*time);
		time++;
	}
	total_length -= length;
}

int main()
{
	SDL_Init(SDL_INIT_AUDIO);

	SDL_AudioSpec desired, obtained;

	desired.freq = Frequency;
	desired.format = AUDIO_S16SYS;
	desired.channels = 1;
	desired.samples = 4096;
	desired.callback = audio_callback;

	SDL_OpenAudio(&desired, &obtained);

	printf("Frequency: %d\nSamples: %d\n", obtained.freq, obtained.samples*2);

	total_length = Frequency*2;
	time = 0;

	SDL_PauseAudio(0);

	while (total_length > 0)
		SDL_Delay(100);

	SDL_CloseAudio();
	SDL_Quit();

	return 0;
}

