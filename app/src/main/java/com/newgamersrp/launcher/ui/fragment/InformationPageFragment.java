package com.newgamersrp.launcher.ui.fragment;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.fragment.app.Fragment;

import com.google.android.material.button.MaterialButton;
import com.joom.paranoid.Obfuscate;

import com.newgamersrp.game.R;
import com.newgamersrp.launcher.utils.Utils;

@Obfuscate
public class InformationPageFragment extends Fragment {

    public InformationPageFragment() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_information_page, container, false);

        MaterialButton discord_button = view.findViewById(R.id.discord_button);
        MaterialButton web_button = view.findViewById(R.id.web_button);

        discord_button.setOnClickListener(view1 -> {
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setData(Uri.parse(Utils.discord));
            startActivity(intent);
        });

        web_button.setOnClickListener(view1 -> {
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setData(Uri.parse(Utils.website));
            startActivity(intent);
        });

        return view;
    }
}
