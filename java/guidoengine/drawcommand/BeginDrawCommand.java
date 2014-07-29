package guidoengine.drawcommand;

import java.lang.StringBuilder;

import java.util.*;
   
public class BeginDrawCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public BeginDrawCommand() {
    super();
  }

  @Override
  public String asString() {
    return "BeginDraw";
  }

}
