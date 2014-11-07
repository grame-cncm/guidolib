package fr.grame.simpleguidoeditor.drawcommand;
import guidoengine.drawcommand.PopPenWidthCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class AndroidCanvasPopPenWidthCommand extends PopPenWidthCommand implements DrawToCanvas {

  public AndroidCanvasPopPenWidthCommand() {
    super();
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view._PEN_WIDTHS.remove(view._PEN_WIDTHS.size() - 1);
  }
}
