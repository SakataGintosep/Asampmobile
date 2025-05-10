package com.newgamersrp.launcher.utils;

import android.graphics.Paint;
import android.text.Editable;
import android.text.TextPaint;
import android.text.TextWatcher;
import android.widget.TextView;

import com.joom.paranoid.Obfuscate;

@Obfuscate
public class OutlineTextWatcher implements TextWatcher {
    private final TextView textView;
    private final float strokeWidth;
    private final int strokeColor;

    public OutlineTextWatcher(TextView textView, float strokeWidth, int strokeColor) {
        this.textView = textView;
        this.strokeWidth = strokeWidth;
        this.strokeColor = strokeColor;
    }

    @Override
    public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {
        // No action needed
    }

    @Override
    public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
        // No action needed
    }

    @Override
    public void afterTextChanged(Editable editable) {
        applyTextOutline();
    }

    private void applyTextOutline() {
        TextPaint textPaint = textView.getPaint();
        textPaint.setStyle(Paint.Style.STROKE);
        textPaint.setStrokeWidth(strokeWidth);
        textPaint.setColor(strokeColor);
        textPaint.setAntiAlias(true);

        textView.invalidate();
    }
}