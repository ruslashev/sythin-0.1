#include "audio_master.hpp"

AudioMaster::AudioMaster(unsigned int requested_samplefrequency,
			unsigned int requested_samples)
{
	SDL_AudioSpec requested, obtained;

	requested.freq = requested_samplefrequency;
	requested.format = AUDIO_S8;
	requested.channels = 1;
	requested.samples = requested_samples;
	requested.callback = AudioCallback;
	requested.userdata = this;

	int status = SDL_OpenAudio(&requested, &obtained);
	assertf(status == 0,
			"Failed to open the audio device "
			"or couldn't set up the audio thread");

	SampleFrequency = obtained.freq;
	Samples = obtained.samples;

	audio_waves.push_back(Wave(SampleFrequency, "Simple sine", 1000));
}

Wave::Wave(unsigned int SampleFrequency, std::string nName,
		unsigned int nFrequency)
{
	Name = nName;
	Phase = 0;
	Frequency = nFrequency;
	Playing = false;

	values.resize(400);
	for (int i = 0; i < 400; i++)
		values[i] = 0;

	_bytes_per_period = SampleFrequency / Frequency;
}

int Wave::f()
{
	return 128*sin(Phase*2*M_PI / _bytes_per_period);
}

void Wave::Update()
{
	Phase++;
	Phase %= _bytes_per_period;
}

void AudioCallback(void *userdata, Uint8 *stream, int length)
{
	AudioMaster *audio_master = (AudioMaster*)userdata;
	printf("%d\n", length);
	for (int i = 0; i < length; i++) {
		int value = 0;
		for (auto &wave : audio_master->audio_waves) {
			if (wave.Playing) {
				value += wave.f();
				wave.Update();
				wave.values.push_front(wave.f());
				wave.values.pop_back();
			} else {
				wave.Phase = 0;
			}
		}
		if (value > 127)
			value = 127;
		if (value < -128)
			value = -128;
		stream[i] = value;
	}
}

