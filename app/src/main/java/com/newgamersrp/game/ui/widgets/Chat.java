package com.newgamersrp.game.ui.widgets;

import android.app.Activity;
import android.text.SpannableString;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;
import java.util.List;

import com.joom.paranoid.Obfuscate;
import com.newgamersrp.game.R;
import com.newgamersrp.game.Utils;

@Obfuscate
public class Chat {
    private final Activity activity;
    private boolean visible;

    private final List<SpannableString> messageList;
    private final ChatAdapter chatAdapter;

    private final RelativeLayout chatLayout;
    private final RecyclerView recyclerView;

    public interface Listener {
        void chatShowKeyboard(boolean z);
        void _chatSend(String str);
    }

    private Listener listener;

    public Chat(Activity activity, Listener listener) {
        this.activity = activity;
        this.listener = listener;

        visible = false;

        messageList = new ArrayList<>();
        chatAdapter = new ChatAdapter(messageList);

        chatLayout = (RelativeLayout) activity.getLayoutInflater().inflate(R.layout.activity_game_chat, null);
        activity.addContentView(chatLayout, new RelativeLayout.LayoutParams(-1, -1));

        LinearLayout chat = chatLayout.findViewById(R.id.chat);
        chat.setOnClickListener(v -> listener.chatShowKeyboard(true));

        recyclerView = chatLayout.findViewById(R.id.recyclerview_chat);
        recyclerView.setLayoutManager(new LinearLayoutManager(activity));
        recyclerView.setAdapter(chatAdapter);

        chatLayout.setVisibility(View.GONE);
    }

    public boolean isVisible() {
        return visible;
    }

    public void show() {
        chatLayout.setVisibility(View.VISIBLE);
    }

    public void onSend(String keyboardString) {
        if (!keyboardString.isEmpty()) {
            listener._chatSend(keyboardString);
            listener.chatShowKeyboard(false);
        }
    }

    public void addMessage(SpannableString message) {
        messageList.add(message);
        chatAdapter.notifyItemInserted(messageList.size() - 1);
        recyclerView.smoothScrollToPosition(messageList.size() - 1);
    }

    private static class ChatAdapter extends RecyclerView.Adapter<ChatAdapter.MessageViewHolder> {
        private List<SpannableString> messageList;

        public ChatAdapter(List<SpannableString> messageList) {
            this.messageList = messageList;
        }

        @NonNull
        @Override
        public MessageViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
            View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.layout_chat_item, parent, false);
            return new MessageViewHolder(view);
        }

        @Override
        public void onBindViewHolder(@NonNull MessageViewHolder holder, int position) {
            SpannableString message = messageList.get(position);
            holder.bind(message);
        }

        @Override
        public int getItemCount() {
            return messageList.size();
        }

        public static class MessageViewHolder extends RecyclerView.ViewHolder {
            private TextView messageTextView;

            public MessageViewHolder(View itemView) {
                super(itemView);
                messageTextView = itemView.findViewById(R.id.textview_message);
                //OutlineTextWatcher textWatcher = new OutlineTextWatcher(messageTextView, 2f, Color.BLACK);
                //messageTextView.addTextChangedListener(textWatcher);
            }

            public void bind(SpannableString message) {
                messageTextView.setText(Utils.transformColors(message.toString()));
            }
        }
    }
}