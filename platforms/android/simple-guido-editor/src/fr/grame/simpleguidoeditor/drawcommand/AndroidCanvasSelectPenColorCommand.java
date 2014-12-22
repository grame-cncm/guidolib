package fr.grame.simpleguidoeditor.drawcommand;
import drawcommand.SelectPenColorCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
import android.graphics.Color;
   
public class AndroidCanvasSelectPenColorCommand extends SelectPenColorCommand implements DrawToCanvas {


  public AndroidCanvasSelectPenColorCommand(char alpha, char red, char green, char blue) {
    super(alpha, red, green, blue);
  }
  
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    view.setCurrentPenColor(Color.argb(_alpha, _red, _green, _blue));
  }
}
