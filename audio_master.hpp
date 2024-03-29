#ifndef AUDIO_MASTER
#define AUDIO_MASTER

#include <SDL2/SDL_audio.h>

#include <string>
#include <vector>
#include <deque>

#include "utils.hpp"

class Wave
{
	unsigned int _bytes_per_period;
public:
	std::string Name;
	unsigned int Phase;
	unsigned int Frequency;
	bool Playing;

	std::deque<int> values;

	int f();
	void Update();

	Wave(unsigned int SampleFrequency, std::string nName,
			unsigned int nFreq);
};

class AudioMaster
{
public:
	unsigned int SampleFrequency;
	unsigned int Samples;
	std::vector<Wave> audio_waves;

	AudioMaster(unsigned int requested_samplefrequency,
			unsigned int requested_samples);

	void Pause() {
		SDL_PauseAudio(1);
	}
	void Unpause() {
		SDL_PauseAudio(0);
	}
};

void AudioCallback(void *userdata, Uint8 *stream, int length);

#endif

