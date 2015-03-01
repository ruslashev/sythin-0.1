#ifndef AUDIO_MASTER
#define AUDIO_MASTER

#include <SDL2/SDL_audio.h>
#include <stdexcept>

class AudioMaster
{
public:
	unsigned int Frequency;
	unsigned int Samples;
	AudioMaster(unsigned int requested_frequency,
			unsigned int requested_samples);
};

void AudioCallback(void *userdata, Uint8 *stream, int length);

#endif

