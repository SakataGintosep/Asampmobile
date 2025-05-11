package com.newgamersrp.launcher.utils;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Environment;

import com.joom.paranoid.Obfuscate;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.text.CharacterIterator;
import java.text.StringCharacterIterator;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;

import java.util.function.Consumer;
import java.net.URL;
import java.net.HttpURLConnection;
import java.io.InputStreamReader;
import android.os.Handler;
import android.os.Looper;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

import com.newgamersrp.game.BuildConfig;

@Obfuscate
public class Utils {
    public static File EXTERNAL_DIR = new File(Environment.getExternalStorageDirectory().getAbsolutePath().concat("/Android/data/com.newgamersrp.game/"));

    public static boolean status = false;
    public static boolean beta_status = false;

    public static String copyright = "SCH: Underground Launcher\nCopyright © Alyn_SAMPMOBILE";
    public static String website = "https://whatsapp.com/channel/0029Vb1S3SFLikg3vTxC320x";
    public static String web = "https://raw.githubusercontent.com/RevTer14/server-status/main/";
    public static String update = web + "release.json";
    public static String discord = "https://discord.gg/zb7GN6bkge";
    public static String hostedServersFileStr = web + "server.json";

    public static File serversFile = new File(EXTERNAL_DIR, "servers.json");

    public enum GPUType {
        NONE,
        DXT,
        PVR,
        ETC
    }

    public static GPUType GPU_TYPE = GPUType.NONE;

    public static boolean isOnline(Activity activity) {
        System.out.println("isOnline");
        ConnectivityManager connectivityManager = (ConnectivityManager) activity.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo activeNetworkInfo = connectivityManager != null ? connectivityManager.getActiveNetworkInfo() : null;
        return activeNetworkInfo != null && activeNetworkInfo.isConnectedOrConnecting();
    }

