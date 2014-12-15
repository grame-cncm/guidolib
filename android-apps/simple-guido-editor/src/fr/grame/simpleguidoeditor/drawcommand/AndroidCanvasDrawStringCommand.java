package fr.grame.simpleguidoeditor.drawcommand;
import drawcommand.DrawStringCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import android.graphics.Paint;
   
public class AndroidCanvasDrawStringCommand extends DrawStringCommand implements DrawToCanvas {

  public AndroidCanvasDrawStringCommand(float x, float y, String s, int inCharCount) {
    super(x, y, s, inCharCount);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    double x = _x;
    double y = _y;
    //double size = view._MUSIC_FONT_SIZE;

    view.correctTransformMatrix(canvas);

    Paint paint = new Paint();
    paint.setTypeface(view._TEXT_FONT);
    paint.setColor(view._FONT_COLOR);
    paint.setTextSize((float)(view._TEXT_FONT_SIZE));

    canvas.drawText(_s, (float) x, (float) y, paint);
    view.resetTransformMatrix(canvas);
  }

}
