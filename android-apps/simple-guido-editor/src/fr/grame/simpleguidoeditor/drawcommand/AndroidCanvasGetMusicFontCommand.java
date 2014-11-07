package fr.grame.simpleguidoeditor.drawcommand;
import guidoengine.drawcommand.GetMusicFontCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class AndroidCanvasGetMusicFontCommand extends GetMusicFontCommand implements DrawToCanvas {

  public AndroidCanvasGetMusicFontCommand() {
    super();
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
