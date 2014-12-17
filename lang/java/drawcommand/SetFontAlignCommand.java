package drawcommand;

   
public class SetFontAlignCommand extends GuidoDrawCommand implements PrintableDrawCommand {

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

}
