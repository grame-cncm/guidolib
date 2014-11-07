package fr.grame.simpleguidoeditor.drawcommand;
import guidoengine.drawcommand.SetScaleCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class AndroidCanvasSetScaleCommand extends SetScaleCommand implements DrawToCanvas {

  public AndroidCanvasSetScaleCommand(float x, float y) {
    super(x, y);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    canvas.scale(_x, _y);
  }
  
}
