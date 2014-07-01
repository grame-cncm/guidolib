package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import android.graphics.Color;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
import android.graphics.Matrix;
import android.graphics.Paint;

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
    double left = 0;
    double right = canvas.getWidth();
    double top = canvas.getHeight();
    double bottom = 0;

    Paint paint = new Paint();
    paint.setColor(Color.WHITE);
    paint.setStyle(Paint.Style.FILL);
    canvas.drawRect((float)left, (float)top, (float)right, (float)bottom, paint);

  }

}
