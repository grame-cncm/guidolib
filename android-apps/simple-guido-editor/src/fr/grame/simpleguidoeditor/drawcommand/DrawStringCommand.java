package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import android.graphics.Paint;

import java.util.*;
   
public class DrawStringCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public float _x;
  public float _y;
  public String _s;
  public int _inCharCount;

  public DrawStringCommand(float x, float y, String s, int inCharCount) {
    super();
    _x = x;
    _y = y;
    _s = s;
    _inCharCount = inCharCount;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("DrawString");
    out.append(" ");
    out.append(_x);
    out.append(" ");
    out.append(_y);
    out.append(" ");
    out.append(_s);
    out.append(" ");
    out.append(_inCharCount);
    return out.toString();
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    double x = _x;
    double y = _y;
    double size = view._MUSIC_FONT_SIZE;

    view.correctTransformMatrix(canvas);

    Paint paint = new Paint();
    paint.setTypeface(view._TEXT_FONT);
    paint.setColor(view._FONT_COLOR);
    paint.setTextSize((float)(view._TEXT_FONT_SIZE));

    canvas.drawText(_s, (float) x, (float) y, paint);
    view.resetTransformMatrix(canvas);
  }

}