    public static void saveSettings(Context ctx) {
        System.out.println("saveSettings");

        try {
            File file = new File(EXTERNAL_DIR, "files/SAMP/settings.json");

            if (file.exists()) {
                file.delete();
            }

            SharedPreferences settings_prefs = ctx.getSharedPreferences("samp_settings", Context.MODE_PRIVATE);
            SharedPreferences server_prefs = ctx.getSharedPreferences("samp_server", Context.MODE_PRIVATE);

            JSONObject jSONObject = new JSONObject();
            jSONObject.put("client", new JSONObject());

            jSONObject.getJSONObject("client").put("server", new JSONObject());
            jSONObject.getJSONObject("client").getJSONObject("server").put("host", server_prefs.getString("host", "127.0.0.1"));
            jSONObject.getJSONObject("client").getJSONObject("server").put("port", Integer.valueOf(server_prefs.getString("port", "7777")));
            jSONObject.getJSONObject("client").getJSONObject("server").put("password", server_prefs.getString("password", ""));

            jSONObject.getJSONObject("client").put("settings", new JSONObject());
            jSONObject.getJSONObject("client").getJSONObject("settings").put("nick_name", settings_prefs.getString("nick_name", "samp_player"));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("samp_version", settings_prefs.getInt("samp_version", 0));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("new_interface", settings_prefs.getBoolean("new_interface", false));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("system_keyboard", settings_prefs.getBoolean("system_keyboard", true));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("timestamp", settings_prefs.getBoolean("timestamp", false));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("fullscreen", settings_prefs.getBoolean("fullscreen", false));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("fast_connect", settings_prefs.getBoolean("fast_connect", false));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("voice_chat", settings_prefs.getBoolean("voice_chat", false));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("display_fps", settings_prefs.getBoolean("display_fps", false));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("cleo_scripts", settings_prefs.getBoolean("cleo_scripts", false));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("aml_scripts", settings_prefs.getBoolean("aml_scripts", false));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("monet_scripts", settings_prefs.getBoolean("monet_scripts", false));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("modloader", settings_prefs.getBoolean("modloader", false));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("modify_files", settings_prefs.getBoolean("modify_files", false));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("fps_limit", settings_prefs.getInt("fps_limit", 60));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("chat_strings", settings_prefs.getInt("chat_strings", 5));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("font_size", settings_prefs.getFloat("font_size", 28.0f));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("chat_posx", settings_prefs.getInt("chat_posx", 100));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("chat_posy", settings_prefs.getInt("chat_posy", 10));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("chat_sizex", settings_prefs.getInt("chat_sizex", 400));
            jSONObject.getJSONObject("client").getJSONObject("settings").put("chat_sizey", settings_prefs.getInt("chat_sizey", 150));

            file.createNewFile();

            writeJSONToFile(file, jSONObject);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void restoreSettings(Context ctx) {
        System.out.println("restoreSettings");

        SharedPreferences.Editor edit = ctx.getSharedPreferences("samp_settings", Context.MODE_PRIVATE).edit();
        edit.putString("nick_name", "samp_player");
        edit.putInt("samp_version", 0);
        edit.putBoolean("new_interface", false);
        edit.putBoolean("system_keyboard", true);
        edit.putBoolean("timestamp", false);
        edit.putBoolean("fullscreen", false);
        edit.putBoolean("fast_connect", false);
        edit.putBoolean("voice_chat", false);
        edit.putBoolean("display_fps", false);
        edit.putBoolean("cleo_scripts", false);
        edit.putBoolean("aml_scripts", false);
        edit.putBoolean("monet_scripts", false);
        edit.putBoolean("modloader", false);
        edit.putBoolean("modify_files", false);
        edit.putInt("fps_limit", 60);
        edit.putInt("chat_strings", 5);
        edit.putFloat("font_size", 28.0f);
        edit.putInt("chat_posx", 100);
        edit.putInt("chat_posy", 10);
        edit.putInt("chat_sizex", 400);
        edit.putInt("chat_sizey", 150);
        edit.apply();

        saveSettings(ctx);
    }

    public static boolean addServerToFavorites(String address) {
        try {
            File serversFile = new File(EXTERNAL_DIR, "servers.json");

            if (!serversFile.exists()) {
                // initialize servers.json with an empty array
                JSONObject initialJson = new JSONObject();
                initialJson.put("servers", new JSONArray());
                writeJSONToFile(serversFile, initialJson);
            } else {
                if (serversFile.length() == 0) {
                    serversFile.delete();
                }
            }

            String jsonString = readJSONFromFile(serversFile);
            JSONObject json = new JSONObject(jsonString);
            JSONArray serversArray = json.getJSONArray("servers");

            // check if the server already exists in favorites
            for (int i = 0; i < serversArray.length(); i++) {
                JSONObject serverJson = serversArray.getJSONObject(i);
                String host = serverJson.getString("host");
                String port = serverJson.getString("port");

                if (address.equals(host + ":" + port)) {
                    return false; // Server already exists in favorites
                }
            }

            // add the new server to the JSON array
            String host = address.split(":")[0];
            String port = address.split(":")[1];

            JSONObject newServerJson = new JSONObject();
            newServerJson.put("host", host);
            newServerJson.put("port", port);
            serversArray.put(newServerJson);

            // update the JSON object and write it back to the file
            writeJSONToFile(serversFile, json);

            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public static void deleteServerFromFavorites(String address) throws Exception {
        String jsonString = readJSONFromFile(serversFile);
        JSONObject json = new JSONObject(jsonString);
        JSONArray serversArray = json.getJSONArray("servers");

        JSONArray newServersArray = new JSONArray();
        String[] parts = address.split(":");
        String host = parts[0];
        String port = parts[1];

        for (int i = 0; i < serversArray.length(); i++) {
            JSONObject serverJson = serversArray.getJSONObject(i);
            String serverHost = serverJson.getString("host");
            String serverPort = serverJson.getString("port");

            if (!host.equals(serverHost) || !port.equals(serverPort)) {
                newServersArray.put(serverJson);
            }
        }

        json.put("servers", newServersArray);
        writeJSONToFile(serversFile, json);
    }

    public static String readJSONFromFile(File file) throws IOException {
        BufferedReader br = new BufferedReader(new FileReader(file));
        StringBuilder sb = new StringBuilder();
        String line;

        while ((line = br.readLine()) != null) {
            sb.append(line);
        }

        br.close();
        return sb.toString();
    }

    public static void writeJSONToFile(File file, JSONObject json) throws IOException, JSONException {
        BufferedWriter bw = new BufferedWriter(new FileWriter(file));
        bw.write(json.toString(4));
        bw.write(System.lineSeparator());
        bw.close();
    }

    public static String getStringOutputByURL(final String url) throws IOException {
        OkHttpClient client = new OkHttpClient();
        Request request = new Request.Builder()
                .url(url)
                .build();

        try (Response response = client.newCall(request).execute()) {
            if (!response.isSuccessful()) {
                throw new IOException("Unexpected response code: " + response.code());
            }

            assert response.body() != null;
            return response.body().string();
        }
    }

    public static String convertBytesToString(long bytes) {
        if (-1000 < bytes && bytes < 1000) {
            return bytes + " B";
        }
        CharacterIterator ci = new StringCharacterIterator("kMGTPE");
        while (bytes <= -999_950 || bytes >= 999_950) {
            bytes /= 1000;
            ci.next();
        }
        return String.format("%.1f %cB", bytes / 1000.0, ci.current());
    }
    public static void checkServerStatus(Consumer<Boolean> callback) {
        new Thread(() -> {
            boolean serverStatus;
            try {
                URL url = new URL("https://raw.githubusercontent.com/RevTer14/server-status/main/status.json");
                HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                conn.setRequestMethod("GET");
                conn.setRequestProperty("User-Agent", "MyApp");

                int responseCode = conn.getResponseCode();
                if (responseCode != 200) throw new IOException("HTTP Error: " + responseCode);

                BufferedReader br = new BufferedReader(new InputStreamReader(conn.getInputStream()));
                StringBuilder response = new StringBuilder();
                String line;
                while ((line = br.readLine()) != null) {
                    response.append(line);
                }
                br.close();

                JSONObject json = new JSONObject(response.toString());
                serverStatus = json.optBoolean("server_status", false); // Gunakan `optBoolean` biar aman

            } catch (Exception e) {
                e.printStackTrace();
                serverStatus = false;
            }

            boolean finalStatus = serverStatus;
            new Handler(Looper.getMainLooper()).post(() -> callback.accept(finalStatus));
        }).start();
    }
}