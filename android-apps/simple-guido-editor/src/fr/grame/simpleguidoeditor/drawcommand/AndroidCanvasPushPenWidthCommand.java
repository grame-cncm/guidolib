package fr.grame.simpleguidoeditor.drawcommand;
import guidoengine.drawcommand.PushPenWidthCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class AndroidCanvasPushPenWidthCommand extends PushPenWidthCommand implements DrawToCanvas {

  public AndroidCanvasPushPenWidthCommand(float width) {
    super(width);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view._PEN_WIDTHS.add((double)_width);
  }
}
