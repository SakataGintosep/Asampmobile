package com.arizona.game;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;

import com.newgamersrp.game.SAMP;
import com.newgamersrp.game.ui.UI;

import java.nio.charset.StandardCharsets;

public class GTASA extends SAMP implements UI.Listener {
    private static final String TAG = "GTASAMainClass";
    private boolean isAutocompleteEnabled = true;
    private boolean isInit = false;
    private boolean isNewInterface = false;
    private boolean isNewKeyboard = true;
    private boolean show_fps = false;

    private native void InitSetting(boolean z, int i, boolean z2);

    private native void OnInputEnd(String str);

    private native void OnKeyboardClosed();

    private native void clicked(int i, int i2, int i3, byte[] bArr);

    private native void onDialogResponse(int i, int i2, int i3, byte[] bArr);

    private native void sliderValueChanged(int i, int i2, int i3);

    private native void switchStatusChanged(int i, int i2, boolean z);

    private native void viewShownStatusChanged(int i, boolean z);

    public native void InitModloaderConfig(int i);

    public native void initSAMP();

    public native void main();

    public void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        this.isNewInterface = false;
        this.isNewKeyboard = getSharedPreferences("samp_settings", Context.MODE_PRIVATE).getBoolean("system_keyboard", true);
        this.show_fps = getSharedPreferences("samp_settings", Context.MODE_PRIVATE).getBoolean("display_fps", false);
        InitModloaderConfigWrapper(getSharedPreferences("samp_settings", Context.MODE_PRIVATE).getBoolean("modloader", false) ? 2 : 0); // 2 textures/models | 1 textures | 0 off
        initSAMPWrapper();
    }

    public boolean onTouchEvent(MotionEvent motionEvent) {
        int pointerCount = motionEvent.getPointerCount();
        int i = 0;
        int i2 = 0;
        int i3 = 0;
        int i4 = 0;
        int i5 = 0;
        int i6 = 0;
        int i7 = 0;
        int i8 = 0;
        for (int i9 = 0; i9 < pointerCount; i9++) {
            int pointerId = motionEvent.getPointerId(i9);
            if (pointerId == 0) {
                i = (int) motionEvent.getX(i9);
                i2 = (int) motionEvent.getY(i9);
            } else if (pointerId == 1) {
                i3 = (int) motionEvent.getX(i9);
                i4 = (int) motionEvent.getY(i9);
            } else if (pointerId == 2) {
                i5 = (int) motionEvent.getX(i9);
                i6 = (int) motionEvent.getY(i9);
            } else if (pointerId == 3) {
                i7 = (int) motionEvent.getX(i9);
                i8 = (int) motionEvent.getY(i9);
            }
        }
        multiTouchEvent4(motionEvent.getActionMasked(), motionEvent.getPointerId(motionEvent.getActionIndex()), i, i2, i3, i4, i5, i6, i7, i8, motionEvent);
        return true;
    }

    public String getPlayerNickname() {
        return getSharedPreferences("samp_settings", Context.MODE_PRIVATE).getString("nick_name", "samp_player");
    }

    public void destroyDialog() {

    }

    public void inputFinished(String str, int i) {

    }

    public void requestInput(int i, boolean z, String str) {

    }

    private void OnInputEndWrapper(String str) {
        try {
            OnInputEnd(str);
        } catch (UnsatisfiedLinkError e) {
            Log.w(TAG, "Unable to call native method", e);
        }
    }

    private void InitSettingWrapper(boolean z, int i, boolean z2) {
        try {
            InitSetting(z, i, z2);
        } catch (UnsatisfiedLinkError e) {
            Log.w(TAG, "Unable to call native method", e);
        }
    }

    private void OnKeyboardClosedWrapper() {
        try {
            OnKeyboardClosed();
        } catch (UnsatisfiedLinkError e) {
            Log.w(TAG, "Unable to call native method", e);
        }
    }

    public void initSAMPWrapper() {
        try {
            initSAMP();
        } catch (UnsatisfiedLinkError e) {
            Log.w(TAG, "Unable to call native method", e);
        }
    }

    public void mainWrapper() {
        try {
            main();
        } catch (Exception e) {
            Log.w(TAG, "Unable to call native method", e);
        }
    }

    public void InitModloaderConfigWrapper(int i) {
        try {
            InitModloaderConfig(i);
        } catch (UnsatisfiedLinkError e) {
            Log.w(TAG, "Unable to call native method", e);
        }
    }

    public void sliderValueChangedWrapper(int i, int i2, int i3) {
        try {
            sliderValueChanged(i, i2, i3);
        } catch (Exception e) {
            Log.w(TAG, "sliderValueChangedWrapper: failed to call native method", e);
        }
    }

    public void onDialogResponseWrapper(int i, int i2, int i3, byte[] bArr) {
        try {
            onDialogResponse(i, i2, i3, bArr);
        } catch (Exception e) {
            Log.w(TAG, "onDialogResponseWrapper: failed to call native method", e);
        }
    }

    public void clickedWrapper(int i, int i2, int i3, byte[] bArr) {
        try {
            clicked(i, i2, i3, bArr);
        } catch (Exception e) {
            Log.w(TAG, "Exception during call clicked, uiID:" + i + ", buttonID: " + i2 + ", subID" + i3 + ", payload:" + new String(bArr), e);
        }
    }

    public void clickedWrapper(int i, int i2, int i3) {
        try {
            clicked(i, i2, i3, "".getBytes(StandardCharsets.UTF_8));
        } catch (Exception e) {
            Log.w(TAG, "Exception during call clicked, uiID:" + i + ", buttonID: " + i2 + ", subID" + i3, e);
        }
    }

    public void switchStatusChangedWrapper(int i, int i2, boolean z) {
        try {
            switchStatusChanged(i, i2, z);
        } catch (Exception e) {
            Log.w(TAG, "switchStatusChangedWrapper: failed to call native method", e);
        }
    }

    public void viewShownStatusChangedWrapper(int i, boolean z) {
        try {
            viewShownStatusChanged(i, z);
        } catch (Exception e) {
            Log.w(TAG, "viewShownStatusChangedWrapper: failed to call native method", e);
        }
    }

    public void onActivityResult(int i, int i2, Intent intent) {
        super.onActivityResult(i, i2, intent);
    }

    public void onDestroy() {
        super.onDestroy();
    }

    public void onPause() {
        super.onPause();
    }

    public void onRestart() {
        super.onRestart();
    }

    public void onResume() {
        super.onResume();
        if (this.isInit) {
            CloseAll();
        }
    }

    public void onStart() {
        super.onStart();
    }

    public void onStop() {
        super.onStop();
    }

    public void SetInputLayout(int i) {
        ui.showKeyboard(i == 1);
    }

    public void InitGui() {
        boolean z = this.isNewInterface;
        boolean z2 = this.show_fps;
        InitSettingWrapper(z, z2 ? 1 : 0, this.isNewKeyboard);
        this.isInit = true;
    }

    public void setBinderState(int i, boolean z) {

    }

    public void setBinderName(int i, byte[] bArr) {

    }

    public void addKeyboardHistory(byte[] bArr) {

    }

    @Override
    public void _keyboardSend(String str) {
        OnInputEndWrapper(str);
        CloseAll();
    }

    public void setUIElementVisible(int i, boolean z, boolean z2) {

    }

    public void setAutocompleteState(boolean z) {

    }

    public int addSliderToAzVoiceSettings(int i, int i2, int i3, byte[] bArr) {
        return 0;
    }

    public void removeAllSlidersFormPlayersVoiceSettings() {

    }

    public void addSliderToAzVoicePlayersSettings(int i, int i2, int i3, byte[] bArr, int i4) {

    }

    public void removeSliderFromAzVoicePlayerSettingsByPlayerId(int i) {

    }

    public void removeSliderFromAzVoiceSettingsBySliderId(int i) {

    }

    public void setAzVoiceUserSliders(byte[][] bArr, int[] iArr, int[] iArr2, int i, int i2) {

    }

    public void showPlayerDialog(int i, int i2, byte[] bArr, byte[] bArr2, byte[] bArr3, byte[] bArr4) {

    }

    public void removeMainScreen() {

    }

    public void sendFrontendMessage(int i, int i2, String str) {

    }

    public void setChatPageSize(int i) {
        getSettingsPreferences().edit().putInt("CHAT_PAGE_SIZE", i).apply();
    }

    public void setChatFontSize(float f) {
        getSettingsPreferences().edit().putFloat("CHAT_FONT_SIZE", f).apply();
    }

    public void setChatPrintTimestamp(boolean z) {
        getSettingsPreferences().edit().putBoolean("CHAT_PRINT_TIMESTAMP", z).apply();
    }

    public void setIsHeadMoving(boolean z) {
        getSettingsPreferences().edit().putBoolean("IS_HEAD_MOVING", z).apply();
    }

    private SharedPreferences getSettingsPreferences() {
        return getSharedPreferences("samp_settings", Context.MODE_PRIVATE);
    }

    public int getModloaderState() {
        return getSettingsPreferences().getInt("modloader", 0);
    }

    public void exitGame() {
        finish();
    }

    public boolean getAutocompleteState() {
        return this.isAutocompleteEnabled;
    }

    private void CloseAll() {
        OnKeyboardClosedWrapper();
    }
}
