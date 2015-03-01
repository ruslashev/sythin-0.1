#include "audio_master.hpp"

unsigned int SampleFrequency = 0;
unsigned int phase = 0;

AudioMaster::AudioMaster(unsigned int requested_frequency,
			unsigned int requested_samples)
{
	SDL_AudioSpec requested, obtained;

	requested.freq = requested_frequency;
	requested.format = AUDIO_S8;
	requested.channels = 1;
	requested.samples = requested_samples;
	requested.callback = AudioCallback;
	requested.userdata = NULL;

	int status = SDL_OpenAudio(&requested, &obtained);

	if (status) {
		throw std::runtime_error(
				"Error: Failed to open the audio device "
				"or couldn't set up the audio thread\n");
	}

	printf("Frequency requested: %d | Frequency obtained: %d\n",
			requested.freq, obtained.freq);

	printf("Samples requested: %d | Samples obtained: %d\n",
			requested.samples, obtained.samples);

	SampleFrequency = obtained.freq;
}

void AudioCallback(void *userdata, Uint8 *stream, int length)
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

