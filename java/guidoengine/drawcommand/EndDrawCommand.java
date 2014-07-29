package guidoengine.drawcommand;

import java.lang.StringBuilder;

import java.util.*;
   
public class EndDrawCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public EndDrawCommand() {
    super();
  }

  @Override
  public String asString() {
    return "EndDraw";
  }

}
