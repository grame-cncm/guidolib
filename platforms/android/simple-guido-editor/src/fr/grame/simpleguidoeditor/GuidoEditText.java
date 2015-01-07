package fr.grame.simpleguidoeditor;

import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.AttributeSet;
import android.widget.EditText;

/**
 * View with the text edit to type gmn code.
 *
 */
public class GuidoEditText extends EditText implements TextWatcher {
	/** Listener have to be informed on gmn code change */
	private OnGmnCodeChangeListener listener;
	private String gmnCode;

	public GuidoEditText(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
		try {
			listener = (OnGmnCodeChangeListener) context;
		} catch (ClassCastException e) {
			throw new ClassCastException(context.toString() + " must implement OnGmnCodeChangeListener");
		}
		if (gmnCode != null)
			listener.setGmnCode(gmnCode);
	}

	@Override
	public void afterTextChanged(Editable s) {
	}

	@Override
	public void beforeTextChanged(CharSequence s, int start, int count, int after) {
	}

	@Override
	public void onTextChanged(CharSequence s, int start, int before, int count) {
		gmnCode = s.toString();
		// this method can be called before the listener is inject
		if (listener != null)
			listener.setGmnCode(gmnCode);
	}
}
