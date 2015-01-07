package fr.grame.android.drawcommand.command;
import drawcommand.SetFontColorCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
import android.graphics.Color;
   
public class AndroidCanvasSetFontColorCommand extends SetFontColorCommand implements DrawToCanvas {


  public AndroidCanvasSetFontColorCommand(char alpha, char red, char green, char blue) {
    super(alpha, red, green, blue);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view.setFontColor(Color.argb(_alpha, _red, _green, _blue));
  }
}
