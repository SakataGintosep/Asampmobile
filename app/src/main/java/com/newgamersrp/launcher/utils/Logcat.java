package com.newgamersrp.launcher.utils;

import com.joom.paranoid.Obfuscate;

import java.io.File;
import java.io.IOException;

@Obfuscate
public class Logcat {
    public static void save() {
        File log_file = new File(Utils.EXTERNAL_DIR, "Alyn_SAMPMOBILE_log.txt");

        if (log_file.exists()) {
            log_file.delete();
        }

        try {
            Runtime.getRuntime().exec("logcat -f " + log_file);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}