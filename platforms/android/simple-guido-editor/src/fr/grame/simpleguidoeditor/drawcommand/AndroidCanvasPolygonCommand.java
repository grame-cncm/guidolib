package fr.grame.simpleguidoeditor.drawcommand;
import drawcommand.PolygonCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
import android.graphics.Path;
import android.graphics.Paint;

import java.util.*;
   
public class AndroidCanvasPolygonCommand extends PolygonCommand implements DrawToCanvas {

  public AndroidCanvasPolygonCommand(List<Float> xCoords, List<Float> yCoords, int count) {
    super(xCoords, yCoords, count);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    Path path = new Path();
    view.correctTransformMatrix(canvas);
    path.moveTo(_xCoords.get(0), _yCoords.get(0));

    for (int i = 1; i < _yCoords.size(); i++) {
      path.lineTo(_xCoords.get(i), _yCoords.get(i));
    }
    
    Paint paint = new Paint();
    
    paint.setColor(view.getCurrentFillColor());
    paint.setStyle(Paint.Style.FILL);
    
    canvas.drawPath(path, paint);
    view.resetTransformMatrix(canvas);
  }
}
