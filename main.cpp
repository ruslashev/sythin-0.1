#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <stdexcept>

void error_callback(RtAudioError::Type type, const std::string &error_text)
{
	if (type == RtAudioError::WARNING)
		printf("%sWarning: %s%s\n", "\x1b[33m", error_text.c_str(), "\x1b[30m");
	else if (type != RtAudioError::WARNING)
		throw std::runtime_error(error_text);
}

int saw(void *out_buffer, void *in_buffer, unsigned int buffer_frames,
		double streamTime, RtAudioStreamStatus status, void *data)
{
	extern const unsigned int channels;
	double *buffer = (double*)out_buffer;
	double *lastValues = (double*)data;

	if (status) {
		std::cout << "Stream underflow detected!" << std::endl;
		return 1;
	}

	for (unsigned int i = 0; i < buffer_frames; i++) {
		for (unsigned int j = 0; j < channels; j++) {
			*buffer++ = lastValues[j];

			lastValues[j] += 0.005*(j + 1 + (j*0.1));
			if (lastValues[j] >= 1.0)
				lastValues[j] -= 2.0;
		}
	}

	return 0;
}

const unsigned int channels = 2;
RtAudio::StreamOptions options;

int main(int argc, char *argv[])
{
	RtAudio dac;
	double *data = new double [channels];

	try {
		if (dac.getDeviceCount() < 1)
			throw std::runtime_error("No audio devices found!");
		dac.showWarnings(true);

		unsigned int buffer_frames,
					 freq = 44100,
					 offset = 0;

		buffer_frames = 512;
		RtAudio::StreamParameters params;
		params.deviceId = 2;
		params.nChannels = channels;
		params.firstChannel = offset;

		options.flags = RTAUDIO_HOG_DEVICE | RTAUDIO_SCHEDULE_REALTIME;

		dac.openStream(&params, NULL, RTAUDIO_FLOAT64, freq, &buffer_frames,
				&saw, (void*)data, &options, &error_callback);
		dac.startStream();

		printf("Stream latency = %ld\n", dac.getStreamLatency());
		printf("Playing... press <enter> to quit (buffer size = %d).\n",
				buffer_frames);
		char input;
		std::cin.get(input);

		dac.stopStream();
	} catch (RtAudioError& e) {
		e.printMessage();
	} catch (std::bad_alloc &e) {
		printf("%sMemory Allocation Error: %s%s\n", "\x1b[31m", e.what(),
				"\x1b[30m");
	} catch (std::exception &e) {
		printf("%sError: %s%s\n", "\x1b[31m", e.what(), "\x1b[30m");
	}

	if (dac.isStreamOpen())
		dac.closeStream();

	delete data;

	return 0;
}

