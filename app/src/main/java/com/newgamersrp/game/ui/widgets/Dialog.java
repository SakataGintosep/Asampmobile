package com.newgamersrp.game.ui.widgets;

import android.app.Activity;
import android.content.Context;
import android.text.Editable;
import android.text.InputType;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

import com.google.android.material.textview.MaterialTextView;
import com.joom.paranoid.Obfuscate;

import com.newgamersrp.game.R;
import com.newgamersrp.game.Utils;
import com.newgamersrp.game.ui.widgets.adapter.DialogAdapter;

@Obfuscate
public class Dialog {
    private static final int DIALOG_BUTTON_1_ID = 1;
    private static final int DIALOG_BUTTON_2_ID = 0;

    private static final int DIALOG_STYLE_MSGBOX = 0;
    private static final int DIALOG_STYLE_INPUT = 1;
    private static final int DIALOG_STYLE_LIST = 2;
    private static final int DIALOG_STYLE_PASSWORD = 3;
    private static final int DIALOG_STYLE_TABLIST = 4;
    private static final int DIALOG_STYLE_TABLIST_HEADERS = 5;

    private Activity activity;
    private Listener listener;

    private final ConstraintLayout dialog_layout;
    private final MaterialTextView dialog_title;
    private final MaterialTextView dialog_message;

    private final FrameLayout dialog_button1;
    private final FrameLayout dialog_button2;

    private final EditText dialog_input;
    private final FrameLayout dialog_input_layout;
    private final LinearLayout dialog_list_layout;
    private final ConstraintLayout dialog_message_layout;
    private final RecyclerView dialog_list_recycler;

    private int dialog_id = -1;
    private int dialog_style = -1;
    private String inputString = "";
    private int list_item = -1;

    private final ArrayList<MaterialTextView> headersList;
    private ArrayList<String> rowsList;

    public interface Listener {
        void _sendDialogResponse(int dialog_id, int button_id, int list_item, byte[] input);
    }

    public Dialog(Activity activity, Listener listener) {
        this.activity = activity;
        this.listener = listener;

        dialog_layout = (ConstraintLayout) activity.getLayoutInflater().inflate(R.layout.activity_game_dialog, null);
        activity.addContentView(dialog_layout, new ConstraintLayout.LayoutParams(-1, -1));

        dialog_title = (MaterialTextView) activity.findViewById(R.id.dialog_title);
        dialog_message = (MaterialTextView) activity.findViewById(R.id.dialog_message);

        dialog_button1 = activity.findViewById(R.id.dialog_button1_layout);
        dialog_button2 = activity.findViewById(R.id.dialog_button2_layout);

        dialog_input_layout = activity.findViewById(R.id.dialog_input_layout);
        dialog_list_layout = activity.findViewById(R.id.dialog_list_layout);
        dialog_message_layout = activity.findViewById(R.id.dialog_message_layout);
        dialog_input = activity.findViewById(R.id.dialog_input);
        dialog_list_recycler = activity.findViewById(R.id.dialog_list_recycler);

        dialog_button1.setOnClickListener(view -> sendDialogResponse(DIALOG_BUTTON_1_ID));
        dialog_button2.setOnClickListener(view -> sendDialogResponse(DIALOG_BUTTON_2_ID));

        rowsList = new ArrayList<>();
        headersList = new ArrayList<>();

        LinearLayout headersLayout = activity.findViewById(R.id.dialog_tablist_row);
        for (int i = 0; i < headersLayout.getChildCount(); i++) {
            headersList.add((MaterialTextView) headersLayout.getChildAt(i));
        }

        dialog_input.setOnEditorActionListener((textView, i, keyEvent) -> {
            Editable editableText;
            if ((i != 6 && i != 5) || (editableText = dialog_input.getText()) == null) {
                return false;
            }
            inputString = editableText.toString();
            return false;
        });

        dialog_input.setOnClickListener(view -> {
            dialog_input.requestFocus();
            ((InputMethodManager) activity.getSystemService(Context.INPUT_METHOD_SERVICE)).showSoftInput(dialog_input, 1);
        });

        dialog_layout.setVisibility(View.GONE);
    }

    private static ArrayList<String> fixFields(ArrayList<String> fields) {
        ArrayList<String> newList = new ArrayList<>();
        int max = 0;
        for (int i = 0; i < fields.size(); i++) {
            int len = fields.get(i).split("\t").length;
            if (len > max) {
                max = len;
            }
        }
        for (int i2 = 0; i2 < fields.size(); i2++) {
            StringBuilder field = new StringBuilder(fields.get(i2));
            for (int len2 = fields.get(i2).split("\t").length; len2 != max; len2++) {
                field.append("\\t ");
            }
            newList.add(field.toString());
        }
        return newList;
    }

