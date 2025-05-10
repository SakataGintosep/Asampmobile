package com.newgamersrp.launcher.ui.fragment;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.widget.ArrayAdapter;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.Fragment;

import com.google.android.material.button.MaterialButton;
import com.google.android.material.button.MaterialButtonToggleGroup;
import com.google.android.material.materialswitch.MaterialSwitch;
import com.google.android.material.textfield.MaterialAutoCompleteTextView;
import com.google.android.material.textfield.TextInputEditText;
import com.joom.paranoid.Obfuscate;
import com.newgamersrp.game.R;
import com.newgamersrp.launcher.SplashActivity;
import com.newgamersrp.launcher.utils.Utils;

import java.util.Objects;

@Obfuscate
public class SettingsPageFragment extends Fragment {
    MaterialAutoCompleteTextView samp_version;

    MaterialSwitch voice;
    MaterialSwitch timestamp;
    MaterialSwitch fullscreen;
    MaterialSwitch fastconnect;
    MaterialSwitch displayfps;
    MaterialSwitch newinterface;
    MaterialSwitch systemkeyboard;
    MaterialSwitch cleo;
    MaterialSwitch aml;
    MaterialSwitch monet;
    MaterialSwitch modloader;
    MaterialSwitch modify_files;

    MaterialButton data_full;
    MaterialButton data_lite;

    MaterialButtonToggleGroup fpslimit_layout;

    MaterialButton chat_minus;
    MaterialButton chat_strings;
    MaterialButton chat_plus;

    TextInputEditText fontsize;
    TextInputEditText chatposx;
    TextInputEditText chatposy;

    TextInputEditText chatsizex;
    TextInputEditText chatsizey;

    SharedPreferences settings_prefs;

