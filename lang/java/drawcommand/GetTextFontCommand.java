package drawcommand;

   
public class GetTextFontCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public GetTextFontCommand() {
    super();
  }
  
  @Override
  public String asString() {
    return "GetTextFont";
  }
  
}
