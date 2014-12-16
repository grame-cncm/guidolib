 package fr.grame.simpleguidoeditor.drawcommand;
import drawcommand.SetTextFontCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
import android.graphics.Typeface;
   
public class AndroidCanvasSetTextFontCommand extends SetTextFontCommand implements DrawToCanvas {

  public AndroidCanvasSetTextFontCommand(String name, int size, int properties) {
    super(name, size, properties);
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view._TEXT_FONT = Typeface.createFromAsset(view.getContext().getAssets(), "fonts/" + _name + ".ttf");
    view._TEXT_FONT_SIZE = _size;
  }
}
