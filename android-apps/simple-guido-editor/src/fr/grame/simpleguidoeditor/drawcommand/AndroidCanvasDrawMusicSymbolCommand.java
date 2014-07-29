package fr.grame.simpleguidoeditor.drawcommand;
import guidoengine.drawcommand.DrawMusicSymbolCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;

import android.util.Log;
import android.graphics.Paint;
import android.graphics.Color;

public class AndroidCanvasDrawMusicSymbolCommand extends DrawMusicSymbolCommand implements DrawToCanvas {

  public AndroidCanvasDrawMusicSymbolCommand(float x, float y, int inSymbolID) {
    super(x, y, inSymbolID);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    double x = _x;
    double y = _y;
    double size = view._MUSIC_FONT_SIZE;

    view.correctTransformMatrix(canvas);

    Paint paint = new Paint();
    paint.setTypeface(view._MUSIC_FONT);
    paint.setColor(view._FONT_COLOR);
    paint.setTextSize((float)(view._MUSIC_FONT_SIZE));

    StringBuilder mus_glyph_to_string= new StringBuilder(1);
    mus_glyph_to_string.append(Character.toChars(_inSymbolID));
    canvas.drawText(mus_glyph_to_string.toString(), (float) x, (float) y, paint);
    view.resetTransformMatrix(canvas);
  }
}
