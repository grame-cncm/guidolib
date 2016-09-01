package fr.grame.android.drawcommand.command;
import drawcommand.SelectPenColorCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
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
