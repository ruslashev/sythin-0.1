#include <math.h>
#include "audio_master.hpp"
#include "gui.hpp"

int main()
{
	try {
		GUI gui;

		const int RequestedFrequency = 100500;
		const int RequestedSamples = 4096;
		AudioMaster audio_master(RequestedFrequency, RequestedSamples);

		// audio_master.Unpause();
		gui.MainLoop();
	} catch (std::exception &e) {
		printf("Error: %s\n", e.what());
		return 1;
	}

	return 0;
}

