package guidoengine.drawcommand;

import java.lang.StringBuilder;

import java.util.*;
   
public class GetMusicFontCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public GetMusicFontCommand() {
    super();
  }

  @Override
  public String asString() {
    return "GetMusicFont";
  }
  
}
