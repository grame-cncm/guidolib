package drawcommand;

   
public class PopPenColorCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public PopPenColorCommand() {
    super();
  }

  @Override
  public String asString() {
    return "PopPenColor";
  }
  
}
