package fr.grame.simpleguidoeditor.drawcommand;
import guidoengine.drawcommand.GetTextFontCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class AndroidCanvasGetTextFontCommand extends GetTextFontCommand implements DrawToCanvas {

  public AndroidCanvasGetTextFontCommand() {
    super();
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
