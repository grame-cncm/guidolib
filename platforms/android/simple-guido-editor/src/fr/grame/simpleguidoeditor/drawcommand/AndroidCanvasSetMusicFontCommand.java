package fr.grame.simpleguidoeditor.drawcommand;
import drawcommand.SetMusicFontCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
import android.graphics.Typeface;
   
public class AndroidCanvasSetMusicFontCommand extends SetMusicFontCommand implements DrawToCanvas {

  public AndroidCanvasSetMusicFontCommand(String name, int size, int properties) {
    super(name, size, properties);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view._MUSIC_FONT = Typeface.createFromAsset(view.getContext().getAssets(), "fonts/" + _name + ".ttf");
    view._MUSIC_FONT_SIZE = _size;
  }
}
