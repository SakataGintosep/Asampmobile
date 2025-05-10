#pragma once

#include <string>

class Encoding {
public:
	static char* cp2utf(const char* s);

private:
	static void cp2utf_(char *out, const char *in, unsigned int len);
};