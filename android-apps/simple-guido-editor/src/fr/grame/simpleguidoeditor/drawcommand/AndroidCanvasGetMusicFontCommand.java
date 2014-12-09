package fr.grame.simpleguidoeditor.drawcommand;
import drawcommand.GetMusicFontCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
   
public class AndroidCanvasGetMusicFontCommand extends GetMusicFontCommand implements DrawToCanvas {

  public AndroidCanvasGetMusicFontCommand() {
    super();
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
