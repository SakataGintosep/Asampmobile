#pragma once

#include <unwind.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <string>
#include <array>

void dump_register(int signal, siginfo_t* info, void* ctx);
void dump_stack(int ignoreDepth = 0, int maxDepth = 31);
