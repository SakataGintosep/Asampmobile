package com.newgamersrp.launcher.utils;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.util.Log;

import com.joom.paranoid.Obfuscate;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

@Obfuscate
public class SignatureChecker {
    public static boolean isSignatureValid(Context ctx, String packageName) {
        try {
            PackageInfo packageInfo = getPackageInfo(ctx, packageName);
            if (packageInfo != null) {
                String appSignature = getSignature(packageInfo);
                Log.d("SignatureChecker", "App Signature: " + appSignature);

                String expectedSignature = "49e194660dc709d044d11048a55825366ac1e5b2c6a07958eea2b4a845712a93";

                assert appSignature != null;
                return appSignature.equalsIgnoreCase(expectedSignature);
            }
        } catch (NameNotFoundException | NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        return false;
    }

    private static PackageInfo getPackageInfo(Context ctx, String packageName) throws NameNotFoundException {
        PackageManager pm = ctx.getPackageManager();
        return pm.getPackageInfo(packageName, PackageManager.GET_SIGNATURES);
    }

    private static String getSignature(PackageInfo packageInfo) throws NoSuchAlgorithmException {
        Signature[] signatures = packageInfo.signatures;
        MessageDigest md = MessageDigest.getInstance("SHA-256");
        for (Signature signature : signatures) {
            byte[] signatureBytes = signature.toByteArray();
            byte[] digest = md.digest(signatureBytes);
            return byteArrayToHexString(digest);
        }
        return null;
    }

    private static String byteArrayToHexString(byte[] array) {
        StringBuilder sb = new StringBuilder(array.length * 2);
        for (byte b : array) {
            sb.append(String.format("%02x", b));
        }
        return sb.toString();
    }
}
