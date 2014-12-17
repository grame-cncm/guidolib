package drawcommand;

   
public class GetMusicFontCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public GetMusicFontCommand() {
    super();
  }

  @Override
  public String asString() {
    return "GetMusicFont";
  }
  
}
