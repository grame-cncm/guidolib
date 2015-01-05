package fr.grame.simpleguidoeditor.drawcommand.command;
import drawcommand.SetScaleCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
   
public class AndroidCanvasSetScaleCommand extends SetScaleCommand implements DrawToCanvas {

  public AndroidCanvasSetScaleCommand(float x, float y) {
    super(x, y);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    canvas.scale(_x, _y);
  }
  
}
