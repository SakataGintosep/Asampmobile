package com.newgamersrp.launcher.ui.fragment;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.navigation.NavController;
import androidx.navigation.NavOptions;
import androidx.navigation.Navigation;

import com.joom.paranoid.Obfuscate;
import com.newgamersrp.game.R;

@Obfuscate
public class ServersPageFragment extends Fragment {

    public ServersPageFragment() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_servers_page, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        NavController navController = Navigation.findNavController(view.findViewById(R.id.servers_nav_host_fragment));

        Button favorite_servers = view.findViewById(R.id.favorite_servers);
        favorite_servers.setOnClickListener(v -> navController.navigate(R.id.favorite_servers_fragment, Bundle.EMPTY, new NavOptions.Builder().setLaunchSingleTop(true).build()));

        Button hosted_servers = view.findViewById(R.id.hosted_servers);
        hosted_servers.setOnClickListener(v -> navController.navigate(R.id.hosted_servers_fragment, Bundle.EMPTY, new NavOptions.Builder().setLaunchSingleTop(true).build()));
    }
}
