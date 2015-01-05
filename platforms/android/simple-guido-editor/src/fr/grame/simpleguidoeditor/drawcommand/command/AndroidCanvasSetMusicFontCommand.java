package fr.grame.simpleguidoeditor.drawcommand.command;
import drawcommand.SetMusicFontCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
import android.graphics.Typeface;
   
public class AndroidCanvasSetMusicFontCommand extends SetMusicFontCommand implements DrawToCanvas {

  public AndroidCanvasSetMusicFontCommand(String name, int size, int properties) {
    super(name, size, properties);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view.setMusicFont(Typeface.createFromAsset(view.getContext().getAssets(), "fonts/" + _name + ".ttf"));
    view.setMusicFontSize(_size);
  }
}
