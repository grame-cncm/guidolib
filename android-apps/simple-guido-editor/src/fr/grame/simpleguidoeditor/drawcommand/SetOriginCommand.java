package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class SetOriginCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public float _x;
  public float _y;

  public SetOriginCommand(float x, float y) {
    super();
    _x = x;
    _y = y;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("SetOrigin");
    out.append(" ");
    out.append(_x);
    out.append(" ");
    out.append(_y);
    return out.toString();
  }
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    float x = _x * (float) view._GLOBAL_RESCALE_FACTOR;
    float y = _y * (float) view._GLOBAL_RESCALE_FACTOR;
    canvas.translate(x, y);
  }
}
