package fr.grame.android.drawcommand.command;
import drawcommand.GetMusicFontCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
   
public class AndroidCanvasGetMusicFontCommand extends GetMusicFontCommand implements DrawToCanvas {

  public AndroidCanvasGetMusicFontCommand() {
    super();
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
