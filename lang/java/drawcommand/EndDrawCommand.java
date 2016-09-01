package drawcommand;

   
public class EndDrawCommand implements PrintableDrawCommand {

  public EndDrawCommand() {
    super();
  }

  @Override
  public String asString() {
    return "EndDraw";
  }

}
