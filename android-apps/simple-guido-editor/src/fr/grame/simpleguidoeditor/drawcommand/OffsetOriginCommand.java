package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
import android.graphics.Point;

import android.util.Log;
import java.util.*;
   
public class OffsetOriginCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public float _x;
  public float _y;

  public OffsetOriginCommand(float x, float y) {
    super();
    _x = x;
    _y = y;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("OffsetOrigin");
    out.append(" ");
    out.append(_x);
    out.append(" ");
    out.append(_y);
    return out.toString();
  }
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    int x = (int)(_x * view._GLOBAL_RESCALE_FACTOR);
    int y = (int)(_y * view._GLOBAL_RESCALE_FACTOR);

    canvas.translate(x, y);
  }
}
