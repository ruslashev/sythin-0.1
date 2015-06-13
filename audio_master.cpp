#include "audio_master.hpp"

AudioMaster::AudioMaster(unsigned int requested_frequency,
			unsigned int requested_samples)
{
	SDL_AudioSpec requested, obtained;

	requested.freq = requested_frequency;
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

	audio_waves.push_back(AudioWave(obtained.freq, "sine wave", 1000, 127));
	audio_waves.push_back(AudioWave(obtained.freq, "sine wave low freq", 400, 1));
}

void AudioCallback(void *userdata, Uint8 *stream, int length)
{
	AudioMaster *audio_master = (AudioMaster*)userdata;
	for (int i = 0; i < length; i++) {
		int value = 0;
		for (auto &wave : audio_master->audio_waves) {
			if (wave.Playing) {
				value += wave.f();
				wave.step();
				wave.values.push_front(wave.f());
				wave.values.pop_back();
				printf("%d ", value);
			}
		}
		if (value > 127)
			value = 127;
		if (value < -128)
			value = -128;
		stream[i] = value;
	}
}

AudioWave::AudioWave(unsigned int SampleFrequency, std::string nName,
		unsigned int nFrequency, unsigned int nAmplitude)
{
	Name = nName;
	Phase = 0;
	Frequency = nFrequency;
	Amplitude = nAmplitude;
	Playing = false;

	values.resize(400);
	for (int i = 0; i < 400; i++)
		values[i] = 0;

	_bytes_per_period = SampleFrequency / Frequency;
}

int AudioWave::f()
{
	return Amplitude*sin(Phase*2*M_PI / _bytes_per_period);
}

void AudioWave::step()
{
	Phase++;
	Phase %= _bytes_per_period;
}
