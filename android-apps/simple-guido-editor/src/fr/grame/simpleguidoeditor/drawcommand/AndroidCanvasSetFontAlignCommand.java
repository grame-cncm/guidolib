package fr.grame.simpleguidoeditor.drawcommand;
import drawcommand.SetFontAlignCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;
   
public class AndroidCanvasSetFontAlignCommand extends SetFontAlignCommand implements DrawToCanvas {


  public AndroidCanvasSetFontAlignCommand(int inAlign) {
    super(inAlign);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
