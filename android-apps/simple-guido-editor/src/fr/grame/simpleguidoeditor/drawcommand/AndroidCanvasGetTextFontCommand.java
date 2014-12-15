package fr.grame.simpleguidoeditor.drawcommand;
import drawcommand.GetTextFontCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
   
public class AndroidCanvasGetTextFontCommand extends GetTextFontCommand implements DrawToCanvas {

  public AndroidCanvasGetTextFontCommand() {
    super();
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
