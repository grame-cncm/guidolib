package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import android.graphics.Color;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
import android.graphics.Matrix;

import java.util.*;
   
public class BeginDrawCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public BeginDrawCommand() {
    super();
  }

  @Override
  public String asString() {
    return "BeginDraw";
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    canvas.setMatrix(new Matrix());
    canvas.drawColor(Color.WHITE);
  }

}
