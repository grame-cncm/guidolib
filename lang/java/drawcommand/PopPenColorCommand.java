package drawcommand;

import java.lang.StringBuilder;

import java.util.*;
   
public class PopPenColorCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public PopPenColorCommand() {
    super();
  }

  @Override
  public String asString() {
    return "PopPenColor";
  }
  
}
