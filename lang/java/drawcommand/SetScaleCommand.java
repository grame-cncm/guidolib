package drawcommand;

   
public class SetScaleCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public float _x;
  public float _y;

  public SetScaleCommand(float x, float y) {
    super();
    _x = x;
    _y = y;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("SetScale");
    out.append(" ");
    out.append(_x);
    out.append(" ");
    out.append(_y);
    return out.toString();
  }
  
}
