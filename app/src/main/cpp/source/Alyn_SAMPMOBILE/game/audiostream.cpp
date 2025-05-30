#include <memory>
#include <unistd.h>
#include <pthread.h>
#include <bass/bass.h>

#include "audiostream.h"
#include "../main.h"
#include "../samp.h"

HSTREAM bassStream = 0;
char g_szAudioStreamUrl[256 + 1];
float g_fAudioStreamX;
float g_fAudioStreamY;
float g_fAudioStreamZ;
float g_fAudioStreamRadius;
bool g_audioStreamUsePos;
bool g_bAudioStreamStop;
bool g_bAudioStreamThreadWorked;
bool g_bAudioStreamPlaying;

void* audioStreamThread(void*)
{
	g_bAudioStreamThreadWorked = true;
	if (bassStream) {
		BASS_ChannelStop(bassStream);
		bassStream = 0;
	}

	bassStream = BASS_StreamCreateURL(g_szAudioStreamUrl, 0, 0x940000, 0);
	BASS_ChannelPlay(bassStream, 0);

	//BASS_ChannelSetSync(bassStream, 4, 0, 0, 0);
	//BASS_ChannelSetSync(bassStream, 12, 0, 0, 0);

	while (!g_bAudioStreamStop) {
		// ..
		usleep(2000);
	}

	BASS_ChannelStop(bassStream);
	bassStream = 0;
	g_bAudioStreamThreadWorked = false;
	pthread_exit(nullptr);
}

AudioStream::AudioStream()
{
	m_bInited = false;
	g_bAudioStreamPlaying = false;
}

bool AudioStream::Initialize()
{
	bassStream = 0;

	BASS_Free();

	if (!BASS_Init(-1, 44100, 0)) return false;

	BASS_SetConfigPtr(16, "SA-MP/0.3");
	//BASS_SetConfig(5, ) volume
	BASS_SetConfig(21, 1);            // BASS_CONFIG_NET_PLAYLIST
	BASS_SetConfig(11, 10000);        // BASS_CONFIG_NET_TIMEOUT

	m_bInited = true;
	return true;
}

void AudioStream::Process()
{
	if (g_bAudioStreamThreadWorked) {
		// ..
		if (SAMP::paused()) {
			BASS_SetConfig(5, 0);
		}
		else {
			BASS_SetConfig(5, 5000);
		}
	}
}

bool AudioStream::Play(const char* szUrl, float fX, float fY, float fZ, float fRadius, bool bUsePos)
{
	LOGI("Play: %s", szUrl);

	if (!m_bInited) return false;
	Stop(true);

	if (bassStream) {
		BASS_ChannelStop(bassStream);
		bassStream = 0;
	}

	memset(g_szAudioStreamUrl, 0, sizeof(g_szAudioStreamUrl));
	strncpy(g_szAudioStreamUrl, szUrl, 256);

	g_fAudioStreamX = fX;
	g_fAudioStreamY = fY;
	g_fAudioStreamZ = fZ;
	g_fAudioStreamRadius = fRadius;
	g_audioStreamUsePos = bUsePos;

	g_bAudioStreamStop = false;
	g_bAudioStreamPlaying = true;

	pthread_t thread;
	pthread_create(&thread, nullptr, audioStreamThread, nullptr);
	return true;
}

bool AudioStream::Stop(bool bWaitThread)
{
	LOGI("Stop: %s", g_szAudioStreamUrl);
	if (!m_bInited || !g_bAudioStreamThreadWorked) {
		return false;
	}

	g_bAudioStreamStop = true;
	g_bAudioStreamPlaying = false;

	if (bWaitThread) {
		while (g_bAudioStreamThreadWorked)
			usleep(200);
	}

	BASS_StreamFree(bassStream);
	bassStream = 0;
	return true;
}
