package fr.grame.simpleguidoeditor.drawcommand;
import drawcommand.GetFontColorCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
   
public class AndroidCanvasGetFontColorCommand extends GetFontColorCommand implements DrawToCanvas {

  public AndroidCanvasGetFontColorCommand() {
    super();
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
  
}
