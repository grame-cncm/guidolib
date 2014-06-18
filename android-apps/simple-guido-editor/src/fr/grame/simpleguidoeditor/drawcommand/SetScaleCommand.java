package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class SetScaleCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public float _x;
  public float _y;

  public SetScaleCommand(float x, float y) {
    super();
    _x = x;
    _y = y;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("SetScale");
    out.append(" ");
    out.append(_x);
    out.append(" ");
    out.append(_y);
    return out.toString();
  }
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    canvas.scale(_x, _y);
  }
  
}
