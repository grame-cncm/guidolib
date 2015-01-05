 package fr.grame.simpleguidoeditor.drawcommand.command;
import drawcommand.SetTextFontCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
import android.graphics.Typeface;
   
public class AndroidCanvasSetTextFontCommand extends SetTextFontCommand implements DrawToCanvas {

  public AndroidCanvasSetTextFontCommand(String name, int size, int properties) {
    super(name, size, properties);
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view.setTextFont(Typeface.createFromAsset(view.getContext().getAssets(), "fonts/" + _name + ".ttf"));
    view.setTextFontSize(_size);
  }
}
