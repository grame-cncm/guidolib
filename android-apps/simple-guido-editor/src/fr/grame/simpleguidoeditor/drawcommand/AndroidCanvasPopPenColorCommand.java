package fr.grame.simpleguidoeditor.drawcommand;
import guidoengine.drawcommand.PopPenColorCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class AndroidCanvasPopPenColorCommand extends PopPenColorCommand implements DrawToCanvas {

  public AndroidCanvasPopPenColorCommand() {
    super();
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view._PEN_COLORS.remove(view._PEN_COLORS.size() - 1);
  }
}
