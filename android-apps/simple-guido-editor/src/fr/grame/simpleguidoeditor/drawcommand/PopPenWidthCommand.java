package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class PopPenWidthCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public PopPenWidthCommand() {
    super();
  }
  @Override
  public String asString() {
    return "PopPenWidth";
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view._PEN_WIDTHS.remove(view._PEN_WIDTHS.size() - 1);
  }
}
