#include "main.h"
#include "samp.h"
#include "java.h"
#include "settings.h"
#include "backtrace.h"

JavaVM* g_VM = nullptr;

uintptr_t g_saAddr = 0x00;
uintptr_t g_sampAddr = 0x00;

void* g_saHandle = nullptr;
void* g_sampHandle = nullptr;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	LOGI("Initializing Alyn_SAMPMOBILE library... Build time: " __DATE__ " " __TIME__);
	g_VM = vm;
	return JNI_VERSION_1_4;
}

__attribute__((constructor)) void constructor()
{
	struct sigaction sig_action{};
	sig_action.sa_sigaction = [](int signal, siginfo_t* info, void* ctx) {
		dump_register(signal, info, ctx);
		dump_stack(2);
		exit(signal);
	};

	sigemptyset(&sig_action.sa_mask);
	sig_action.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &sig_action, nullptr);

	g_saHandle = dlopen("/data/data/com.newgamersrp.game/lib/libGTASA.so", RTLD_LAZY);
	g_sampHandle = dlopen("/data/data/com.newgamersrp.game/lib/libsamp.so", RTLD_LAZY);

	if (!g_saHandle) {
		LOGE("Failed to open SA library!");
		return;
	}

	if (!g_sampHandle) {
		LOGE("Failed to open SAMP library!");
		return;
	}

	g_saAddr = Memory::getLibraryAddress("libGTASA.so");
	g_sampAddr = Memory::getLibraryAddress("libsamp.so");

	if (!g_saAddr) {
		LOGE("SA library address not found!");
		return;
	}

	if (!g_sampAddr) {
		LOGE("SAMP library address not found!");
		return;
	}

	SAMP::initialize();

	pthread_t pthread;
	pthread_create(&pthread, nullptr, SAMP::mainThread, nullptr);
}
