package fr.grame.simpleguidoeditor.drawcommand.command;
import drawcommand.PopPenWidthCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

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
