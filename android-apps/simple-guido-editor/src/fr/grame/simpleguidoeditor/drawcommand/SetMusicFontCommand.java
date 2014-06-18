package fr.grame.simpleguidoeditor.drawcommand;

import java.lang.StringBuilder;
import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

import java.util.*;
   
public class SetMusicFontCommand extends GuidoDrawCommand implements PrintableDrawCommand, DrawToCanvas {

  public String _name;
  public int _size;
  public int _properties;

  public SetMusicFontCommand(String name, int size, int properties) {
    super();
    _name = name;
    _size = size;
    _properties = properties;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("SelectMusicFont");
    out.append(" ");
    out.append(_name);
    out.append(" ");
    out.append(_size);
    out.append(" ");
    out.append(_properties);
    return out.toString();
  }
  @Override
  public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
  }
}
