package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class EndDrawCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public EndDrawCommand() {
    super();
  }

  @Override
  public String asString() {
    return "EndDraw";
  }
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
