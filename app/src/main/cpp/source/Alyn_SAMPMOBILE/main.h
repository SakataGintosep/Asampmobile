#include <jni.h>
#include <android/log.h>
#include <chrono>
#include <pthread.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <dlfcn.h>
#include <unordered_map>
#include <dirent.h>

#define LOG_TAG "Alyn_SAMPMOBILE"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#include <bass/bass.h>
#include <memory/memory.h>
#include <encoding/encoding.h>
#include <obfuscator/obfuscator.h>

#define SA_Addr(addr) (g_saAddr + addr)
#define SAMP_Addr(addr) (g_sampAddr + addr)

#define SA_Symbol(sym) dlsym(g_saHandle, sym)
#define SAMP_Symbol(sym) dlsym(g_sampHandle, sym)

extern uintptr_t g_saAddr;
extern uintptr_t g_sampAddr;

extern void* g_saHandle;
extern void* g_sampHandle;
