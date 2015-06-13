#include "utils.hpp"

static const size_t outputMaxLen = 512;

void assertf(bool condition, const char *format, ...)
{
#ifndef RELEASE
	if (!condition) {
		char output[outputMaxLen];
		va_list args;
		va_start(args, format);
		vsnprintf(output, outputMaxLen, format, args);
		va_end(args);
		throw std::runtime_error(output);
	}
#endif
}

void errorf(const char *format, ...)
{
	char output[outputMaxLen];
	va_list args;
	va_start(args, format);
	vsnprintf(output, outputMaxLen, format, args);
	va_end(args);
	throw std::runtime_error(output);
}

void warningf(const char *format, ...)
{
	char output[outputMaxLen];
	const int yellowWarningLen = snprintf(output, outputMaxLen,
			" %sWarning%s ", AnsiColors.Yellow, AnsiColors.Normal);
	va_list args;
	va_start(args, format);
	vsnprintf(output+yellowWarningLen, outputMaxLen-yellowWarningLen, format, args);
	va_end(args);
	fputs(output, stdout);
	fputc('\n', stdout);
}

File readFile(std::string path)
{
	std::ifstream ifs(path.c_str(), std::ifstream::binary);
	assertf(ifs.good(), "Failed to open file \"%s\" for reading.", path.c_str());

	ifs.seekg(0, ifs.end);
	const size_t filesize = ifs.tellg();
	ifs.seekg(0, ifs.beg);

	// size sanity check
	const size_t size_sane = 4096;
	if (filesize > size_sane)
		errorf("File \"%s\" is too big: %d bytes out of %d\n"
				"currently being a sane maximum allocated for a file.",
				path.c_str(),
				filesize,
				size_sane);

	char *buffer = new char [filesize+1];
	ifs.read(buffer, filesize);
	buffer[filesize] = '\0';

	int read = ifs.gcount();
	double percent = 100.*read/(double)filesize;
	assertf(ifs.good(), "Only %d characters (%.2f%%) read from file \"%s\"",
			read, percent, path.c_str());

	ifs.close();

	std::string buf_str(buffer);

	delete [] buffer;

	return File { filesize+1, buf_str };
}

