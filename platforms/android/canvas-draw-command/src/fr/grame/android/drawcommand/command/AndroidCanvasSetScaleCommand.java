package fr.grame.android.drawcommand.command;
import drawcommand.SetScaleCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

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
