package drawcommand;

   
public class PushPenWidthCommand implements PrintableDrawCommand {

  public float _width;

  public PushPenWidthCommand(float width) {
    super();
    _width = width;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("PushPenWidth");
    out.append(" ");
    out.append(_width);
    return out.toString();
  }

}
