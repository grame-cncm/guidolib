package drawcommand;

import java.util.List;
   
public class PolygonCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public List<Float> _xCoords;
  public List<Float> _yCoords;
  public int _count;

  public PolygonCommand(List<Float> xCoords, List<Float> yCoords, int count) {
    super();
    _xCoords = xCoords;
    _yCoords = yCoords;
    _count = count;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("Polygon");
    for (int i = 0; i < _xCoords.size(); i++) {
      out.append(" ");
      out.append(_xCoords.get(i));
    }
    for (int i = 0; i < _yCoords.size(); i++) {
      out.append(" ");
      out.append(_yCoords.get(i));
    }
    out.append(" ");
    out.append(_count);
    return out.toString();
  }
  
}
