package fr.grame.simpleguidoeditor.drawcommand;
import drawcommand.LineCommand;

import android.graphics.Canvas;
import android.graphics.Paint;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

public class AndroidCanvasLineCommand extends LineCommand implements DrawToCanvas {

  public AndroidCanvasLineCommand(float x1, float y1, float x2, float y2) {
    super(x1, y1, x2, y2);
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    
    view.correctTransformMatrix(canvas);

    Paint paint = new Paint();
    
    paint.setColor(view.getCurrentPenColor());
    paint.setStyle(Paint.Style.STROKE);
    paint.setStrokeWidth(view.getCurrentPenWidth());

    canvas.drawLine(_x1, _y1, _x2, _y2, paint);
    
    view.resetTransformMatrix(canvas);
  }
}
