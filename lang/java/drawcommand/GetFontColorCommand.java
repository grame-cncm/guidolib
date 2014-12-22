package drawcommand;

   
public class GetFontColorCommand implements PrintableDrawCommand {

  public GetFontColorCommand() {
    super();
  }

  @Override
  public String asString() {
    return "GetFontColor";
  }
  
}
