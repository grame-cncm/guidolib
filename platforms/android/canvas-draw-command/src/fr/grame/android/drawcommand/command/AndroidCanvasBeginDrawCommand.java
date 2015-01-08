package fr.grame.android.drawcommand.command;
import drawcommand.BeginDrawCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
   
public class AndroidCanvasBeginDrawCommand extends BeginDrawCommand implements DrawToCanvas {

  public AndroidCanvasBeginDrawCommand() {
    super();
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    double left = 0;
    double right = canvas.getWidth();
    double top = canvas.getHeight();
    double bottom = 0;

    Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
    paint.setColor(Color.WHITE);
    paint.setStyle(Paint.Style.FILL);
    canvas.drawRect((float)left, (float)top, (float)right, (float)bottom, paint);

  }

}
