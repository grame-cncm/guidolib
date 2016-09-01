package drawcommand;

   
public class PopPenColorCommand implements PrintableDrawCommand {

  public PopPenColorCommand() {
    super();
  }

  @Override
  public String asString() {
    return "PopPenColor";
  }  
}
