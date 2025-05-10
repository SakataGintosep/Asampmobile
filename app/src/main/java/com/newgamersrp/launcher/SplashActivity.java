package com.newgamersrp.launcher;

import android.Manifest;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.os.StrictMode;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.google.android.material.button.MaterialButton;
import com.google.android.material.textview.MaterialTextView;
import com.joom.paranoid.Obfuscate;
import com.newgamersrp.game.BuildConfig;
import com.newgamersrp.game.R;
import com.newgamersrp.launcher.utils.Logcat;
import com.newgamersrp.launcher.utils.SignatureChecker;
import com.newgamersrp.launcher.utils.Utils;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

@Obfuscate
public class SplashActivity extends AppCompatActivity implements GLSurfaceView.Renderer {
    private static final int PERMISSION_REQUEST_CODE = 1337;
    private static final String[] permissions = {
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.RECORD_AUDIO
    };

    public IncomingHandler mInHandler;
    public Messenger mMessenger;
    public Messenger mService;
    GLSurfaceView glSurfaceView;

    private final ServiceConnection mConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(@NonNull ComponentName componentName, @NonNull IBinder service) {
            mService = new Messenger(service);
            checkUpdate();
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mService = null;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        StrictMode.setThreadPolicy(new StrictMode.ThreadPolicy.Builder().permitAll().build());

        if (!SignatureChecker.isSignatureValid(this, getPackageName())) {
            Log.e("SplashActivity", "No remake idiot!");
            finish();
            return;
        } else {
            Log.i("SplashActivity", "Using original client!");
        }

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash);

        ((MaterialTextView) findViewById(R.id.ahahaha)).setText(Utils.copyright);

        mInHandler = new IncomingHandler();
        mMessenger = new Messenger(mInHandler);

        Logcat.save();

        glSurfaceView = findViewById(R.id.surface);
        glSurfaceView.setRenderer(this);

        Toast.makeText(this, "SCH SA-MP Launcher v" + BuildConfig.VERSION_NAME, Toast.LENGTH_SHORT).show();

