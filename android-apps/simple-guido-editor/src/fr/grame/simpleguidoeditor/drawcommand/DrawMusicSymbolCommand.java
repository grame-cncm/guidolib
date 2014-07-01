package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;

import android.util.Log;
import android.graphics.Paint;
import android.graphics.Color;

public class DrawMusicSymbolCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public float _x;
  public float _y;
  public int _inSymbolID;

  public DrawMusicSymbolCommand(float x, float y, int inSymbolID) {
    super();
    _x = x;
    _y = y;
    _inSymbolID = inSymbolID;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("DrawMusicSymbol");
    out.append(" ");
    out.append(_x);
    out.append(" ");
    out.append(_y);
    out.append(" ");
    out.append(_inSymbolID);
    return out.toString();
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    double x = _x;
    double y = _y;
    double size = view._MUSIC_FONT_SIZE;

    view.correctTransformMatrix(canvas);

    Paint paint = new Paint();
    paint.setTypeface(view._MUSIC_FONT);
    paint.setColor(view._FONT_COLOR);
    paint.setTextSize((float)(view._MUSIC_FONT_SIZE));

    StringBuilder mus_glyph_to_string= new StringBuilder(1);
    mus_glyph_to_string.append(Character.toChars(_inSymbolID));
    canvas.drawText(mus_glyph_to_string.toString(), (float) x, (float) y, paint);
    view.resetTransformMatrix(canvas);
  }
}
