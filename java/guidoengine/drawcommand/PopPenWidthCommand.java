package guidoengine.drawcommand;

import java.lang.StringBuilder;

import java.util.*;
   
public class PopPenWidthCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public PopPenWidthCommand() {
    super();
  }
  @Override
  public String asString() {
    return "PopPenWidth";
  }

}
