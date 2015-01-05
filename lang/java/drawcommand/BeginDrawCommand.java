package drawcommand;

   
public class BeginDrawCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public BeginDrawCommand() {
    super();
  }

  @Override
  public String asString() {
    return "BeginDraw";
  }

}
