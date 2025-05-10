#include "java.h"
#include "main.h"
#include "samp.h"

Java* g_java = nullptr;

extern JavaVM* g_VM;

extern UI* pUI;

/* Java */

Java::Java(JNIEnv* env, jobject sampObj, jobject uiObj, jobject assetMgr)
{
	LOGI("Initializing java..");

	if (!sampObj || !uiObj || !assetMgr) {
		return;
	}

	m_sampActivity = env->NewGlobalRef(sampObj);
	m_uiActivity = env->NewGlobalRef(uiObj);
	m_assetManager = env->NewGlobalRef(assetMgr);

	jclass sampClass = env->GetObjectClass(sampObj);
	if (!sampClass) {
		LOGI("SAMP class not found");
		return;
	}

	jclass uiClass = env->GetObjectClass(uiObj);
	if (!uiClass) {
		LOGI("UI class not found");
		return;
	}

	m_showVoice = env->GetMethodID(uiClass, "showVoice", "(Z)V");
	m_showButtonPanel = env->GetMethodID(uiClass, "showButtonPanel", "(Z)V");
	m_showKeyboard = env->GetMethodID(uiClass, "showKeyboard", "(Z)V");

	m_showWantedLevel = env->GetMethodID(uiClass, "showWantedLevel", "(Z)V");
	m_setWantedLevel = env->GetMethodID(uiClass, "setWantedLevel", "(I)V");

	m_showDialog = env->GetMethodID(uiClass, "showDialog", "(IILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	m_showDialogWithoutReset = env->GetMethodID(uiClass, "showDialogWithoutReset", "(Z)V");

	m_showScoreboard = env->GetMethodID(uiClass, "showScoreboard", "(Z)V");
	m_setScoreboardStats = env->GetMethodID(uiClass, "setScoreboardStats", "(Ljava/lang/String;I)V");
	m_addScoreboardPlayer = env->GetMethodID(uiClass, "addScoreboardPlayer", "(ILjava/lang/String;IILjava/lang/String;)V");

	env->DeleteLocalRef(sampClass);
	env->DeleteLocalRef(uiClass);
}

JNIEnv* Java::getEnv()
{
	if (!g_VM) {
		LOGI("No java vm");
		return nullptr;
	}

	JNIEnv* env;
	g_VM->GetEnv((void**) &env, JNI_VERSION_1_4);

	return env;
}

void Java::showButtonPanel(bool show)
{
	JNIEnv* env = getEnv();

	if (!env) {
		LOGI("No env");
		return;
	}

	env->CallVoidMethod(m_uiActivity, m_showButtonPanel, show);
}

void Java::showVoice(bool show)
{
	JNIEnv* env = getEnv();

	if (!env) {
		LOGI("No env");
		return;
	}

	env->CallVoidMethod(m_uiActivity, m_showVoice, show);
}

jstring createJString(JNIEnv* env, jclass strClass, jmethodID ctorID, const char* text,
		jstring encoding)
{
	jbyteArray bytes = env->NewByteArray(strlen(text));
	env->SetByteArrayRegion(bytes, 0, strlen(text), (jbyte*) text);
	return (jstring) env->NewObject(strClass, ctorID, bytes, encoding);
}

void Java::showDialog(int dialog_id, int dialog_style, char* title, char* message, char* button1,
		char* button2)
{
	JNIEnv* env = getEnv();

	if (!env) {
		LOGI("No env");
		return;
	}

	jclass strClass = env->FindClass("java/lang/String");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	jstring encoding = env->NewStringUTF("UTF-8");

	jstring jtitle = createJString(env, strClass, ctorID, title, encoding);
	jstring jmessage = createJString(env, strClass, ctorID, message, encoding);
	jstring jbutton1 = createJString(env, strClass, ctorID, button1, encoding);
	jstring jbutton2 = createJString(env, strClass, ctorID, button2, encoding);

	env->CallVoidMethod(m_uiActivity, m_showDialog, dialog_id, dialog_style, jtitle, jmessage,
			jbutton1, jbutton2);
}

void Java::showDialogWithoutReset(bool show)
{
	JNIEnv* env = getEnv();

	if (!env) {
		LOGI("No env");
		return;
	}

	env->CallVoidMethod(m_uiActivity, m_showDialogWithoutReset, show);
}

void Java::showScoreboard(bool show)
{
	JNIEnv* env = getEnv();

	if (!env) {
		LOGI("No env");
		return;
	}

	env->CallVoidMethod(m_uiActivity, m_showScoreboard, show);
}

void Java::setScoreboardStats(char* server, int players)
{
	JNIEnv* env = getEnv();

	if (!env) {
		LOGI("No env");
		return;
	}

	jclass strClass = env->FindClass("java/lang/String");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	jstring encoding = env->NewStringUTF("UTF-8");

	jstring jserver = createJString(env, strClass, ctorID, server, encoding);

	env->CallVoidMethod(m_uiActivity, m_setScoreboardStats, jserver, players);
}

void Java::addScoreboardPlayer(int id, char* name, int score, int ping, char* color)
{
	JNIEnv* env = getEnv();

	if (!env) {
		LOGI("No env");
		return;
	}

	jclass strClass = env->FindClass("java/lang/String");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	jstring encoding = env->NewStringUTF("UTF-8");

	jstring jname = createJString(env, strClass, ctorID, name, encoding);
	jstring jcolor = createJString(env, strClass, ctorID, color, encoding);

	env->CallVoidMethod(m_uiActivity, m_addScoreboardPlayer, id, jname, score, ping, jcolor);
}

void Java::showKeyboard(bool show)
{
	JNIEnv* env = getEnv();

	if (!env) {
		LOGI("No env");
		return;
	}

	env->CallVoidMethod(m_uiActivity, m_showKeyboard, show);
}

void Java::showWantedLevel(bool show)
{
	JNIEnv* env = getEnv();

	if (!env) {
		LOGI("No env");
		return;
	}

	env->CallVoidMethod(m_uiActivity, m_showWantedLevel, show);
}

void Java::setWantedLevel(int level)
{
	JNIEnv* env = getEnv();

	if (!env) {
		LOGI("No env");
		return;
	}

	env->CallVoidMethod(m_uiActivity, m_setWantedLevel, level);
}

bool g_scoreboardVisible = false;

extern "C"
{
JNIEXPORT void JNICALL Java_com_newgamersrp_game_SAMP_initializeSAMP(JNIEnv* env,
		jobject sampObj, jobject uiObj, jobject asset_manager)
{
	LOGI("Java_com_newgamersrp_game_SAMP_initializeSAMP");
	g_java = new Java(env, sampObj, uiObj, asset_manager);
}

JNIEXPORT void JNICALL Java_com_newgamersrp_game_ui_UI_sendButtonPanelKey(JNIEnv* env,
		jobject obj, jint id)
{
	LOGI("Java_com_newgamersrp_game_ui_UI_sendButtonPanelKey");
}

JNIEXPORT void JNICALL Java_com_newgamersrp_game_ui_UI_sendDialogResponse(JNIEnv* env,
		jobject obj, jint dialog_id, jint button_id, jint list_item, jbyteArray str)
{
	LOGI("Java_com_newgamersrp_game_ui_UI_sendDialogResponse");
}

JNIEXPORT void JNICALL Java_com_newgamersrp_game_ui_UI_sendScoreboardClick(JNIEnv* env,
		jobject obj, jint player_id)
{
	LOGI("Java_com_newgamersrp_game_ui_UI_sendScoreboardClick");
}

JNIEXPORT void JNICALL Java_com_newgamersrp_game_ui_UI_keyboardSend(JNIEnv* env, jobject obj,
		jbyteArray str)
{
	LOGI("Java_com_newgamersrp_ui_UI_SAMP_keyboardSend");
}

JNIEXPORT void JNICALL
Java_com_newgamersrp_game_ui_UI_voicePressed(JNIEnv* env, jobject obj)
{
	LOGI("Java_com_newgamersrp_game_ui_UI_voicePressed");

	if (pUI) {
		pUI->voicebutton()->setRecording(!pUI->voicebutton()->recording());
	}
}
}
