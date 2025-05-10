#include <dlfcn.h>
#include <android/log.h>

#include "bass.h"

int (* BASS_Init)(uint32_t, uint32_t, uint32_t);
int (* BASS_Free)();
int (* BASS_SetConfigPtr)(uint32_t, const char*);
int (* BASS_SetConfig)(uint32_t, uint32_t);
int (* BASS_ChannelStop)(uint32_t);
int (* BASS_StreamCreateURL)(char*, uint32_t, uint32_t, uint32_t);
int (* BASS_StreamCreate)(uint32_t, uint32_t, uint32_t, STREAMPROC*, void*);
int (* BASS_ChannelPlay)(uint32_t, bool);
int (* BASS_ChannelPause)(uint32_t);
int* BASS_ChannelGetTags;
int* BASS_ChannelSetSync;
int* BASS_StreamGetFilePosition;
int (* BASS_StreamFree)(uint32_t);
int (* BASS_ErrorGetCode)();
int (* BASS_Set3DFactors)(float, float, float);
int (* BASS_Set3DPosition)(const BASS_3DVECTOR*, const BASS_3DVECTOR*, const BASS_3DVECTOR*,
		const BASS_3DVECTOR*);
int (* BASS_Apply3D)();
int (* BASS_ChannelSetFX)(uint32_t, HFX);
int (* BASS_ChannelRemoveFX)(uint32_t, HFX);
int (* BASS_FXSetParameters)(HFX, const void*);
int (* BASS_IsStarted)();
int (* BASS_RecordGetDeviceInfo)(uint32_t, BASS_DEVICEINFO*);
int (* BASS_RecordInit)(int);
int (* BASS_RecordGetDevice)();
int (* BASS_RecordFree)();
int (* BASS_RecordStart)(uint32_t, uint32_t, uint32_t, RECORDPROC*, void*);
int (* BASS_ChannelSetAttribute)(uint32_t, uint32_t, float);
int (* BASS_ChannelGetData)(uint32_t, void*, uint32_t);
int (* BASS_RecordSetInput)(int, uint32_t, float);
int (* BASS_StreamPutData)(uint32_t, const void*, uint32_t);
int (* BASS_ChannelSetPosition)(uint32_t, uint64_t, uint32_t);
int (* BASS_ChannelIsActive)(uint32_t);
int (* BASS_ChannelSlideAttribute)(uint32_t, uint32_t, float, uint32_t);
int (* BASS_ChannelSet3DAttributes)(uint32_t, int, float, float, int, int, float);
int (* BASS_ChannelSet3DPosition)(uint32_t, const BASS_3DVECTOR*, const BASS_3DVECTOR*,
		const BASS_3DVECTOR*);
int (* BASS_SetVolume)(float);

