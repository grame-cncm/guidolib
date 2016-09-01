package fr.grame.android.drawcommand.command;
import drawcommand.EndDrawCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
   
public class AndroidCanvasEndDrawCommand extends EndDrawCommand implements DrawToCanvas {

  public AndroidCanvasEndDrawCommand() {
    super();
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
