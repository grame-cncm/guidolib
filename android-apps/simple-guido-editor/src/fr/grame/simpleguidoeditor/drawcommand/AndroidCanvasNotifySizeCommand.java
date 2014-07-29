package fr.grame.simpleguidoeditor.drawcommand;
import guidoengine.drawcommand.NotifySizeCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class AndroidCanvasNotifySizeCommand extends NotifySizeCommand implements DrawToCanvas {

  public AndroidCanvasNotifySizeCommand(int width, int height) {
    super(width, height);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    double rescale_x = (view.getWidth() * 1.0) / _width;
    double rescale_y = (view.getHeight() * 1.0) / _height;
    view._GLOBAL_RESCALE_FACTOR = Math.min(rescale_x, rescale_y);
  }
}
