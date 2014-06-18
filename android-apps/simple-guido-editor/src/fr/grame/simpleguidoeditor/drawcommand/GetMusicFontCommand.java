package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class GetMusicFontCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public GetMusicFontCommand() {
    super();
  }

  @Override
  public String asString() {
    return "GetMusicFont";
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
