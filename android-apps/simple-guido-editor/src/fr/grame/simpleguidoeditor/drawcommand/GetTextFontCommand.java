package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class GetTextFontCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public GetTextFontCommand() {
    super();
  }
  
  @Override
  public String asString() {
    return "GetTextFont";
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
