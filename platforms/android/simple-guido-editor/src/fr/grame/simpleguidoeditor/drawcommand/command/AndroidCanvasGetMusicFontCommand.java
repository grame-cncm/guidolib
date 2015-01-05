package fr.grame.simpleguidoeditor.drawcommand.command;
import drawcommand.GetMusicFontCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
   
public class AndroidCanvasGetMusicFontCommand extends GetMusicFontCommand implements DrawToCanvas {

  public AndroidCanvasGetMusicFontCommand() {
    super();
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
