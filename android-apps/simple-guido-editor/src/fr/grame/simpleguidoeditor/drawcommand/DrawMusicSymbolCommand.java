package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class DrawMusicSymbolCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public float _x;
  public float _y;
  public int _inSymbolID;

  public DrawMusicSymbolCommand(float x, float y, int inSymbolID) {
    super();
    _x = x;
    _y = y;
    _inSymbolID = inSymbolID;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("DrawMusicSymbol");
    out.append(" ");
    out.append(_x);
    out.append(" ");
    out.append(_y);
    out.append(" ");
    out.append(_inSymbolID);
    return out.toString();
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
