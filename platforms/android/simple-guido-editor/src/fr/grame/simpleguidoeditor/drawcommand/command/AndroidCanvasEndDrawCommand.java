package fr.grame.simpleguidoeditor.drawcommand.command;
import drawcommand.EndDrawCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
   
public class AndroidCanvasEndDrawCommand extends EndDrawCommand implements DrawToCanvas {

  public AndroidCanvasEndDrawCommand() {
    super();
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
