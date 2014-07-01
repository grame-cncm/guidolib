package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import android.graphics.Paint;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.RectShape;

import java.util.*;
import android.util.Log;
import android.graphics.Color;

public class RectangleCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public float _left;
  public float _top;
  public float _right;
  public float _bottom;

  public RectangleCommand(float left, float top, float right, float bottom) {
    super();
    _left = left;
    _top = top;
    _right = right;
    _bottom = bottom;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("Rectangle");
    out.append(" ");
    out.append(_left);
    out.append(" ");
    out.append(_top);
    out.append(" ");
    out.append(_right);
    out.append(" ");
    out.append(_bottom);
    return out.toString();
  }
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view.correctTransformMatrix(canvas);
    double left = _left;
    double right = _right;
    double top = _top;
    double bottom = _bottom;
    // abs should be in theory unnecessary, but corrects
    // just in case guido input is off
    double width = Math.abs(right - left);
    double height = Math.abs(top - bottom);

    Paint paint = new Paint();
    paint.setColor(view._PEN_COLORS.get(view._PEN_COLORS.size() - 1));
    paint.setStyle(Paint.Style.FILL);
    canvas.drawRect((float)left, (float)top, (float)right, (float)bottom, paint);

    view.resetTransformMatrix(canvas);
  }
}
