package com.newgamersrp.game.ui.widgets;

import android.app.Activity;
import android.content.Context;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.RelativeLayout;

import com.google.android.material.textfield.TextInputEditText;
import com.joom.paranoid.Obfuscate;

import java.util.ArrayList;

import com.newgamersrp.game.R;

@Obfuscate
public class Keyboard {
    private final Activity activity;
    private final Listener listener;
    private boolean visible;

    private final ArrayList<String> history;
    private int current_history;

    private final RelativeLayout keyboard_layout;
    private final TextInputEditText keyboard_input;

    public interface Listener {
        void _keyboardSend(String str);
    }

    public Keyboard(Activity activity, Listener listener) {
        this.activity = activity;
        this.listener = listener;

        visible = false;

        history = new ArrayList<>();
        current_history = 0;

        keyboard_layout = (RelativeLayout) activity.getLayoutInflater().inflate(R.layout.activity_game_keyboard, null);
        activity.addContentView(keyboard_layout, new RelativeLayout.LayoutParams(-1, -1));

        keyboard_input = keyboard_layout.findViewById(R.id.keyboard_input);
        ImageButton keyboard_button_ok = keyboard_layout.findViewById(R.id.keyboard_button_ok);
        ImageButton keyboard_button_slash = keyboard_layout.findViewById(R.id.keyboard_button_slash);
        ImageButton keyboard_button_prev = keyboard_layout.findViewById(R.id.keyboard_button_prev);
        ImageButton keyboard_button_next = keyboard_layout.findViewById(R.id.keyboard_button_next);

        keyboard_input.setOnEditorActionListener((textView, i, keyEvent) -> {
            if (i == EditorInfo.IME_ACTION_DONE || i == EditorInfo.IME_ACTION_NEXT) {
                if (keyboard_input.getText() != null) {
                    send(keyboard_input.getText().toString());
                    keyboard_input.setText("");
                }
                show(false);
                return true;
            }
            return false;
        });

        keyboard_button_ok.setOnClickListener(view -> {
            if (keyboard_input.getText() != null) {
                send(keyboard_input.getText().toString());
                keyboard_input.setText("");
            }
            show(false);
        });

        keyboard_button_slash.setOnClickListener(view -> {
            keyboard_input.setText("/");
            keyboard_input.setSelection(keyboard_input.getText().length());
        });

        keyboard_button_prev.setOnClickListener(view -> {
            current_history--;

            if (current_history < 0) {
                current_history = 0;
            }

            if (current_history <= 0) {
                keyboard_input.setText("");
                return;
            }

            keyboard_input.setText(history.get(current_history - 1));
            keyboard_input.setSelection(keyboard_input.getText().length());
        });

        keyboard_button_next.setOnClickListener(view -> {
            current_history++;

            if ((current_history - 1) >= history.size()) {
                current_history--;
            }

            if (current_history <= 0) {
                return;
            }

            keyboard_input.setText(history.get(current_history - 1));
            keyboard_input.setSelection(keyboard_input.getText().length());
        });

        show(false);
    }

    public boolean isVisible() {
        return visible;
    }

    public void show(boolean z) {
        visible = z;
        current_history = 0;

        keyboard_layout.setVisibility(z ? View.VISIBLE : View.GONE);

        InputMethodManager imm = (InputMethodManager) activity.getSystemService(Context.INPUT_METHOD_SERVICE);

        if (z) {
            keyboard_input.requestFocus();
            imm.showSoftInput(keyboard_input, InputMethodManager.SHOW_IMPLICIT);
        } else {
            imm.hideSoftInputFromWindow(keyboard_input.getWindowToken(), 0);
        }
    }

    private void send(String str) {
        if (history.size() >= 20) {
            history.remove(history.size() - 1);
        }

        history.add(0, str);

        listener._keyboardSend(str);
    }

    public void onHeightChanged(int height) {
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) keyboard_layout.getLayoutParams();
        params.setMargins(0, 0, 0, height);
        keyboard_layout.setLayoutParams(params);
    }
}
