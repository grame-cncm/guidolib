package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class GetFontColorCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public GetFontColorCommand() {
    super();
  }

  @Override
  public String asString() {
    return "GetFontColor";
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
  
}
