package fr.grame.simpleguidoeditor.drawcommand.command;
import drawcommand.GetFontColorCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
   
public class AndroidCanvasGetFontColorCommand extends GetFontColorCommand implements DrawToCanvas {

  public AndroidCanvasGetFontColorCommand() {
    super();
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
  
}
