package fr.grame.android.drawcommand.command;
import drawcommand.PopPenWidthCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
   
public class AndroidCanvasPopPenWidthCommand extends PopPenWidthCommand implements DrawToCanvas {

  public AndroidCanvasPopPenWidthCommand() {
    super();
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.popPenWidth();
  }
}
