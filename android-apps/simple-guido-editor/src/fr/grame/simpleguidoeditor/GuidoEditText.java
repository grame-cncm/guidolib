package fr.grame.simpleguidoeditor;

import fr.grame.simpleguidoeditor.SimpleGuidoEditor;

import guidoengine.*;

import android.widget.EditText;

import java.util.*;
import android.util.Log;

import fr.grame.simpleguidoeditor.parser.GuidoBinaryParser;
import fr.grame.simpleguidoeditor.drawcommand.GuidoDrawCommand;

import android.graphics.drawable.ShapeDrawable;
import android.view.View;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.drawable.shapes.OvalShape;
import android.util.AttributeSet;
import android.util.Log;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;

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
