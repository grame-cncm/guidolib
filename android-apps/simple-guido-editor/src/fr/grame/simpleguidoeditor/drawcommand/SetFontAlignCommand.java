package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class SetFontAlignCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public int _inAlign;

  public SetFontAlignCommand(int inAlign) {
    super();
    _inAlign = inAlign;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("SetFontAlign");
    out.append(" ");
    out.append(_inAlign);
    return out.toString();
  }
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
