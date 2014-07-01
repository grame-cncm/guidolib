package fr.grame.simpleguidoeditor;

import guidoengine.*;

import android.widget.EditText;

import android.graphics.Paint;
import android.graphics.Path;

import java.util.*;
import java.lang.StringBuilder;
import android.util.Log;

import fr.grame.simpleguidoeditor.parser.GuidoBinaryParser;
import fr.grame.simpleguidoeditor.drawcommand.GuidoDrawCommand;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.PathShape;
import android.view.View;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.drawable.shapes.OvalShape;
import android.util.AttributeSet;
import android.util.Log;
import android.graphics.Point;
import android.graphics.Matrix;
import android.graphics.Color;
import android.graphics.Typeface;

import fr.grame.simpleguidoeditor.SimpleGuidoEditor;

public class GuidoCanvasView extends View {
  private List<GuidoDrawCommand> _drawCommands;
  public double _CHAR_MAX;
  public double _GLOBAL_RESCALE_FACTOR;
  public Matrix _CURRENT_TRANSFORM_MATRIX;
  public List<Integer> _PEN_COLORS;
  public int _FONT_COLOR;
  public List<Double> _PEN_WIDTHS;
  public Typeface _MUSIC_FONT;
  public int _MUSIC_FONT_SIZE;
  public Typeface _TEXT_FONT;
  public int _TEXT_FONT_SIZE;

  public void correctTransformMatrix(Canvas canvas) {
    canvas.save();
    canvas.scale((float) _GLOBAL_RESCALE_FACTOR, (float)_GLOBAL_RESCALE_FACTOR);
  }

  public void resetTransformMatrix(Canvas canvas) {
    canvas.restore();
  }

  public void do_inits() {
    _drawCommands = null;
    _CHAR_MAX = 255.0;
    _GLOBAL_RESCALE_FACTOR = 1.0;
    _CURRENT_TRANSFORM_MATRIX = new Matrix();
    _PEN_COLORS = new ArrayList<Integer>();
    _FONT_COLOR = Color.argb(255,0,0,0);
    _PEN_WIDTHS = new ArrayList<Double>();
    _MUSIC_FONT = Typeface.create("Guido2", Typeface.NORMAL);
    _MUSIC_FONT_SIZE = 12;
    _TEXT_FONT = Typeface.create("Times", Typeface.NORMAL);
    _TEXT_FONT_SIZE = 12;
  }

  public void print_matrix_coords(Canvas canvas) {
    float[] matrix = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    canvas.getMatrix().getValues(matrix);
    StringBuilder sb = new StringBuilder();
    sb.append("current matrix:");
    for (int i = 0; i < 9; i++) {
      sb.append(" ");
      sb.append(matrix[i]);
    }
    Log.i("SimpleGuidoEditor", sb.toString());
  }

  private static byte[] gmntobinary(String gmn) {
      guidoscore gmnscore = new guidoscore();
      int err = gmnscore.ParseString(gmn);
      err = gmnscore.AR2GR();
      err = gmnscore.ResizePageToMusic();
      byte[] out = gmnscore.BinaryExport(1);
      gmnscore.close();
      return out;
  }

  public void generateGuidoCanvas() {
    if (SimpleGuidoEditor._gmn != null) {
      byte[] binary = gmntobinary(SimpleGuidoEditor._gmn);
      if (binary.length > 0) {
        _drawCommands = GuidoBinaryParser.parseIntoDrawCommands(binary);
      }
    }
  }

  public GuidoCanvasView(Context context, AttributeSet attributeSet) {
    super(context);
    do_inits();
  }
  
  @Override
  protected void onDraw(Canvas canvas) {
    if (_drawCommands != null) {
      for (int i = 0; i < _drawCommands.size(); i++) {
        //Log.i("SimpleGuidoEditor", _drawCommands.get(i).asString());
        _drawCommands.get(i).drawToCanvas(canvas, this);
      }
    }
  }
}
