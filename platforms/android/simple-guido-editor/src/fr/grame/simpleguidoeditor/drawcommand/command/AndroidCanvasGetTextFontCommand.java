package fr.grame.simpleguidoeditor.drawcommand.command;
import drawcommand.GetTextFontCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
   
public class AndroidCanvasGetTextFontCommand extends GetTextFontCommand implements DrawToCanvas {

  public AndroidCanvasGetTextFontCommand() {
    super();
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
