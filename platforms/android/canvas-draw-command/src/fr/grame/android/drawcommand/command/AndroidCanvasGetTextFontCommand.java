package fr.grame.android.drawcommand.command;
import drawcommand.GetTextFontCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
   
public class AndroidCanvasGetTextFontCommand extends GetTextFontCommand implements DrawToCanvas {

  public AndroidCanvasGetTextFontCommand() {
    super();
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
