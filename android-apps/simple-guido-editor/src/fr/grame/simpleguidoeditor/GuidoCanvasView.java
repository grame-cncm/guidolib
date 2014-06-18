package fr.grame.simpleguidoeditor;

import guidoengine.*;

import android.widget.EditText;

import android.graphics.Paint;

import java.util.*;
import android.util.Log;

import fr.grame.simpleguidoeditor.parser.GuidoBinaryParser;
import fr.grame.simpleguidoeditor.drawcommand.GuidoDrawCommand;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import android.graphics.drawable.ShapeDrawable;
import android.view.View;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.drawable.shapes.OvalShape;
import android.util.AttributeSet;
import android.util.Log;
import android.graphics.Matrix;
import android.graphics.Color;
import android.graphics.Typeface;

import fr.grame.simpleguidoeditor.SimpleGuidoEditor;

public class GuidoCanvasView extends View {
  private List<GuidoDrawCommand> _drawCommands;
  public double _CHAR_MAX;
  public double _GLOBAL_RESCALE_FACTOR;
  public Matrix _CURRENT_TRANSFORM_MATRIX;
  public List<Color> _PEN_COLORS;
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

  private void do_inits() {
    _CHAR_MAX = 255.0;
    _GLOBAL_RESCALE_FACTOR = 1.0;
    _CURRENT_TRANSFORM_MATRIX = new Matrix();
    _PEN_COLORS = new ArrayList<Color>();
    _FONT_COLOR = Color.argb(255,0,0,0);
    _PEN_WIDTHS = new ArrayList<Double>();
    _MUSIC_FONT = Typeface.create("Guido2", Typeface.NORMAL);
    _MUSIC_FONT_SIZE = 12;
    _TEXT_FONT = Typeface.create("Times", Typeface.NORMAL);
    _TEXT_FONT_SIZE = 12;
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
        /*
        for (int i = 0; i < _drawCommands.size(); i++) {
          Log.i("SimpleGuidoEditor", _drawCommands.get(i).asString());
        }
        */
      }
    }
  }

  public Paint currentPaint() {
    Paint out = new Paint();
    double current_pen_width = 1.0;
    for (int i = 0; i < _PEN_WIDTHS.size(); i++) {
      current_pen_width = _PEN_WIDTHS.get(i);
    }
    out.setStrokeWidth((float)current_pen_width);
    return out;
  }

  public GuidoCanvasView(Context context, AttributeSet attributeSet) {
    super(context);
    do_inits();

    Log.i("SimpleGuidoEditor", "GUIDO DRAWABLE VIEW BEING CREATED");
  }

  @Override
  protected void onDraw(Canvas canvas) {
    Log.i("SimpleGuidoEditor", "DRAWING");
    for (int i = 0; i < _drawCommands.size(); i++) {
      _drawCommands.get(i).drawToCanvas(canvas, this);
    }
  }
}
