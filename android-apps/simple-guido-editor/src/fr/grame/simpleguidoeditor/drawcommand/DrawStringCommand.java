package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class DrawStringCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public float _x;
  public float _y;
  public String _s;
  public int _inCharCount;

  public DrawStringCommand(float x, float y, String s, int inCharCount) {
    super();
    _x = x;
    _y = y;
    _s = s;
    _inCharCount = inCharCount;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("DrawString");
    out.append(" ");
    out.append(_x);
    out.append(" ");
    out.append(_y);
    out.append(" ");
    out.append(_s);
    out.append(" ");
    out.append(_inCharCount);
    return out.toString();
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }

}
