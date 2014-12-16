package drawcommand;

import java.lang.StringBuilder;

import java.util.*;


public class DrawMusicSymbolCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public float _x;
  public float _y;
  public int _inSymbolID;

  public DrawMusicSymbolCommand(float x, float y, int inSymbolID) {
    super();
    _x = x;
    _y = y;
    _inSymbolID = inSymbolID;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("DrawMusicSymbol");
    out.append(" ");
    out.append(_x);
    out.append(" ");
    out.append(_y);
    out.append(" ");
    out.append(_inSymbolID);
    return out.toString();
  }

}
