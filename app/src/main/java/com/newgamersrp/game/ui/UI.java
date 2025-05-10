package com.newgamersrp.game.ui;

import android.graphics.Color;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.style.ForegroundColorSpan;
import android.util.Log;

import com.joom.paranoid.Obfuscate;
import com.newgamersrp.game.SAMP;
import com.newgamersrp.game.ui.widgets.ButtonPanel;
import com.newgamersrp.game.ui.widgets.Chat;
import com.newgamersrp.game.ui.widgets.Dialog;
import com.newgamersrp.game.ui.widgets.HeightProvider;
import com.newgamersrp.game.ui.widgets.Keyboard;
import com.newgamersrp.game.ui.widgets.Scoreboard;
import com.newgamersrp.game.ui.widgets.Voice;
import com.newgamersrp.game.ui.widgets.WantedLevel;

import java.util.Arrays;

@Obfuscate
public class UI implements ButtonPanel.Listener, Voice.Listener, Dialog.Listener, Scoreboard.Listener, Keyboard.Listener, Chat.Listener, HeightProvider.Listener {
    private static final String TAG = "UI";

    private final SAMP samp;

    private ButtonPanel buttonPanel;
    private Voice voice;
    private Dialog dialog;
    private Keyboard keyboard;
    private Scoreboard scoreboard;
    private WantedLevel wantedLevel;
    private Chat chat = null;
    private HeightProvider heightProvider = null;

    private native void sendButtonPanelKey(int id);
    private native void voicePressed();

    private native void keyboardSend(byte[] str);

    private native void sendDialogResponse(int dialog_id, int button_id, int list_item, byte[] input);

    private native void sendScoreboardClick(int player_id);

    private native void chatSend(String str);

    public interface Listener {
        void _keyboardSend(String str);
    }

    public UI(SAMP samp) {
        this.samp = samp;
    }

    public void initializeUI() {
        //new Copyright(samp);
        heightProvider = new HeightProvider(samp).init(samp.GetMainView()).setHeightListener(this);
        buttonPanel = new ButtonPanel(samp, this);
        voice = new Voice(samp, this);
        chat = new Chat(samp, this);
        dialog = new Dialog(samp, this);
        scoreboard = new Scoreboard(samp, this);
        keyboard = new Keyboard(samp, this);
        wantedLevel = new WantedLevel(samp);
    }

    private void showButtonPanel(boolean z) {
        // Log.i(TAG, "**** showButtonPanel " + z);
        samp.runOnUiThread(() -> buttonPanel.show(z));
    }

    private void showVoice(boolean z) {
        // Log.i(TAG, "**** showVoice " + z);
        samp.runOnUiThread(() -> voice.show(z));
    }

    private void showDialog(int dialog_id, int dialog_style, String title, String message, String button1, String button2) {
        Log.i(TAG, "**** showDialog " + dialog_id + " " + title + " " + message + " " + button1 + " " + button2);
        samp.runOnUiThread(() -> dialog.show(dialog_id, dialog_style, title, message, button1, button2));
    }

    private void showDialogWithoutReset(boolean z) {
        Log.i(TAG, "**** showDialogWithoutReset " + z);
        samp.runOnUiThread(() -> dialog.showWithoutReset(z));
    }

    private void showScoreboard(boolean z) {
        Log.i(TAG, "**** showScoreboard " + z);
        samp.runOnUiThread(() -> scoreboard.show(z));
    }

    private void setScoreboardStats(String server, int players) {
        Log.i(TAG, "**** setScoreboardStats " + server + " " + players);
        samp.runOnUiThread(() -> scoreboard.setStats(server, players));
    }

    private void addScoreboardPlayer(int id, String name, int score, int ping, String color) {
        Log.i(TAG, "**** addScoreboardPlayer " + id + " " + name + " " + score + " " + ping + " " + color);
        samp.runOnUiThread(() -> scoreboard.addPlayer(id, name, score, ping, color));
    }

