package fr.grame.simpleguidoeditor.drawcommand;
import guidoengine.drawcommand.LineCommand;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Color;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
import android.util.Log;

public class AndroidCanvasLineCommand extends LineCommand implements DrawToCanvas {

  public AndroidCanvasLineCommand(float x1, float y1, float x2, float y2) {
    super(x1, y1, x2, y2);
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
