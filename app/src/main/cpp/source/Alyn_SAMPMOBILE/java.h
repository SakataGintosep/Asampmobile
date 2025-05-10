#pragma once

#include <jni.h>

class Java {
public:
	Java(JNIEnv* env, jobject sampObj, jobject uiObj, jobject assetMgr);
	JNIEnv* getEnv();

	jobject assetManager() const { return m_assetManager; }

	void showButtonPanel(bool show);
	void showVoice(bool show);
	void showKeyboard(bool show);

	void showWantedLevel(bool show);
	void setWantedLevel(int level);

	void showDialog(int dialog_id, int dialog_style, char* title, char* message, char* button1, char* button2);
	void showDialogWithoutReset(bool show);

	void showScoreboard(bool show);
	void setScoreboardStats(char* server, int players);
	void addScoreboardPlayer(int id, char* name, int score, int ping, char* color);

private:
	jobject m_sampActivity;
	jobject m_uiActivity;
	jobject m_assetManager;

	jmethodID m_showButtonPanel;
	jmethodID m_showVoice;
	jmethodID m_showKeyboard;

	jmethodID m_showWantedLevel;
	jmethodID m_setWantedLevel;

	jmethodID m_showDialog;
	jmethodID m_showDialogWithoutReset;

	jmethodID m_showScoreboard;
	jmethodID m_setScoreboardStats;
	jmethodID m_addScoreboardPlayer;
};

extern Java* g_java;
