package com.newgamersrp.game;

import android.text.Html;
import android.text.Spanned;

import java.util.LinkedList;
import java.util.regex.Pattern;

import com.joom.paranoid.Obfuscate;

@Obfuscate
public class Utils {
    public static Spanned transformColors(String str) {
        LinkedList linkedList = new LinkedList();
        int i2 = 0;
        String str2 = str;
        int i3 = 0;
        for (int i4 = 0; i4 < str2.length(); i4++) {
            if (str2.charAt(i4) == '{') {
                int i = i4 + 7;
                if (i < str2.length()) {
                    int i52 = i4 + 1;
                    String sb = "#" + str2.substring(i52, i);
                    linkedList.addLast(sb);
                    str2 = str2.substring(0, i52) + "repl" + i3 + str2.substring(i);
                    i3++;
                }
            }
        }
        for (Object o : linkedList) {
            String str3 = (String) o;
            if (i2 == 0) {
                str2 = str2.replaceAll(Pattern.quote("{repl" + i2 + "}"), "<font color='" + str3 + "'>");
            } else {
                str2 = str2.replaceAll(Pattern.quote("{repl" + i2 + "}"), "</font><font color='" + str3 + "'>");
            }
            i2++;
        }
        if (linkedList.size() >= 1) {
            str2 = str2 + "</font>";
        }
        return Html.fromHtml(str2.replaceAll(Pattern.quote("\n"), "<br>"));
    }
}