    public void show(int dialog_id, int dialog_style, String title, String message, String button1, String button2) {
        clearDialog();

        this.dialog_id = dialog_id;
        this.dialog_style = dialog_style;

        if (dialog_style == DIALOG_STYLE_MSGBOX) {
            dialog_input_layout.setVisibility(View.GONE);
            dialog_list_layout.setVisibility(View.GONE);
            dialog_message_layout.setVisibility(View.VISIBLE);
        } else if (dialog_style == DIALOG_STYLE_INPUT || dialog_style == DIALOG_STYLE_PASSWORD) {
            dialog_input.setInputType(dialog_style == DIALOG_STYLE_PASSWORD ? (InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD) : InputType.TYPE_CLASS_TEXT);
            dialog_input_layout.setVisibility(View.VISIBLE);
            dialog_message_layout.setVisibility(View.VISIBLE);
            dialog_list_layout.setVisibility(View.GONE);
        } else { // LIST, TABLIST, TABLIST_HEADER
            dialog_input_layout.setVisibility(View.GONE);
            dialog_message_layout.setVisibility(View.GONE);
            dialog_list_layout.setVisibility(View.VISIBLE);

            loadTabList(message);
            ArrayList<String> fixFieldsForDialog = fixFields(rowsList);
            rowsList = fixFieldsForDialog;

            DialogAdapter adapter = new DialogAdapter(fixFieldsForDialog, headersList);
            adapter.setOnClickListener((i, str) -> {
                list_item = i;
                inputString = str;
            });

            adapter.setOnDoubleClickListener(() -> sendDialogResponse(1));
            dialog_list_recycler.setLayoutManager(new LinearLayoutManager(activity));
            dialog_list_recycler.setAdapter(adapter);
            if (dialog_style != DIALOG_STYLE_LIST) {
                adapter.getClass();
                dialog_list_recycler.post(adapter::updateSizes);
            }
        }

        dialog_title.setText(Utils.transformColors(title));
        dialog_message.setText(Utils.transformColors(message));

        ((MaterialTextView) dialog_button1.getChildAt(0)).setText(Utils.transformColors(button1));
        ((MaterialTextView) dialog_button2.getChildAt(0)).setText(Utils.transformColors(button2));

        if (button2.equals("")) {
            dialog_button2.setVisibility(View.GONE);
        } else {
            dialog_button2.setVisibility(View.VISIBLE);
        }

        dialog_layout.setVisibility(View.VISIBLE);
    }

    public void showWithoutReset(boolean z) {
        if (dialog_id != -1 && dialog_style != -1) {
            dialog_layout.setVisibility(z ? View.VISIBLE : View.GONE);
        }
    }

    public void sendDialogResponse(int button_id) {
        if (!inputString.equals(dialog_input.getText().toString())) {
            inputString = dialog_input.getText().toString();
        }

        ((InputMethodManager) activity.getSystemService(Context.INPUT_METHOD_SERVICE)).hideSoftInputFromWindow(dialog_input.getWindowToken(), 0);

        try {
            listener._sendDialogResponse(dialog_id, button_id, list_item, inputString.getBytes("windows-1251"));
            dialog_layout.setVisibility(View.GONE);
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }

        clearDialog();
    }

    private void loadTabList(String content) {
        String[] strings = content.split("\n");
        for (int i = 0; i < strings.length; i++) {
            if (dialog_style == 5 && i == 0) {
                String[] headers = strings[i].split("\t");
                for (int j = 0; j < headers.length; j++) {
                    headersList.get(j).setText(Utils.transformColors(headers[j]));
                    headersList.get(j).setVisibility(View.VISIBLE);
                }
            } else {
                rowsList.add(strings[i]);
            }
        }
    }

    private void clearDialog() {
        dialog_input.setText("");
        dialog_id = -1;
        dialog_style = -1;
        list_item = -1;
        rowsList.clear();
        for (int i = 0; i < headersList.size(); i++) {
            headersList.get(i).setVisibility(View.GONE);
        }
    }

    public void onHeightChanged(int height) {
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) dialog_layout.getLayoutParams();
        params.setMargins(0, 0, 0, height);
        dialog_layout.setLayoutParams(params);
    }
}
