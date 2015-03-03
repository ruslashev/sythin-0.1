#include <math.h>
#include "audio_master.hpp"
#include "gui.hpp"

void actual_main()
{
	GUI gui;

	const int RequestedFrequency = 100500;
	const int RequestedSamples = 4096;
	AudioMaster audio_master(RequestedFrequency, RequestedSamples);

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

