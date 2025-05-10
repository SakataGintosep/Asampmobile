package com.newgamersrp.game;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import android.widget.Toast;

import com.joom.paranoid.Obfuscate;
import com.rockstargames.gtasa.GTASA;

import java.util.Objects;

import com.newgamersrp.game.ui.UI;
import com.newgamersrp.launcher.utils.SignatureChecker;

@Obfuscate
public class SAMP extends GTASA {
    private static final String TAG = "SAMP";

    public UI ui;

    private native void initializeSAMP(UI ui, AssetManager assetManager);

    private native boolean multiTouchEvent4Ex(int action, int count, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);

    static {
        try {
            System.loadLibrary("Alyn_SAMPMOBILE");
            System.loadLibrary("samp");
        } catch (ExceptionInInitializerError | UnsatisfiedLinkError e) {
            Log.e(TAG, Objects.requireNonNull(e.getMessage()));
        }
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "**** onCreate");

        if (!Objects.equals(getIntent().getStringExtra("ahahahahadog"), "getrektkid")) {
            Log.e(TAG, "Not joined from launcher!");
            finish();
            return;
        }

        if (!SignatureChecker.isSignatureValid(this, getPackageName())) {
            Log.e("SplashActivity", "No remake idiot!");
            finish();
            return;
        } else {
            Log.i("SplashActivity", "Using original client!");
        }

        super.onCreate(savedInstanceState);
        Toast.makeText(this, "SA-MP Mobile Started", Toast.LENGTH_LONG).show();

        if (getSharedPreferences("samp_settings", Context.MODE_PRIVATE).getBoolean("cleo_scripts", false)) {
            try {
                System.loadLibrary("CLEO");
            } catch (ExceptionInInitializerError | UnsatisfiedLinkError e) {
                Log.e(TAG, Objects.requireNonNull(e.getMessage()));
            }
        }

        if (getSharedPreferences("samp_settings", Context.MODE_PRIVATE).getBoolean("aml_scripts", false)) {
            try {
                System.loadLibrary("AML");
            } catch (ExceptionInInitializerError | UnsatisfiedLinkError e) {
                Log.e(TAG, Objects.requireNonNull(e.getMessage()));
            }
        }

        if (getSharedPreferences("samp_settings", Context.MODE_PRIVATE).getBoolean("monet_scripts", false)) {
            try {
                System.loadLibrary("monetloader");
            } catch (ExceptionInInitializerError | UnsatisfiedLinkError e) {
                Log.e(TAG, Objects.requireNonNull(e.getMessage()));
            }
        }

        ui = new UI(this);
        ui.initializeUI();

        try {
            initializeSAMP(ui, getAssets());
        } catch (UnsatisfiedLinkError e) {
            Log.e(TAG, Objects.requireNonNull(e.getMessage()));
        }

        // fullscreen
        if (getSharedPreferences("samp_settings", Context.MODE_PRIVATE).getBoolean("fullscreen", false)) {
            if (Build.VERSION.SDK_INT >= 30) {
                getWindow().getAttributes().layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_ALWAYS;
            } else if (Build.VERSION.SDK_INT >= 28) {
                getWindow().getAttributes().layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            }
        }
    }

    /*@Override
    public boolean onTouchEvent(MotionEvent event) {
        if (wantsMultitouch) {
            int x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0, x4 = 0, y4 = 0;
            int numEvents = event.getPointerCount();

            for (int i = 0; i < numEvents; i++) {
                int pointerId = event.getPointerId(i);

                if (pointerId == 0) {
                    x1 = (int) event.getX(i);
                    y1 = (int) event.getY(i);
                } else if (pointerId == 1) {
                    x2 = (int) event.getX(i);
                    y2 = (int) event.getY(i);
                } else if (pointerId == 2) {
                    x3 = (int) event.getX(i);
                    y3 = (int) event.getY(i);
                } else if (pointerId == 3) {
                    x4 = (int) event.getX(i);
                    y4 = (int) event.getY(i);
                }
            }

            int pointerId = event.getPointerId(event.getActionIndex());
            int action = event.getActionMasked();

            try {
                multiTouchEvent4Ex(action, pointerId, x1, y1, x2, y2, x3, y3, x4, y4);
            } catch (UnsatisfiedLinkError e) {
                e.printStackTrace();
            }
        } else {
            touchEvent(event.getAction(), (int) event.getX(), (int) event.getY(), event);
        }
        return super.onTouchEvent(event);
    }*/

    @Override
    public void onStart() {
        Log.i(TAG, "**** onStart");
        super.onStart();
    }

    @Override
    public void onRestart() {
        Log.i(TAG, "**** onRestart");
        super.onRestart();
    }

    @Override
    public void onResume() {
        Log.i(TAG, "**** onResume");
        super.onResume();

        ui.onResume();
    }

    @Override
    public void onPause() {
        Log.i(TAG, "**** onPause");
        super.onPause();
    }

    @Override
    public void onStop() {
        Log.i(TAG, "**** onStop");
        super.onStop();
    }

    @Override
    public void onDestroy() {
        Log.i(TAG, "**** onDestroy");
        super.onDestroy();
    }
}
