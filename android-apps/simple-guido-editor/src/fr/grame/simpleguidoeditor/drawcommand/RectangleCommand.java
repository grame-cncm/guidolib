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
    Log.i("SimpleGuidoEditor", "drawing line");
    view.correctTransformMatrix(canvas);
    ShapeDrawable rect = new ShapeDrawable(new RectShape());
    rect.getPaint().setColor(Color.BLACK);
    rect.setBounds(0, 0, (int) Math.abs(_top - _bottom), (int) Math.abs(_right - _left));
    rect.draw(canvas);
    view.resetTransformMatrix(canvas);
  }
}
