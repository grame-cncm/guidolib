package fr.grame.simpleguidoeditor.drawcommand;
import drawcommand.SetFontColorCommand;

import android.graphics.Canvas;
import android.graphics.Color;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
   
public class AndroidCanvasSetFontColorCommand extends SetFontColorCommand implements DrawToCanvas {


  public AndroidCanvasSetFontColorCommand(char alpha, char red, char green, char blue) {
    super(alpha, red, green, blue);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view._FONT_COLOR = Color.argb(_alpha, _red, _green, _blue);
  }
}
