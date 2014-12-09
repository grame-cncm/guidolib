package fr.grame.simpleguidoeditor;

import fr.grame.simpleguidoeditor.SimpleGuidoEditor;

import android.widget.EditText;

import android.content.Context;
import android.util.AttributeSet;
import android.text.TextWatcher;
import android.text.Editable;

public class GuidoEditText extends EditText implements TextWatcher {

  public GuidoEditText(Context context, AttributeSet attributeSet) {
    super(context, attributeSet);
    if (SimpleGuidoEditor._gmn == null) {
      SimpleGuidoEditor._gmn = getText().toString();
    }
  }

  @Override
  public void afterTextChanged(Editable s) {
  }

  @Override
  public void beforeTextChanged(CharSequence s, int start, int count, int after) {
  }

  @Override
  public void onTextChanged(CharSequence s, int start, int before, int count) {
      SimpleGuidoEditor._gmn = s.toString();
  }
}
