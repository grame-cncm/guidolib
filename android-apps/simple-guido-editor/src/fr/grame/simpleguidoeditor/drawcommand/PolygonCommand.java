package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
import android.graphics.Path;
import android.graphics.Paint;

import java.util.*;
   
public class PolygonCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public List<Float> _xCoords;
  public List<Float> _yCoords;
  public int _count;

  public PolygonCommand(List<Float> xCoords, List<Float> yCoords, int count) {
    super();
    _xCoords = xCoords;
    _yCoords = yCoords;
    _count = count;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("Polygon");
    for (int i = 0; i < _xCoords.size(); i++) {
      out.append(" ");
      out.append(_xCoords.get(i));
    }
    for (int i = 0; i < _yCoords.size(); i++) {
      out.append(" ");
      out.append(_yCoords.get(i));
    }
    out.append(" ");
    out.append(_count);
    return out.toString();
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
    
    paint.setColor(view._PEN_COLORS.get(view._PEN_COLORS.size() - 1));
    paint.setStyle(Paint.Style.FILL);
    
    canvas.drawPath(path, paint);
    view.resetTransformMatrix(canvas);
  }
}
