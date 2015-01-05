package fr.grame.simpleguidoeditor.drawcommand.command;
import drawcommand.SetFontAlignCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
   
public class AndroidCanvasSetFontAlignCommand extends SetFontAlignCommand implements DrawToCanvas {


  public AndroidCanvasSetFontAlignCommand(int inAlign) {
    super(inAlign);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
