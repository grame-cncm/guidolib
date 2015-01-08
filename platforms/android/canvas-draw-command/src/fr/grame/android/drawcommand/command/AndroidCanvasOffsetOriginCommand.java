package fr.grame.android.drawcommand.command;
import drawcommand.OffsetOriginCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
   
public class AndroidCanvasOffsetOriginCommand extends OffsetOriginCommand implements DrawToCanvas {

  public AndroidCanvasOffsetOriginCommand(float x, float y) {
    super(x, y);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    int x = (int)(_x * view.getGlobalRescaleFactor());
    int y = (int)(_y * view.getGlobalRescaleFactor());

    canvas.translate(x, y);
  }
}
