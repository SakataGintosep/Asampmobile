package com.newgamersrp.launcher;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.navigation.NavController;
import androidx.navigation.NavOptions;
import androidx.navigation.Navigation;

import com.google.android.material.imageview.ShapeableImageView;
import com.google.android.material.textview.MaterialTextView;
import com.joom.paranoid.Obfuscate;
import com.newgamersrp.game.R;
import com.newgamersrp.launcher.utils.SignatureChecker;
import com.newgamersrp.launcher.utils.Utils;

import androidx.appcompat.app.AlertDialog;

@Obfuscate
public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        if (!getIntent().getStringExtra("ahahahahadog").equals("getrektkid")) {
            Log.e("MainActivity", "Not joined from launcher!");
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

        if (!Utils.isOnline(this)) {
            Toast.makeText(this, "No internet connection!", Toast.LENGTH_LONG).show();

            try {
                Intent intent = new Intent(MainActivity.this, SplashActivity.class);
                startActivity(intent);
                finish();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (getIntent().getBooleanExtra("login_success", false)) {
            showLoginSuccessPopup();
        }

        ((MaterialTextView) findViewById(R.id.ahahaha)).setText(Utils.copyright);

        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment);

        ShapeableImageView home_button = findViewById(R.id.home_button);
        ShapeableImageView play_button = findViewById(R.id.play_button);
        ShapeableImageView settings_button = findViewById(R.id.settings_button);

        home_button.setOnClickListener(view -> navController.navigate(R.id.information_page_fragment, Bundle.EMPTY, new NavOptions.Builder().setLaunchSingleTop(true).build()));
        play_button.setOnClickListener(view -> navController.navigate(R.id.servers_page_fragment, Bundle.EMPTY, new NavOptions.Builder().setLaunchSingleTop(true).build()));
        settings_button.setOnClickListener(view -> navController.navigate(R.id.settings_page_fragment, Bundle.EMPTY, new NavOptions.Builder().setLaunchSingleTop(true).build()));
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

    private void showLoginSuccessPopup() {
        new AlertDialog.Builder(this)
                .setTitle("Attention")
                .setMessage("Special thanks to the following people below:\n-Alyn SA-MP Mobile\n-RevTer\n-zkaaa\n-Bang Daxtra\n-VinnnX\n\n*If you steal this client, then you will burn in hell without time limit.")
                .setPositiveButton("OK", (dialog, which) -> dialog.dismiss())
                .setCancelable(false)
                .show();
    }
}
