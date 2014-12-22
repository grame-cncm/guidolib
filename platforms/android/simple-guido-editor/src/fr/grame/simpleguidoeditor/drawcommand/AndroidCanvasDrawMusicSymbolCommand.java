package fr.grame.simpleguidoeditor.drawcommand;
import drawcommand.DrawMusicSymbolCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import android.graphics.Paint;

public class AndroidCanvasDrawMusicSymbolCommand extends DrawMusicSymbolCommand implements DrawToCanvas {

  public AndroidCanvasDrawMusicSymbolCommand(float x, float y, int inSymbolID) {
    super(x, y, inSymbolID);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view.correctTransformMatrix(canvas);

    Paint paint = new Paint();
    paint.setTypeface(view._MUSIC_FONT);
    paint.setColor(view._FONT_COLOR);
    paint.setTextSize(view._MUSIC_FONT_SIZE);

    StringBuilder mus_glyph_to_string= new StringBuilder(1);
    mus_glyph_to_string.append(Character.toChars(_inSymbolID));
    canvas.drawText(mus_glyph_to_string.toString(), _x, _y, paint);
    view.resetTransformMatrix(canvas);
  }
}
