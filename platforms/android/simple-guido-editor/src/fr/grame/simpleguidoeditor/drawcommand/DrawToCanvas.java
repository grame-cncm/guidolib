package fr.grame.simpleguidoeditor.drawcommand;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

/**
 * Draw to android canvas 
 *
 */
public interface DrawToCanvas {

  void drawToCanvas(Canvas canvas, GuidoCanvasView view);

}