        if (arePermissionsGranted()) {
            startApp();
        } else {
            requestPermissions();
        }
    }

    private boolean arePermissionsGranted() {
        for (String permission : permissions) {
            if (ContextCompat.checkSelfPermission(this, permission) != PackageManager.PERMISSION_GRANTED) {
                return false;
            }
        }

        return true;
    }

    private void requestPermissions() {
        ActivityCompat.requestPermissions(this, permissions, PERMISSION_REQUEST_CODE);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (requestCode == PERMISSION_REQUEST_CODE) {
            boolean granted = true;
            for (int result : grantResults) {
                if (result != PackageManager.PERMISSION_GRANTED) {
                    granted = false;
                    break;
                }
            }

            if (granted) {
                startApp();
            } else {
                Toast.makeText(this, "Permissions not granted! (try to give from app settings)", Toast.LENGTH_LONG).show();
                startApp();
            }
        }
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    private void startApp() {
        getExternalFilesDir("/");
        getObbDir();
        getExternalMediaDirs();

        if (!Utils.isOnline(this)) {
            Toast.makeText(this, "No internet connection!", Toast.LENGTH_LONG).show();
            return;
        }

        if (!getSharedPreferences("samp_settings", Context.MODE_PRIVATE).getString("files_type", "none").equals("lite") &&
                !getSharedPreferences("samp_settings", Context.MODE_PRIVATE).getString("files_type", "none").equals("full")) {
            View view = getLayoutInflater().inflate(R.layout.layout_files_selection, null);

            AlertDialog.Builder dialogBuilder = new AlertDialog.Builder(this).setView(view);
            AlertDialog dialog = dialogBuilder.create();

            MaterialButton lite = view.findViewById(R.id.lite_button);
            MaterialButton full = view.findViewById(R.id.full_button);

            lite.setOnClickListener(v -> {
                getSharedPreferences("samp_settings", Context.MODE_PRIVATE).edit().putString("files_type", "lite").apply();
                dialog.dismiss();
                new Handler().postDelayed(() -> bindService(new Intent(this, UpdateService.class), mConnection, Context.BIND_AUTO_CREATE), 2500);
            });

            full.setOnClickListener(v -> {
                getSharedPreferences("samp_settings", Context.MODE_PRIVATE).edit().putString("files_type", "full").apply();
                dialog.dismiss();
                new Handler().postDelayed(() -> bindService(new Intent(this, UpdateService.class), mConnection, Context.BIND_AUTO_CREATE), 2500);
            });

            dialog.show();
        } else {
            new Handler().postDelayed(() -> bindService(new Intent(this, UpdateService.class), mConnection, Context.BIND_AUTO_CREATE), 2500);
        }
        //new Handler().postDelayed(() -> bindService(new Intent(this, UpdateService.class), mConnection, Context.BIND_AUTO_CREATE), 2500);
    }

    private void startMain() {
        if (!isFinishing()) {
            try {
                Intent intent = new Intent(SplashActivity.this, MainActivity.class);
                intent.putExtra("ahahahahadog", "getrektkid");
                intent.putExtra("login_success", true); // Tambahkan flag login sukses
                startActivity(intent);
                finish();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        String renderer = gl10.glGetString(gl10.GL_RENDERER);
        Log.i("SplashActivity", "Renderer: " + renderer);

        String extensions = gl10.glGetString(gl10.GL_EXTENSIONS);
        Log.i("SplashActivity", "Extensions: " + extensions);

        Utils.GPUType type;

        if (extensions.contains("GL_IMG_texture_compression_pvrtc")) {
            type = Utils.GPUType.PVR;
        } else if (extensions.contains("GL_EXT_texture_compression_dxt1") || extensions.contains("GL_EXT_texture_compression_s3tc") || extensions.contains("GL_AMD_compressed_ATC_texture")) {
            type = Utils.GPUType.DXT;
        } else {
            type = Utils.GPUType.ETC;
        }

        /*if (extensions.contains("pvr")) {
            type = Utils.GPUType.PVR;
        } else if (extensions.contains("dxt")) {
            type = Utils.GPUType.DXT;
        } else {
            type = Utils.GPUType.ETC;
        }*/

        Utils.GPU_TYPE = type;
        Log.i("SplashActivity", "GPU_TYPE: " + Utils.GPU_TYPE.name());

        int error = gl10.glGetError();
        if (error != gl10.GL_NO_ERROR) {
            Log.e("SplashActivity", "OpenGL error: " + error);
        }

        runOnUiThread(() -> glSurfaceView.setVisibility(View.GONE));
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int i, int i1) {

    }

    @Override
    public void onDrawFrame(GL10 gl10) {

    }

    public void checkUpdate() {
        // Log.i("SplashActivity", "checkUpdate");

        if (!Utils.isOnline(this)) {
            Toast.makeText(this, "No internet connection!", Toast.LENGTH_LONG).show();
            return;
        }

        Message obtain = Message.obtain(null, 0);
        obtain.replyTo = mMessenger;

        try {
            mService.send(obtain);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    private class IncomingHandler extends Handler {
        @Override
        public void handleMessage(Message message) {
            int i = message.what;
            if (i == 4) {
                UpdateActivity.UpdateStatus updateStatus = UpdateActivity.UpdateStatus.valueOf(message.getData().getString("status", ""));
                if (updateStatus == UpdateActivity.UpdateStatus.Undefined) {
                    Message obtain = Message.obtain(null, 5);
                    obtain.replyTo = mMessenger;
                    Messenger messenger = mService;
                    try {
                        messenger.send(obtain);
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                } else if (updateStatus == UpdateActivity.UpdateStatus.CheckUpdate) {
                    Message obtain2 = Message.obtain(null, 4);
                    obtain2.replyTo = mMessenger;
                    Messenger messenger = mService;
                    try {
                        messenger.send(obtain2);
                    } catch (RemoteException e2) {
                        e2.printStackTrace();
                    }
                }
            } else if (i == 5) {
                UpdateActivity.GameStatus gameStatus = UpdateActivity.GameStatus.valueOf(message.getData().getString("status", ""));
                Log.i("SplashActivity", "gameStatus = " + gameStatus);

                    Utils.checkServerStatus(isOnline -> {  
                    Log.i("SplashActivity", "Server status: " + isOnline);   
                    if (!isOnline) {  
                        Log.e("SplashActivity", "Server down!");  
                        try {  
                            new AlertDialog.Builder(SplashActivity.this)  
                                .setTitle("Status:")
                                .setMessage("EN:\nThe application server is not working, make sure you have the latest client version. If the problem continues, please contact the developer\n\nID:\nServer aplikasi sedang tidak berfungsi, pastikan anda memiliki versi client terbaru. Jika masalah berlanjut, silahkan hubungi developer.")  
                                .setPositiveButton("Exit", (dialog, which) -> finish())  
                                .setCancelable(false)  
                                .show();  
                        } catch (Exception e) {  
                            e.printStackTrace();  
                        }  
                     } else {  
                        if (gameStatus == UpdateActivity.GameStatus.GameUpdateRequired && !Utils.beta_status && !BuildConfig.DEBUG) {  
                            try {  
                                new AlertDialog.Builder(SplashActivity.this)  
                                        .setTitle("Update:")
                                        .setMessage("App Launcher update required!")  
                                        .setPositiveButton("Update", (dialog, which) -> {  
                                            Intent intent = new Intent(SplashActivity.this, UpdateActivity.class);  
                                            intent.putExtra("mode", UpdateActivity.UpdateMode.GameUpdate.name());  
                                            startActivity(intent);  
                                            finish();  
                                        })  
                                        .setCancelable(false)  
                                        .show();  
                            } catch (Exception e) {  
                                e.printStackTrace();  
                            }  
                        } else if (gameStatus == UpdateActivity.GameStatus.GameFilesUpdateRequired) {  
                            try {  
                                new AlertDialog.Builder(SplashActivity.this)  
                                        .setTitle("Warning:")
                                        .setMessage("EN:\nGame cache are incomplete\nplease update if blackscreen when entering the server.\n\nID:\nGame cache tidak lengkap\nHarap perbarui jika layar hitam saat memasuki server.")  
                                        .setPositiveButton("Update", (dialog, which) -> {  
                                            Intent intent = new Intent(SplashActivity.this, UpdateActivity.class);  
                                            intent.putExtra("mode", UpdateActivity.UpdateMode.GameUpdate.name());  
                                            startActivity(intent);  
                                            finish();  
                                        })  
                                        .setNegativeButton("No", (dialog, which) -> {  
                                            if (!Utils.isOnline(SplashActivity.this)) {  
                                                Toast.makeText(SplashActivity.this, "No internet connection!", Toast.LENGTH_LONG).show();  
                                                return;  
                                            }  
                                            startMain();  
                                        })  
                                        .setCancelable(false)  
                                        .show();  
                            } catch (Exception e) {  
                                e.printStackTrace();  
                            }  
                        } else {  
                            if (!Utils.isOnline(SplashActivity.this)) {  
                                Toast.makeText(SplashActivity.this, "No internet connection!", Toast.LENGTH_LONG).show();  
                                return;  
                            }  
                            startMain();  
                        }  
                    }  
                });
            }
        }
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        quitApp();
    }

    private long quit_time = 0;

    public void quitApp() {
        if ((System.currentTimeMillis() - quit_time) > 2000) {
            Toast.makeText(this, "Press again to quit.", Toast.LENGTH_LONG).show();
            quit_time = System.currentTimeMillis();
        } else {
            finish();
        }
    }
}
