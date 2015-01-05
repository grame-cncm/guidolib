package fr.grame.simpleguidoeditor.drawcommand.command;
import android.graphics.Canvas;
import drawcommand.NotifySizeCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;
   
/**
 * Android specific notifySizeCommand.
 */
public class AndroidCanvasNotifySizeCommand extends NotifySizeCommand implements DrawToCanvas {

  public AndroidCanvasNotifySizeCommand(int width, int height) {
    super(width, height);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
    double rescale_x = (view.getWidth() * 1.0) / _width;
    double rescale_y = (view.getHeight() * 1.0) / _height;
    view.setGlobalRescaleFactor(Math.min(rescale_x, rescale_y));
  }
}
