package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import android.graphics.Paint;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.PathShape;
import android.graphics.Path;

import java.util.*;
   
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
    Paint paint = view.currentPaint();
    paint.setStyle(Paint.Style.STROKE);
    canvas.drawLine(_x1, _y1, _x2, _y2, paint);
    view.resetTransformMatrix(canvas);
  }
}
