package drawcommand;

   
public class EndDrawCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public EndDrawCommand() {
    super();
  }

  @Override
  public String asString() {
    return "EndDraw";
  }

}