void loadBassLibrary()
{
	__android_log_print(ANDROID_LOG_DEBUG, "BASS", "Loading BASS library..");

	void* v0 = dlopen("/data/data/com.newgamersrp.game/lib/libBASS.so", RTLD_LAZY);
	if (!v0) {
		__android_log_print(ANDROID_LOG_DEBUG, "BASS", "%s", dlerror());
	}

	BASS_Init = (int (*)(uint32_t, uint32_t, uint32_t)) dlsym(v0, "BASS_Init");
	BASS_Free = (int (*)()) dlsym(v0, "BASS_Free");
	BASS_SetConfigPtr = (int (*)(uint32_t, const char*)) dlsym(v0, "BASS_SetConfigPtr");
	BASS_SetConfig = (int (*)(uint32_t, uint32_t)) dlsym(v0, "BASS_SetConfig");
	BASS_ChannelStop = (int (*)(uint32_t)) dlsym(v0, "BASS_ChannelStop");
	BASS_StreamCreateURL = (int (*)(char*, uint32_t, uint32_t, uint32_t)) dlsym(v0,
			"BASS_StreamCreateURL");
	BASS_StreamCreate = (int (*)(uint32_t, uint32_t, uint32_t, STREAMPROC*, void*)) dlsym(v0,
			"BASS_StreamCreate");
	BASS_ChannelPlay = (int (*)(uint32_t, bool)) dlsym(v0, "BASS_ChannelPlay");
	BASS_ChannelPause = (int (*)(uint32_t)) dlsym(v0, "BASS_ChannelPause");
	BASS_ChannelGetTags = (int*) dlsym(v0, "BASS_ChannelGetTags");
	BASS_ChannelSetSync = (int*) dlsym(v0, "BASS_ChannelSetSync");
	BASS_StreamGetFilePosition = (int*) dlsym(v0, "BASS_StreamGetFilePosition");
	BASS_StreamFree = (int (*)(uint32_t)) dlsym(v0, "BASS_StreamFree");
	BASS_ErrorGetCode = (int (*)()) dlsym(v0, "BASS_ErrorGetCode");
	BASS_Set3DFactors = (int (*)(float, float, float)) dlsym(v0, "BASS_Set3DFactors");
	BASS_Set3DPosition = (int (*)(const BASS_3DVECTOR*, const BASS_3DVECTOR*, const BASS_3DVECTOR*,
			const BASS_3DVECTOR*)) dlsym(v0, "BASS_Set3DPosition");
	BASS_Apply3D = (int (*)()) dlsym(v0, "BASS_Apply3D");
	BASS_ChannelSetFX = (int (*)(uint32_t, HFX)) dlsym(v0, "BASS_ChannelSetFX");
	BASS_ChannelRemoveFX = (int (*)(uint32_t, HFX)) dlsym(v0, "BASS_ChannelRemoveFX");
	BASS_FXSetParameters = (int (*)(HFX, const void*)) dlsym(v0, "BASS_FXSetParameters");
	BASS_IsStarted = (int (*)()) dlsym(v0, "BASS_IsStarted");
	BASS_RecordGetDeviceInfo = (int (*)(uint32_t, BASS_DEVICEINFO*)) dlsym(v0,
			"BASS_RecordGetDeviceInfo");
	BASS_RecordInit = (int (*)(int)) dlsym(v0, "BASS_RecordInit");
	BASS_RecordGetDevice = (int (*)()) dlsym(v0, "BASS_RecordGetDevice");
	BASS_RecordFree = (int (*)()) dlsym(v0, "BASS_RecordFree");
	BASS_RecordStart = (int (*)(uint32_t, uint32_t, uint32_t, RECORDPROC*, void*)) dlsym(v0,
			"BASS_RecordStart");
	BASS_ChannelSetAttribute = (int (*)(uint32_t, uint32_t, float)) dlsym(v0,
			"BASS_ChannelSetAttribute");
	BASS_ChannelGetData = (int (*)(uint32_t, void*, uint32_t)) dlsym(v0, "BASS_ChannelGetData");
	BASS_RecordSetInput = (int (*)(int, uint32_t, float)) dlsym(v0, "BASS_RecordSetInput");
	BASS_StreamPutData = (int (*)(uint32_t, const void*, uint32_t)) dlsym(v0, "BASS_StreamPutData");
	BASS_ChannelSetPosition = (int (*)(uint32_t, uint64_t, uint32_t)) dlsym(v0,
			"BASS_ChannelSetPosition");
	BASS_ChannelIsActive = (int (*)(uint32_t)) dlsym(v0, "BASS_ChannelIsActive");
	BASS_ChannelSlideAttribute = (int (*)(uint32_t, uint32_t, float, uint32_t)) dlsym(v0,
			"BASS_ChannelSlideAttribute");
	BASS_ChannelSet3DAttributes = (int (*)(uint32_t, int, float, float, int, int, float)) dlsym(v0,
			"BASS_ChannelSet3DAttributes");
	BASS_ChannelSet3DPosition = (int (*)(uint32_t, const BASS_3DVECTOR*, const BASS_3DVECTOR*,
			const BASS_3DVECTOR*)) dlsym(v0, "BASS_ChannelSet3DPosition");
	BASS_SetVolume = (int (*)(float)) dlsym(v0, "BASS_SetVolume");
}
