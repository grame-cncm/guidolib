package fr.grame.simpleguidoeditor.drawcommand;
import drawcommand.SetOriginCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
   
public class AndroidCanvasSetOriginCommand extends SetOriginCommand implements DrawToCanvas {

  public AndroidCanvasSetOriginCommand(float x, float y) {
    super(x, y);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    int x = (int)(_x * view._GLOBAL_RESCALE_FACTOR);
    int y = (int)(_y * view._GLOBAL_RESCALE_FACTOR);

    canvas.translate(0, 0);
    canvas.translate(x, y);
  }
}