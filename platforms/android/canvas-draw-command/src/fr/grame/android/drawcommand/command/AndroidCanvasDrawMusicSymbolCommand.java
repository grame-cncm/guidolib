package fr.grame.android.drawcommand.command;
import drawcommand.DrawMusicSymbolCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;

import android.graphics.Paint;

public class AndroidCanvasDrawMusicSymbolCommand extends DrawMusicSymbolCommand implements DrawToCanvas {

  public AndroidCanvasDrawMusicSymbolCommand(float x, float y, int inSymbolID) {
    super(x, y, inSymbolID);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view.correctTransformMatrix(canvas);

    Paint paint = new Paint();
    paint.setAntiAlias(true);
    
    paint.setTypeface(view.getMusicFont());
    paint.setColor(view.getFontColor());
    paint.setTextSize(view.getMusicFontSize());

    StringBuilder mus_glyph_to_string= new StringBuilder(1);
    mus_glyph_to_string.append(Character.toChars(_inSymbolID));
    canvas.drawText(mus_glyph_to_string.toString(), _x, _y, paint);
    view.resetTransformMatrix(canvas);
  }
}
