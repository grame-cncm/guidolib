package fr.grame.simpleguidoeditor.drawcommand;
import guidoengine.drawcommand.SetFontAlignCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class AndroidCanvasSetFontAlignCommand extends SetFontAlignCommand implements DrawToCanvas {


  public AndroidCanvasSetFontAlignCommand(int inAlign) {
    super(inAlign);
  }

  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
