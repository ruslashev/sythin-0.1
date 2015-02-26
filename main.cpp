#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <stdexcept>

typedef signed short MY_TYPE;
#define FORMAT RTAUDIO_SINT16
#define SCALE  32767.0

/*
typedef S24 MY_TYPE;
#define FORMAT RTAUDIO_SINT24
#define SCALE  8388607.0

typedef signed long MY_TYPE;
#define FORMAT RTAUDIO_SINT32
#define SCALE  2147483647.0

typedef float MY_TYPE;
#define FORMAT RTAUDIO_FLOAT32
#define SCALE  1.0

typedef double MY_TYPE;
#define FORMAT RTAUDIO_FLOAT64
#define SCALE  1.0
*/

#if defined( __WINDOWS_ASIO__ ) || defined( __WINDOWS_DS__ ) || defined( __WINDOWS_WASAPI__ )
  #include <windows.h>
  #define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants
  #include <unistd.h>
  #define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

#define BASE_RATE 0.005
#define TIME   1.0

void error_callback(RtAudioError::Type type, const std::string &error_text)
{
	if (type == RtAudioError::WARNING)
		printf("%sWarning: %s%s\n", "\x1b[33m", error_text.c_str(), "\x1b[30m");
	else if (type != RtAudioError::WARNING) {
		printf("%sError: %s%s\n", "\x1b[31m", error_text.c_str(), "\x1b[30m");
		throw;
	}
}

const unsigned int channels = 1;
RtAudio::StreamOptions options;
unsigned int frame_counter = 0;

int saw(void *out_buffer, void *in_buffer, unsigned int nbuffer_frames,
		double streamTime, RtAudioStreamStatus status, void *data)
{
	extern const unsigned int channels;
	MY_TYPE *buffer = (MY_TYPE*)out_buffer;
	double *lastValues = (double*)data;

	if (status)
		std::cout << "Stream underflow detected!" << std::endl;

	for (unsigned int i = 0; i < nbuffer_frames; i++) {
		for (unsigned int j = 0; j < channels; j++) {
			*buffer++ = (MY_TYPE)(lastValues[j]*SCALE*0.5);
			lastValues[j] += BASE_RATE*(j + 1 + (j*0.1));
			if (lastValues[j] >= 1.0)
				lastValues[j] -= 2.0;
		}
	}

	frame_counter += nbuffer_frames;
	return 0;
}

int main(int argc, char *argv[])
{
	RtAudio dac;
	double *data = (double*)calloc(channels, sizeof(double));

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

		dac.openStream(&params, NULL, FORMAT, freq, &buffer_frames,
				&saw, (void*)data, &options, &error_callback);
		dac.startStream();

		//std::cout << "Stream latency = " << dac.getStreamLatency() << "\n" << std::endl;
		std::cout << "\nPlaying... press <enter> to quit (buffer size = " << buffer_frames << ").\n";
		char input;
		std::cin.get(input);

		dac.stopStream();
	} catch (RtAudioError& e) {
		e.printMessage();
		if (dac.isStreamOpen())
			dac.closeStream();
		free(data);
	} catch (std::bad_alloc &e) {
		printf("%sMemory Allocation Error: %s%s\n", "\x1b[31m", e.what(),
				"\x1b[30m");
	} catch (std::exception &e) {
		printf("%sError: %s%s\n", "\x1b[31m", e.what(), "\x1b[30m");
	}

	return 0;
}

