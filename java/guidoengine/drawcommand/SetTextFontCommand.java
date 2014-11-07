 package guidoengine.drawcommand;

import java.lang.StringBuilder;

import java.util.*;
   
public class SetTextFontCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public String _name;
  public int _size;
  public int _properties;

  public SetTextFontCommand(String name, int size, int properties) {
    super();
    _name = name;
    _size = size;
    _properties = properties;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("SetTextFont");
    out.append(" ");
    out.append(_name);
    out.append(" ");
    out.append(_size);
    out.append(" ");
    out.append(_properties);
    return out.toString();
  }

}
