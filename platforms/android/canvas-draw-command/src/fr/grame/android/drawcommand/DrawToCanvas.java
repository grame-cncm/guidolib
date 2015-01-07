package fr.grame.android.drawcommand;
import android.graphics.Canvas;

/**
 * Interface to draw in android canvas.
 * The android draw command implements this interface.
 */
public interface DrawToCanvas {

  void drawToCanvas(Canvas canvas, GuidoCanvasView view);

}
