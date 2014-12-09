package fr.grame.simpleguidoeditor.drawcommand;
import drawcommand.EndDrawCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
   
public class AndroidCanvasEndDrawCommand extends EndDrawCommand implements DrawToCanvas {

  public AndroidCanvasEndDrawCommand() {
    super();
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
