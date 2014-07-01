package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Color;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
import android.util.Log;

   public class LineCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public float _x1;
  public float _y1;
  public float _x2;
  public float _y2;

  public LineCommand(float x1, float y1, float x2, float y2) {
    super();
    _x1 = x1;
    _y1 = y1;
    _x2 = x2;
    _y2 = y2;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("Line");
    out.append(" ");
    out.append(_x1);
    out.append(" ");
    out.append(_y1);
    out.append(" ");
    out.append(_x2);
    out.append(" ");
    out.append(_y2);
    return out.toString();
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    
    view.correctTransformMatrix(canvas);
    float x1 = (float)(_x1);
    float y1 = (float)(_y1);
    float x2 = (float)(_x2);
    float y2 = (float)(_y2);
    float width = Math.abs(x2 - x1);
    float height = Math.abs(y2 - y1);
    double pen_width = view._PEN_WIDTHS.get(view._PEN_WIDTHS.size() - 1);

    Paint paint = new Paint();
    
    paint.setColor(view._PEN_COLORS.get(view._PEN_COLORS.size() - 1));
    paint.setStyle(Paint.Style.STROKE);
    paint.setStrokeWidth((float) pen_width);

    canvas.drawLine(x1, y1, x2, y2, paint);
    
    view.resetTransformMatrix(canvas);
  }
}