    private void showWantedLevel(boolean z) {
        // Log.i(TAG, "**** showWantedLevel " + z);
        samp.runOnUiThread(() -> wantedLevel.show(z));
    }

    private void setWantedLevel(int level) {
        Log.i(TAG, "**** setWantedLevel " + level);
        samp.runOnUiThread(() -> wantedLevel.setLevel(level));
    }

    @Override
    public void _sendButtonPanelKey(int id) {
        Log.i(TAG, "**** sendButtonPanelKey " + id);
        sendButtonPanelKey(id);
    }

    @Override
    public void _voicePressed() {
        Log.i(TAG, "**** voicePressed");
        voicePressed();
    }

    @Override
    public void _sendDialogResponse(int dialog_id, int button_id, int list_item, byte[] input) {
        Log.i(TAG, "**** sendDialogResponse " + dialog_id + " " + button_id + " " + list_item + " " + Arrays.toString(input));
        sendDialogResponse(dialog_id, button_id, list_item, input);
    }

    @Override
    public void _sendScoreboardClick(int player_id) {
        Log.i(TAG, "**** _sendScoreboardClick " + player_id);
        sendScoreboardClick(player_id);
    }

    @Override
    public void _keyboardSend(String str) {
        // Log.i(TAG, "**** keyboardSend " + str);

        /*byte[] ret = new byte[0];

        try {
            ret = str.getBytes("windows-1251");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }

        try {
            keyboardSend(ret);
        } catch (UnsatisfiedLinkError e) {
            Log.e(TAG, Objects.requireNonNull(e.getMessage()));
        }*/

        ((Listener) samp)._keyboardSend(str);
    }

    public void showKeyboard(boolean z) {
        Log.i(TAG, "**** showKeyboard " + z);

        samp.runOnUiThread(() -> keyboard.show(z));
    }

    private void showChat() {
        Log.i(TAG, "**** showChat");
        samp.runOnUiThread(() -> chat.show());
    }

    public void addChatMessage(String message, String nick, String nickColor) {
        String formattedMessage = nick + ": " + message;
        SpannableString spannableString = new SpannableString(formattedMessage);
        spannableString.setSpan(new ForegroundColorSpan(Color.parseColor(nickColor)), 0, nick.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        spannableString.setSpan(new ForegroundColorSpan(Color.parseColor("#FFFFFF")), nick.length(), formattedMessage.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        samp.runOnUiThread(() -> chat.addMessage(spannableString));
    }

    public void addInfoMessage(String message) {
        SpannableString spannableString = new SpannableString(message);
        spannableString.setSpan(new ForegroundColorSpan(Color.parseColor("#00C8C8")), 0, message.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        samp.runOnUiThread(() -> chat.addMessage(spannableString));
    }

    public void addDebugMessage(String message) {
        SpannableString spannableString = new SpannableString(message);
        spannableString.setSpan(new ForegroundColorSpan(Color.parseColor("#F4A419")), 0, message.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        samp.runOnUiThread(() -> chat.addMessage(spannableString));
    }

    public void addClientMessage(String message, String color) {
        SpannableString spannableString = new SpannableString(message);
        spannableString.setSpan(new ForegroundColorSpan(Color.parseColor(color)), 0, message.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        samp.runOnUiThread(() -> chat.addMessage(spannableString));
    }

    @Override
    public void chatShowKeyboard(boolean z) {

    }

    @Override
    public void _chatSend(String str) {
        Log.i(TAG, "**** chatSend " + str);
        chatSend(str);
    }

    @Override
    public void onHeightChanged(int orientation, int height) {
        if (keyboard != null) {
            keyboard.onHeightChanged(height);
        }

        if (dialog != null) {
            dialog.onHeightChanged(height);
        }
    }

    public void onResume() {
        if (heightProvider != null) {
            heightProvider.init(samp.GetMainView());
        }
    }
}
