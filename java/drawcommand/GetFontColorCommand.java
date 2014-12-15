package drawcommand;

import java.lang.StringBuilder;

import java.util.*;
   
public class GetFontColorCommand extends GuidoDrawCommand implements PrintableDrawCommand {

  public GetFontColorCommand() {
    super();
  }

  @Override
  public String asString() {
    return "GetFontColor";
  }
  
}
