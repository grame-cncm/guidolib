package drawcommand;

   
public class PopPenWidthCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public PopPenWidthCommand() {
    super();
  }
  @Override
  public String asString() {
    return "PopPenWidth";
  }

}
