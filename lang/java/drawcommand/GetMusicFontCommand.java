package drawcommand;

   
public class GetMusicFontCommand implements PrintableDrawCommand {

  public GetMusicFontCommand() {
    super();
  }

  @Override
  public String asString() {
    return "GetMusicFont";
  }
  
}
