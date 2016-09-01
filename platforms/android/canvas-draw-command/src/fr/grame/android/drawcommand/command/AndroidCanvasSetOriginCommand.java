package fr.grame.android.drawcommand.command;
import drawcommand.SetOriginCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
   
public class AndroidCanvasSetOriginCommand extends SetOriginCommand implements DrawToCanvas {

  public AndroidCanvasSetOriginCommand(float x, float y) {
    super(x, y);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    int x = (int)(_x * view.getGlobalRescaleFactor());
    int y = (int)(_y * view.getGlobalRescaleFactor());

    canvas.translate(0, 0);
    canvas.translate(x, y);
  }
}