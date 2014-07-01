package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
import android.graphics.Color;

import java.util.*;
   
public class SelectPenColorCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public char _alpha;
  public char _red;
  public char _green;
  public char _blue;

  public SelectPenColorCommand(char alpha, char red, char green, char blue) {
    super();
    _alpha = alpha;
    _red = red;
    _green = green;
    _blue = blue;
  }
  
  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("SelectPenColor");
    out.append(" ");
    out.append(_alpha);
    out.append(" ");
    out.append(_red);
    out.append(" ");
    out.append(_green);
    out.append(" ");
    out.append(_blue);
    return out.toString();
  }
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view._PEN_COLORS.add(Color.argb(_alpha, _red, _green, _blue));
  }

}
