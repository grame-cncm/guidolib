package drawcommand;

   
public class BeginDrawCommand implements PrintableDrawCommand {

  public BeginDrawCommand() {
    super();
  }

  @Override
  public String asString() {
    return "BeginDraw";
  }

}
