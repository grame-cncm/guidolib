package drawcommand;

   
public class PopPenWidthCommand implements PrintableDrawCommand {

  public PopPenWidthCommand() {
    super();
  }
  @Override
  public String asString() {
    return "PopPenWidth";
  }

}
