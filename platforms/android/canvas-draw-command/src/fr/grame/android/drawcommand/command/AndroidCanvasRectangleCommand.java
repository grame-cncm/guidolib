package fr.grame.android.drawcommand.command;
import drawcommand.RectangleCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
import android.graphics.Paint;

public class AndroidCanvasRectangleCommand extends RectangleCommand implements DrawToCanvas {

  public AndroidCanvasRectangleCommand(float left, float top, float right, float bottom) {
    super(left, top, right, bottom);
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
    /*double width = Math.abs(right - left);
    double height = Math.abs(top - bottom);*/

    Paint paint = new Paint();
    paint.setAntiAlias(true);
    paint.setColor(view.getCurrentFillColor());
    paint.setStyle(Paint.Style.FILL);
    canvas.drawRect((float)left, (float)top, (float)right, (float)bottom, paint);

    view.resetTransformMatrix(canvas);
  }
}
