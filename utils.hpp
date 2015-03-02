#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdarg>
#include <fstream>
#include <stdexcept>
#include <string>

const struct {
	const char *Normal;
	const char *Red;
	const char *Yellow;
} AnsiColors {
	"\x1b[0m",
	"\x1b[31m",
	"\x1b[33m"
};

void assertf(bool condition, const char *format, ...);

void errorf(const char *format, ...);

void warningf(const char *format, ...);

struct File {
	size_t size;
	std::string contents;
};
File readFile(std::string path);

#endif

