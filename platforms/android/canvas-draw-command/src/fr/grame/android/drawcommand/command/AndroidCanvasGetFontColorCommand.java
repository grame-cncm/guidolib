package fr.grame.android.drawcommand.command;
import drawcommand.GetFontColorCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
   
public class AndroidCanvasGetFontColorCommand extends GetFontColorCommand implements DrawToCanvas {

  public AndroidCanvasGetFontColorCommand() {
    super();
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
  
}
