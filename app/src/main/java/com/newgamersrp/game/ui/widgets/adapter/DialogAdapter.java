package com.newgamersrp.game.ui.widgets.adapter;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.RecyclerView;

import com.google.android.material.imageview.ShapeableImageView;
import com.google.android.material.textview.MaterialTextView;
import com.joom.paranoid.Obfuscate;

import java.util.ArrayList;

import com.newgamersrp.game.R;
import com.newgamersrp.game.Utils;

@Obfuscate
public class DialogAdapter extends RecyclerView.Adapter<DialogAdapter.ViewHolder> {
    private int selectedPosition = 0;
    private View selectedView;
    private final ArrayList<MaterialTextView> fieldHeaders;
    private final ArrayList<String> fieldTexts;
    private final ArrayList<ArrayList<MaterialTextView>> fields;
    private OnClickListener onClickListener;
    private OnDoubleClickListener onDoubleClickListener;

    public interface OnClickListener {
        void onClick(int i, String str);
    }

    public interface OnDoubleClickListener {
        void onDoubleClick();
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        public ShapeableImageView fieldBg;
        public ArrayList<MaterialTextView> fields = new ArrayList<>();
        private final View view;

        public ViewHolder(View itemView) {
            super(itemView);
            view = itemView;
            fieldBg = itemView.findViewById(R.id.dialog_item_bg);
            ConstraintLayout field = itemView.findViewById(R.id.dialog_item);
            for (int i = 1; i < field.getChildCount(); i++) {
                MaterialTextView childView = (MaterialTextView) field.getChildAt(i);
                childView.setVisibility(View.GONE);
                fields.add(childView);
            }
        }

        public View getView() {
            return view;
        }
    }

    public DialogAdapter(ArrayList<String> fields, ArrayList<MaterialTextView> fieldHeaders) {
        fieldTexts = fields;
        this.fieldHeaders = fieldHeaders;
        this.fields = new ArrayList<>();
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        return new ViewHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.layout_dialog_item, parent, false));
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        String[] headers = fieldTexts.get(position).split("\t");
        ArrayList<MaterialTextView> fields = new ArrayList<>();
        for (int i = 0; i < headers.length; i++) {
            MaterialTextView field = holder.fields.get(i);
            field.setText(Utils.transformColors(headers[i].replace("\\t", "")));
            field.setVisibility(View.VISIBLE);
            fields.add(field);
        }
        this.fields.add(fields);
        if (selectedPosition == position) {
            ShapeableImageView imageView = holder.fieldBg;
            selectedView = imageView;
            imageView.setVisibility(View.VISIBLE);
            onClickListener.onClick(position, holder.fields.get(0).getText().toString());
        } else {
            holder.fieldBg.setVisibility(View.GONE);
        }

        holder.getView().setOnClickListener(view -> {
            if (selectedPosition != holder.getAdapterPosition()) {
                View view2 = selectedView;
                if (view2 != null) {
                    view2.setVisibility(View.GONE);
                }
                selectedPosition = holder.getAdapterPosition();
                selectedView = holder.fieldBg;
                holder.fieldBg.setVisibility(View.VISIBLE);
                onClickListener.onClick(holder.getAdapterPosition(), holder.fields.get(0).getText().toString());
                return;
            }
            OnDoubleClickListener onDoubleClickListener = this.onDoubleClickListener;
            if (onDoubleClickListener != null) {
                onDoubleClickListener.onDoubleClick();
            }
        });
    }

    public void updateSizes() {
        int[] max = new int[4];
        for (int i = 0; i < fields.size(); i++) {
            for (int j = 0; j < fields.get(i).size(); j++) {
                int width = fields.get(i).get(j).getWidth();
                if (max[j] < width) {
                    max[j] = width;
                }
            }
        }
        for (int i2 = 0; i2 < max.length; i2++) {
            int headerWidth = fieldHeaders.get(i2).getWidth();
            Log.i("DIALOG", max[i2] + "\t" + ((Object) fieldHeaders.get(i2).getText()) + " " + headerWidth);
            if (max[i2] < headerWidth) {
                max[i2] = headerWidth;
            }
        }
        for (int i3 = 0; i3 < fields.size(); i3++) {
            for (int j2 = 0; j2 < fields.get(i3).size(); j2++) {
                fields.get(i3).get(j2).setWidth(max[j2]);
            }
        }
        for (int i4 = 0; i4 < fieldHeaders.size(); i4++) {
            fieldHeaders.get(i4).setWidth(max[i4]);
        }
    }

    public void setOnClickListener(OnClickListener onClickListener) {
        this.onClickListener = onClickListener;
    }

    public void setOnDoubleClickListener(OnDoubleClickListener onDoubleClickListener) {
        this.onDoubleClickListener = onDoubleClickListener;
    }

    public ArrayList<ArrayList<MaterialTextView>> getFields() {
        return fields;
    }

    public int getItemCount() {
        return fieldTexts.size();
    }
}
