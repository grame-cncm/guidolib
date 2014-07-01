package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class PopPenColorCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public PopPenColorCommand() {
    super();
  }

  @Override
  public String asString() {
    return "PopPenColor";
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view._PEN_COLORS.remove(view._PEN_COLORS.size() - 1);
  }
}
