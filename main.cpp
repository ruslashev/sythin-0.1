#include <math.h>
#include "audio_master.hpp"
#include "gui.hpp"

void actual_main()
{
	GUI gui;

	const int RequestedSampleFrequency = 44100;
	const int RequestedSamples = 4096;
	AudioMaster audio_master(RequestedSampleFrequency, RequestedSamples);

	gui.audio_master = &audio_master;
	gui.sampFreq = audio_master.SampleFrequency;
	gui.samples = audio_master.Samples;

	audio_master.Unpause();
	gui.MainLoop();
}

int main()
{
	try {
		actual_main();
	} catch (std::exception &e) {
		printf("Error: %s\n", e.what());
		return 1;
	}

	return 0;
}

