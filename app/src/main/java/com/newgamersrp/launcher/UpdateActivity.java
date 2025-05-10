package com.newgamersrp.launcher;

import android.annotation.SuppressLint;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.view.WindowManager;
import android.widget.ProgressBar;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.NotificationCompat;
import androidx.core.content.FileProvider;

import com.google.android.material.textview.MaterialTextView;
import com.joom.paranoid.Obfuscate;

import java.io.File;

import com.newgamersrp.game.BuildConfig;
import com.newgamersrp.game.R;
import com.newgamersrp.launcher.utils.Utils;

@Obfuscate
public class UpdateActivity extends AppCompatActivity {
    private final Messenger mMessenger = new Messenger(new IncomingHandler());
    private Messenger mService;
    public UpdateMode mUpdateMode = UpdateMode.Undefined;

    private boolean mIsStartingUpdate;

    private static final int INSTALL_APK = 0;
    private File mGameApk;

    public enum UpdateMode {
        Undefined,
        GameUpdate
    }

    public enum GameStatus {
        Undefined,
        GameUpdateRequired,
        GameFilesUpdateRequired,
        Updated
    }

    public enum UpdateStatus {
        Undefined,
        CheckUpdate,
        DownloadGame,
        DownloadGameFiles
    }

    private final ServiceConnection mConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(@NonNull ComponentName componentName, @NonNull IBinder service) {
            mService = new Messenger(service);
            if (mUpdateMode == UpdateMode.GameUpdate) {
                Message outMsg = Message.obtain(null, 7);
                outMsg.replyTo = mMessenger;
                try {
                    mService.send(outMsg);
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mService = null;
        }
    };

    private final class IncomingHandler extends Handler {
        @SuppressLint("SetTextI18n")
        public void handleMessage(@NonNull Message msg) {
            // Log.i("UpdateActivity", "handleMessage -> " + msg.what);
            String apkPath;
            switch (msg.what) {
                case 1:
                    Log.i("UpdateActivity", "UpdateService.UPDATE_GAME");
                    apkPath = msg.getData().getString("apkPath", "");
                    if (msg.getData().getBoolean(NotificationCompat.CATEGORY_STATUS, false)) {
                        if (apkPath.length() > 0) mGameApk = new File(apkPath);

                        Message outMsg = Message.obtain(null, 2);
                        outMsg.replyTo = mMessenger;
                        try {
                            mService.send(outMsg);
                        } catch (RemoteException e) {
                            e.printStackTrace();
                        }
                    } else {
                        Log.e("UpdateActivity", "Error update game");
                        return;
                    }
                    break;
                case 2:
                    Log.i("UpdateActivity", "UpdateService.FINISH_UPDATE_GAME");
                    resetProgress(true, 100, 100);
                    ((MaterialTextView) findViewById(R.id.update_state)).setText("Installing...");
                    if (msg.getData().getBoolean(NotificationCompat.CATEGORY_STATUS, false)) {
                        apkPath = msg.getData().getString("apkPath", "");
                        if (apkPath.length() > 0) {
                            mGameApk = new File(apkPath);
                        }

                        if (mGameApk == null || Utils.beta_status || BuildConfig.DEBUG) {
                            startActivity(new Intent(UpdateActivity.this, SplashActivity.class));
                            finish();
                            break;
                        } else {
                            if (!mGameApk.exists() || Utils.beta_status || BuildConfig.DEBUG) {
                                startActivity(new Intent(UpdateActivity.this, SplashActivity.class));
                                finish();
                            } else {
                                requestInstallGame();
                            }
                        }
                    } else {
                        Log.e("UpdateActivity", "Error update game");
                        return;
                    }
                    break;
                case 4:
                    Log.i("UpdateActivity", "UpdateService.UPDATE_STATUS");
                    String statusName = msg.getData().getString(NotificationCompat.CATEGORY_STATUS, "");
                    Log.i("UpdateActivity", "statusname = " + statusName);
                    if (statusName.length() >= 2) {
                        switch (UpdateStatus.valueOf(statusName)) {
                            case Undefined:
                                if (!mIsStartingUpdate) {
                                    Message outMsg = Message.obtain(null, 1);
                                    outMsg.replyTo = mMessenger;
                                    try {
                                        mService.send(outMsg);
                                    } catch (RemoteException e) {
                                        e.printStackTrace();
                                    }
                                    mIsStartingUpdate = true;
                                }
                                break;
                            case CheckUpdate:
                            case DownloadGame:
                                ((MaterialTextView) findViewById(R.id.update_state)).setText("Updating game...");
                                if (msg.getData().getBoolean("withProgress", false)) {
                                    long total = msg.getData().getLong("total", 0);
                                    long current = msg.getData().getLong("current", 0);
                                    ProgressBar progressBar = findViewById(R.id.progressBar);
                                    progressBar.setIndeterminate(false);
                                    progressBar.setMax((int) (total / 1000));
                                    progressBar.setProgress((int) (current / 1000));
                                    ((MaterialTextView) findViewById(R.id.progress)).setText((((long) 100) * current) / (total + 1) + "%");
                                    ((MaterialTextView) findViewById(R.id.filename)).setText("");
                                    ((MaterialTextView) findViewById(R.id.size)).setText("");
                                }
                                break;
                            case DownloadGameFiles:
                                ((MaterialTextView) findViewById(R.id.update_state)).setText("Updating game files...");
                                if (msg.getData().getBoolean("withProgress", false)) {
                                    long total = msg.getData().getLong("total", 0);
                                    long current = msg.getData().getLong("current", 0);
                                    String filename = msg.getData().getString("filename", "");
                                    filename = filename.replace("8.com.newgamersrp.game.", "");
                                    long totalfiles = msg.getData().getLong("totalfiles", 0);
                                    long currentfile = msg.getData().getLong("currentfile", 0);
                                    ProgressBar progressBar = findViewById(R.id.progressBar);
                                    progressBar.setIndeterminate(false);
                                    progressBar.setMax((int) (total / 1000));
                                    progressBar.setProgress((int) (current / 1000));
                                    ((MaterialTextView) findViewById(R.id.progress)).setText((((long) 100) * current) / (total + 1) + "%");
                                    ((MaterialTextView) findViewById(R.id.filename)).setText(filename);
                                    ((MaterialTextView) findViewById(R.id.size)).setText(Utils.convertBytesToString(current) + " / " + Utils.convertBytesToString(total));

                                    Log.i("UpdateActivity", '[' + filename + "] total = " + total + " | current = " + current + " + " + (((long) 100) * current) / (total + 1) + "%");
                                }
                                break;
                        }
                    }
                    break;
                case 9:
                    Log.e("UpdateActivity", "UpdateActivity -> UpdateService.UPDATE_STATUS_ERROR");
                    try {
                        new AlertDialog.Builder(UpdateActivity.this)
                                .setTitle("Update:")
                                .setMessage("Failed to download the update!")
                                .setPositiveButton("Ok", (dialog, which) -> {
                                    startActivity(new Intent(UpdateActivity.this, SplashActivity.class));
                                    finish();
                                }).setCancelable(false).show();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    break;
            }
        }
    }

    protected void onDestroy() {
        super.onDestroy();
        unbindService(mConnection);
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(1);
        setContentView(R.layout.activity_update);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        resetProgress(false, 0, 0);
        ((MaterialTextView) findViewById(R.id.update_state)).setText("Preparing...");

        ((MaterialTextView) findViewById(R.id.ahahaha)).setText(Utils.copyright);
        
        mUpdateMode = UpdateMode.valueOf(getIntent().getStringExtra("mode"));
        if (mService == null)
            bindService(new Intent(this, UpdateService.class), mConnection, Context.BIND_AUTO_CREATE);
        else if (mUpdateMode == UpdateMode.GameUpdate) {
            Message outMsg = Message.obtain(null, 7);
            outMsg.replyTo = mMessenger;
            try {
                mService.send(outMsg);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
    }

    private void resetProgress(boolean withProgress, long total, long current) {
        ProgressBar progressBar = findViewById(R.id.progressBar);
        ((MaterialTextView) findViewById(R.id.progress)).setText("");
        ((MaterialTextView) findViewById(R.id.filename)).setText("");
        ((MaterialTextView) findViewById(R.id.size)).setText("");
        progressBar.setIndeterminate(true);
        if (withProgress) {
            progressBar.setMax((int) total);
            progressBar.setProgress((int) current);
        }
    }

    void requestInstallGame() {
        try {
            new AlertDialog.Builder(UpdateActivity.this)
                    .setTitle("Install:")
                    .setMessage("Install the game APK manually or automatically?")
                    .setPositiveButton("Automatically", (dialog, which) -> {
                        Intent intent = new Intent("android.intent.action.VIEW");
                        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        if (Build.VERSION.SDK_INT >= 24) {
                            intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
                            intent.setData(FileProvider.getUriForFile(getApplicationContext(), BuildConfig.APPLICATION_ID + ".provider", mGameApk));
                        } else {
                            intent.setDataAndType(Uri.parse("file://" + mGameApk.toString()), "application/vnd.android.package-archive");
                        }
                        sendBroadcast(intent);
                        startActivityForResult(intent, UpdateActivity.INSTALL_APK);
                    })
                    .setNegativeButton("Manually", (dialog, which) -> new AlertDialog.Builder(UpdateActivity.this)
                            .setTitle("Install:")
                            .setMessage("Open files explorer, go to path '" + Utils.EXTERNAL_DIR.toString() + "' and install '" + UpdateService.GAME_DOWNLOAD_NAME + "'.")
                            .setPositiveButton("Ok", (dialog2, which2) -> {
                                dialog2.cancel();
                                Uri parse = Uri.parse(Utils.EXTERNAL_DIR.toString() + File.separator);
                                Intent intent = new Intent("android.intent.action.VIEW");
                                intent.setDataAndType(parse, "resource/folder");
                                if (intent.resolveActivityInfo(getPackageManager(), 0) != null) {
                                    startActivity(intent);
                                }
                                finishAffinity();
                            }).setCancelable(false).show()).setCancelable(false).show();
        } catch (Exception e) {
            e.printStackTrace();
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