    public SettingsPageFragment() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_settings_page, container, false);

        settings_prefs = requireActivity().getSharedPreferences("samp_settings", Context.MODE_PRIVATE);

        samp_version = view.findViewById(R.id.samp_version);
        String[] items = {"0.3.7", "0.3.7-R4"};
        ArrayAdapter<String> selectCoreAdapter = new ArrayAdapter<>(requireContext(), R.layout.layout_setting_samp_version, items);
        samp_version.setAdapter(selectCoreAdapter);
        samp_version.setText(items[settings_prefs.getInt("samp_version", 0)], false);

        voice = view.findViewById(R.id.voice_switch);
        timestamp = view.findViewById(R.id.timestamp_switch);
        fullscreen = view.findViewById(R.id.fullscreen_switch);
        fastconnect = view.findViewById(R.id.fastconnect_switch);
        displayfps = view.findViewById(R.id.displayfps_switch);
        newinterface = view.findViewById(R.id.newinterface_switch);
        systemkeyboard = view.findViewById(R.id.systemkeyboard_switch);
        cleo = view.findViewById(R.id.cleo_switch);
        aml = view.findViewById(R.id.aml_switch);
        monet = view.findViewById(R.id.monet_switch);
        modloader = view.findViewById(R.id.modloader_switch);
        modify_files = view.findViewById(R.id.modify_switch);

        data_full = view.findViewById(R.id.data_full);
        data_lite = view.findViewById(R.id.data_lite);

        fpslimit_layout = view.findViewById(R.id.fpslimit_layout);

        chat_minus = view.findViewById(R.id.chat_minus);
        chat_strings = view.findViewById(R.id.chat_strings);
        chat_plus = view.findViewById(R.id.chat_plus);

        fontsize = view.findViewById(R.id.fontsize);

        chatposx = view.findViewById(R.id.chatposx);
        chatposy = view.findViewById(R.id.chatposy);

        chatsizex = view.findViewById(R.id.chatsizex);
        chatsizey = view.findViewById(R.id.chatsizey);

        MaterialButton restore_button = view.findViewById(R.id.restore_button);
        restore_button.setOnClickListener(v -> new AlertDialog.Builder(requireActivity()).setTitle("Settings:").setMessage("Are you sure you want to restore to default settings? (This action will not delete your game files)").setPositiveButton("Yes", (dialog, which) -> {
            Toast.makeText(getContext(), "Successfully restored to default settings.", Toast.LENGTH_LONG).show();
            Utils.restoreSettings(requireContext());
            setWidgetsForSettings();
            dialog.dismiss();
        }).setNegativeButton("No", (dialog, which) -> dialog.dismiss()).show());

        // save settings
        Utils.saveSettings(getContext());

        // set widgets for settings
        setWidgetsForSettings();

        samp_version.setOnItemClickListener((adapterView, v, i, l) -> {
            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putInt("samp_version", i);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });

        newinterface.setOnCheckedChangeListener((buttonView, isChecked) -> {
            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putBoolean("new_interface", isChecked);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });

        systemkeyboard.setOnCheckedChangeListener((buttonView, isChecked) -> {
            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putBoolean("system_keyboard", isChecked);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });

        timestamp.setOnCheckedChangeListener((buttonView, isChecked) -> {
            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putBoolean("timestamp", isChecked);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });

        fullscreen.setOnCheckedChangeListener((buttonView, isChecked) -> {
            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putBoolean("fullscreen", isChecked);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });

        fastconnect.setOnCheckedChangeListener((buttonView, isChecked) -> {
            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putBoolean("fast_connect", isChecked);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });

        voice.setOnCheckedChangeListener((buttonView, isChecked) -> {
            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putBoolean("voice_chat", isChecked);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });

        displayfps.setOnCheckedChangeListener((buttonView, isChecked) -> {
            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putBoolean("display_fps", isChecked);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });

        cleo.setOnCheckedChangeListener((buttonView, isChecked) -> {
            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putBoolean("cleo_scripts", isChecked);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });

        aml.setOnCheckedChangeListener((buttonView, isChecked) -> {
            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putBoolean("aml_scripts", isChecked);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });

        monet.setOnCheckedChangeListener((buttonView, isChecked) -> {
            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putBoolean("monet_scripts", isChecked);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });

        modloader.setOnCheckedChangeListener((buttonView, isChecked) -> {
            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putBoolean("modloader", isChecked);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });


        modify_files.setOnCheckedChangeListener((buttonView, isChecked) -> {
            if (isChecked) {
                new AlertDialog.Builder(requireActivity()).setTitle("Warning:").setMessage("If you enable this feature, the launcher will not ask you for update game files anymore. So if you face any crash, disable this option and update game files!").setPositiveButton("Ok", (dialog, which) -> dialog.dismiss()).show();
            }

            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putBoolean("modify_files", isChecked);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });

        data_full.setOnClickListener(v -> {
            if (settings_prefs.getString("files_type", "none").equals("full")) {
                data_full.setChecked(true);
                return;
            }

            new AlertDialog.Builder(requireActivity()).setTitle("Warning:").setMessage("Are you sure you want to change the game files to full? (2.3GB)").setPositiveButton("Yes", (dialog, which) -> {
                // save settings prefs
                SharedPreferences.Editor edit = settings_prefs.edit();
                edit.putString("files_type", "full");
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());

                dialog.dismiss();

                // start splash activity to update game files
                Intent intent = new Intent(getContext(), SplashActivity.class);
                startActivity(intent);
                requireActivity().finish();
            }).setNegativeButton("No", (dialog, which) -> {
                data_full.setChecked(false);
                dialog.dismiss();
            }).show();
        });

        data_lite.setOnClickListener(v -> {
            if (settings_prefs.getString("files_type", "none").equals("lite")) {
                data_lite.setChecked(true);
                return;
            }

            new AlertDialog.Builder(requireActivity()).setTitle("Warning:").setMessage("Are you sure you want to change the game files to lite? (900MB)").setPositiveButton("Yes", (dialog, which) -> {
                // save settings prefs
                SharedPreferences.Editor edit = settings_prefs.edit();
                edit.putString("files_type", "lite");
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());

                dialog.dismiss();

                // start splash activity to update game files
                Intent intent = new Intent(getContext(), SplashActivity.class);
                startActivity(intent);
                requireActivity().finish();
            }).setNegativeButton("No", (dialog, which) -> {
                data_lite.setChecked(false);
                dialog.dismiss();
            }).show();
        });

        fpslimit_layout.addOnButtonCheckedListener((materialButtonToggleGroup, i, b) -> {
            SharedPreferences.Editor edit = settings_prefs.edit();

            if (i == R.id.fpslimit_60 && b) {
                // save settings prefs
                edit.putInt("fps_limit", 60);
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());
            } else if (i == R.id.fpslimit_90 && b) {
                // save settings prefs
                edit.putInt("fps_limit", 90);
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());
            } else if (i == R.id.fpslimit_120 && b) {
                // save settings prefs
                edit.putInt("fps_limit", 120);
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());
            }
        });

        chat_minus.setOnClickListener(v -> {
            chat_minus.setChecked(false);

            int currentValue = settings_prefs.getInt("chat_strings", 5);

            if (currentValue <= 5) {
                return; // avoid going below 5
            }

            currentValue -= 1;
            chat_strings.setText(String.valueOf(currentValue));

            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putInt("chat_strings", currentValue);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });

        chat_plus.setOnClickListener(v -> {
            chat_plus.setChecked(false);

            int currentValue = settings_prefs.getInt("chat_strings", 5);

            if (currentValue >= 20) {
                return; // avoid going above 20
            }

            currentValue += 1;
            chat_strings.setText(String.valueOf(currentValue));

            // save settings prefs
            SharedPreferences.Editor edit = settings_prefs.edit();
            edit.putInt("chat_strings", currentValue);
            edit.apply();

            // save settings
            Utils.saveSettings(getContext());
        });

        fontsize.setOnEditorActionListener((textView, i, keyEvent) -> {
            if (i == EditorInfo.IME_ACTION_DONE || i == EditorInfo.IME_ACTION_NEXT) {
                fontsize.setText(textView.getText().toString());

                // save settings prefs
                SharedPreferences.Editor edit = settings_prefs.edit();
                edit.putFloat("font_size", textView.getText().toString().equals("") ? 28.0f : Float.parseFloat(textView.getText().toString()));
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());

                return true;
            }

            return false;
        });

        fontsize.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                // save settings prefs
                SharedPreferences.Editor edit = settings_prefs.edit();
                edit.putFloat("font_size", Objects.requireNonNull(fontsize.getText()).toString().equals("") ? 28.0f : Float.parseFloat(fontsize.getText().toString()));
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });

        chatposx.setOnEditorActionListener((textView, i, keyEvent) -> {
            if (i == EditorInfo.IME_ACTION_DONE || i == EditorInfo.IME_ACTION_NEXT) {
                chatposx.setText(textView.getText().toString());

                // save settings prefs
                SharedPreferences.Editor edit = settings_prefs.edit();
                edit.putInt("chat_posx", Integer.parseInt(textView.getText().toString().equals("") ? "100" : textView.getText().toString()));
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());

                return true;
            }

            return false;
        });

        chatposx.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                // save settings prefs
                SharedPreferences.Editor edit = settings_prefs.edit();
                edit.putInt("chat_posx", Integer.parseInt(Objects.requireNonNull(chatposx.getText()).toString().equals("") ? "100" : chatposx.getText().toString()));
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });

        chatposy.setOnEditorActionListener((textView, i, keyEvent) -> {
            if (i == EditorInfo.IME_ACTION_DONE || i == EditorInfo.IME_ACTION_NEXT) {
                chatposy.setText(textView.getText().toString());

                // save settings prefs
                SharedPreferences.Editor edit = settings_prefs.edit();
                edit.putInt("chat_posy", Integer.parseInt(textView.getText().toString().equals("") ? "10" : textView.getText().toString()));
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());

                return true;
            }

            return false;
        });

        chatposy.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                // save settings prefs
                SharedPreferences.Editor edit = settings_prefs.edit();
                edit.putInt("chat_posy", Integer.parseInt(Objects.requireNonNull(chatposy.getText()).toString().equals("") ? "10" : chatposy.getText().toString()));
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });

        chatsizex.setOnEditorActionListener((textView, i, keyEvent) -> {
            if (i == EditorInfo.IME_ACTION_DONE || i == EditorInfo.IME_ACTION_NEXT) {
                chatsizex.setText(textView.getText().toString());

                // save settings prefs
                SharedPreferences.Editor edit = settings_prefs.edit();
                edit.putInt("chat_sizex", Integer.parseInt(textView.getText().toString().equals("") ? "400" : textView.getText().toString()));
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());

                return true;
            }

            return false;
        });

        chatsizex.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                // save settings prefs
                SharedPreferences.Editor edit = settings_prefs.edit();
                edit.putInt("chat_sizex", Integer.parseInt(Objects.requireNonNull(chatsizex.getText()).toString().equals("") ? "400" : chatsizex.getText().toString()));
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });

        chatsizey.setOnEditorActionListener((textView, i, keyEvent) -> {
            if (i == EditorInfo.IME_ACTION_DONE || i == EditorInfo.IME_ACTION_NEXT) {
                chatsizey.setText(textView.getText().toString());

                // save settings prefs
                SharedPreferences.Editor edit = settings_prefs.edit();
                edit.putInt("chat_sizey", Integer.parseInt(textView.getText().toString().equals("") ? "150" : textView.getText().toString()));
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());

                return true;
            }

            return false;
        });

        chatsizey.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                // save settings prefs
                SharedPreferences.Editor edit = settings_prefs.edit();
                edit.putInt("chat_sizey", Integer.parseInt(Objects.requireNonNull(chatsizey.getText()).toString().equals("") ? "150" : chatsizey.getText().toString()));
                edit.apply();

                // save settings
                Utils.saveSettings(getContext());
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });

        return view;
    }

    private void setWidgetsForSettings() {
        newinterface.setChecked(settings_prefs.getBoolean("new_interface", false));
        systemkeyboard.setChecked(settings_prefs.getBoolean("system_keyboard", true));
        timestamp.setChecked(settings_prefs.getBoolean("timestamp", false));
        fullscreen.setChecked(settings_prefs.getBoolean("fullscreen", false));
        displayfps.setChecked(settings_prefs.getBoolean("display_fps", false));
        voice.setChecked(settings_prefs.getBoolean("voice_chat", false));
        fastconnect.setChecked(settings_prefs.getBoolean("fast_connect", false));
        cleo.setChecked(settings_prefs.getBoolean("cleo_scripts", false));
        aml.setChecked(settings_prefs.getBoolean("aml_scripts", false));
        monet.setChecked(settings_prefs.getBoolean("monet_scripts", false));
        modloader.setChecked(settings_prefs.getBoolean("modloader", false));
        modify_files.setChecked(settings_prefs.getBoolean("modify_files", false));

        data_full.setChecked(settings_prefs.getString("files_type", "none").equals("full"));
        data_lite.setChecked(settings_prefs.getString("files_type", "none").equals("lite"));

        if (settings_prefs.getInt("fps_limit", 60) == 60) {
            fpslimit_layout.check(R.id.fpslimit_60);
        } else if (settings_prefs.getInt("fps_limit", 60) == 90) {
            fpslimit_layout.check(R.id.fpslimit_90);
        } else if (settings_prefs.getInt("fps_limit", 60) == 120) {
            fpslimit_layout.check(R.id.fpslimit_120);
        } else {
            fpslimit_layout.check(R.id.fpslimit_60);
        }

        chat_minus.setChecked(false);
        chat_strings.setText(String.valueOf(settings_prefs.getInt("chat_strings", 5)));
        chat_plus.setChecked(false);

        fontsize.setText(String.valueOf(settings_prefs.getFloat("font_size", 28.0f)));

        chatposx.setText(String.valueOf(settings_prefs.getInt("chat_posx", 100)));
        chatposy.setText(String.valueOf(settings_prefs.getInt("chat_posy", 10)));

        chatsizex.setText(String.valueOf(settings_prefs.getInt("chat_sizex", 400)));
        chatsizey.setText(String.valueOf(settings_prefs.getInt("chat_sizey", 150)));
    }
}
