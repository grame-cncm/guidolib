package drawcommand;


public class RectangleCommand implements PrintableDrawCommand {

  public float _left;
  public float _top;
  public float _right;
  public float _bottom;

  public RectangleCommand(float left, float top, float right, float bottom) {
    super();
    _left = left;
    _top = top;
    _right = right;
    _bottom = bottom;
  }

  @Override
  public String asString() {
    StringBuilder out = new StringBuilder();
    out.append("Rectangle");
    out.append(" ");
    out.append(_left);
    out.append(" ");
    out.append(_top);
    out.append(" ");
    out.append(_right);
    out.append(" ");
    out.append(_bottom);
    return out.toString();
  }

}
