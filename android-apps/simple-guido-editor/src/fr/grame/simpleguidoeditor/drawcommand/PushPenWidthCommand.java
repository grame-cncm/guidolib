package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class PushPenWidthCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public float _width;

  public PushPenWidthCommand(float width) {
    super();
    _width = width;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("PushPenWidth");
    out.append(" ");
    out.append(_width);
    return out.toString();
  }
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view._PEN_WIDTHS.add((double)_width);
  }
}
