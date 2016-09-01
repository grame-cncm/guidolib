package drawcommand;

   
public class GetTextFontCommand implements PrintableDrawCommand {

  public GetTextFontCommand() {
    super();
  }
  
  @Override
  public String asString() {
    return "GetTextFont";
  }
  
}
