package fr.grame.simpleguidoeditor.parser;

import fr.grame.simpleguidoeditor.drawcommand.*;
import java.nio.ByteBuffer;

import java.util.*;
import android.util.Log;

import java.io.UnsupportedEncodingException;
   
public class GuidoBinaryParser {

    /* codes for parser */
    public static int C_CHAR_SIZE = 1;
    public static int C_INT_SIZE = 4;
    public static int C_UNSIGNED_INT_SIZE = 4;
    public static int C_FLOAT_SIZE = 4;
    
    public static int JAVA_CHAR_SIZE = 2;
    public static int JAVA_INT_SIZE = 4;
    public static int JAVA_UNSIGNED_INT_SIZE = 4;
    public static int JAVA_FLOAT_SIZE = 4;
    
    public static char BEGIN_DRAW_CODE = 0;
    public static char END_DRAW_CODE = 1;
    public static char LINE_CODE = 5;
    public static char POLYGON_CODE = 9;
    public static char RECTANGLE_CODE = 10;
    public static char SET_MUSIC_FONT_CODE = 11;
    public static char GET_MUSIC_FONT_CODE = 12;
    public static char SET_TEXT_FONT_CODE = 13;
    public static char GET_TEXT_FONT_CODE = 14;
    public static char SET_SCALE_CODE = 27;
    public static char SET_ORIGIN_CODE = 28;
    public static char OFFSET_ORIGIN_CODE = 29;
    public static char NOTIFY_SIZE_CODE = 36;
    public static char DRAW_MUSIC_SYMBOL_CODE = 39;
    public static char DRAW_STRING_CODE = 40;
    public static char SET_FONT_COLOR_CODE = 41;
    public static char GET_FONT_COLOR_CODE = 42;
    public static char SET_FONT_ALIGN_CODE = 45;
    public static char SELECT_PEN_COLOR_CODE = 49;
    public static char POP_PEN_COLOR_CODE = 52;
    public static char PUSH_PEN_WIDTH_CODE = 53;
    public static char POP_PEN_WIDTH_CODE = 54;

    public static char getUnsignedChar(byte[] data, int place) {
      //Log.i("SimpleGuidoEditor", "getting char");

      /// ugh...chars in java are represented by two bytes
      byte[] holder = new byte[1];
      holder[0] = data[place];
      try {
        String kludge = new String(holder, "UTF-8");
        char[] chars = kludge.toCharArray(); 
        return chars[0];
      }
      catch (UnsupportedEncodingException e) {
        Log.i("SimpleGuidoEditor", "should never happen");
        return 0;
      }
    }

    // no unsigned in java...
    public static int getUnsignedInt32(byte[] data, int place) {
      //Log.i("SimpleGuidoEditor", "getting unsingned int");
      return GuidoBinaryParser.getInt32(data, place);
    }

    public static int getInt32(byte[] data, int place) {
      //Log.i("SimpleGuidoEditor", "getting int");
      byte[] holder = new byte[4];
      // flip for endianness
      holder[0] = data[place + 3];
      holder[1] = data[place + 2];
      holder[2] = data[place + 1];
      holder[3] = data[place];
      return ByteBuffer.wrap(holder).getInt();
    }

    public static float getFloat(byte[] data, int place) {
      byte[] holder = new byte[4];
      // flip for endianness
      holder[0] = data[place + 3];
      holder[1] = data[place + 2];
      holder[2] = data[place + 1];
      holder[3] = data[place];
      return ByteBuffer.wrap(holder).getFloat();
    }

    public static String getString(byte[] data, int place, int n) {
      String str = "";
      int i = place;
      if (n < 0) {
        n = 1000000;
      }
      while ((GuidoBinaryParser.getUnsignedChar(data, i) != '\0') && (i < n)) {
        str += new String(data, i, 1);
        if (i == 100) {
          Log.w("SimpleGuidoEditor", "INTRACTABLE string");
          break;
        }
        i = i + 1;
      }
      return str;
    }

    public static int moveReadPositionByChar(int place) {
      return place + GuidoBinaryParser.C_CHAR_SIZE;
    }

    public static int moveReadPositionByUnsignedInt32(int place) {
      return place + GuidoBinaryParser.C_UNSIGNED_INT_SIZE;
    }

    public static int moveReadPositionByInt32(int place) {
      return place + GuidoBinaryParser.C_INT_SIZE;
    }

    public static int moveReadPositionByFloat(int place) {
      return place + GuidoBinaryParser.C_FLOAT_SIZE;
    }

    public static int moveReadPositionByString(int place, String str) {
      return place + str.length() + 1; // 1 for the escape char
    }

    public static List<GuidoDrawCommand> parseIntoDrawCommands(byte[] data) {
      int place = 0;
      List<GuidoDrawCommand> drawCommands = new ArrayList<GuidoDrawCommand>();
      while ((place >= 0) && (place < data.length)) {
        char head = GuidoBinaryParser.getUnsignedChar(data, place);
        place = GuidoBinaryParser.moveReadPositionByChar(place);
        if (head == GuidoBinaryParser.BEGIN_DRAW_CODE) {
          place = GuidoBinaryParser.BeginDraw(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.END_DRAW_CODE) {
          place = GuidoBinaryParser.EndDraw(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.LINE_CODE) {
          place = GuidoBinaryParser.Line(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.POLYGON_CODE) {
          place = GuidoBinaryParser.Polygon(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.RECTANGLE_CODE) {
          place = GuidoBinaryParser.Rectangle(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.SET_MUSIC_FONT_CODE) {
          place = GuidoBinaryParser.SetMusicFont(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.GET_MUSIC_FONT_CODE) {
          place = GuidoBinaryParser.GetMusicFont(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.SET_TEXT_FONT_CODE) {
          place = GuidoBinaryParser.SetTextFont(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.GET_TEXT_FONT_CODE) {
          place = GuidoBinaryParser.GetTextFont(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.SET_SCALE_CODE) {
          place = GuidoBinaryParser.SetScale(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.SET_ORIGIN_CODE) {
          place = GuidoBinaryParser.SetOrigin(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.OFFSET_ORIGIN_CODE) {
          place = GuidoBinaryParser.OffsetOrigin(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.NOTIFY_SIZE_CODE) {
          place = GuidoBinaryParser.NotifySize(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.DRAW_MUSIC_SYMBOL_CODE) {
          place = GuidoBinaryParser.DrawMusicSymbol(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.DRAW_STRING_CODE) {
          place = GuidoBinaryParser.DrawString(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.SET_FONT_COLOR_CODE) {
          place = GuidoBinaryParser.SetFontColor(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.GET_FONT_COLOR_CODE) {
          place = GuidoBinaryParser.GetFontColor(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.SET_FONT_ALIGN_CODE) {
          place = GuidoBinaryParser.SetFontAlign(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.SELECT_PEN_COLOR_CODE) {
          place = GuidoBinaryParser.SelectPenColor(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.POP_PEN_COLOR_CODE) {
          place = GuidoBinaryParser.PopPenColor(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.PUSH_PEN_WIDTH_CODE) {
          place = GuidoBinaryParser.PushPenWidth(data, place, drawCommands);
        } else if (head == GuidoBinaryParser.POP_PEN_WIDTH_CODE) {
          place = GuidoBinaryParser.PopPenWidth(data, place, drawCommands);
        } else {
          // panic
          Log.w("SimpleGuidoEditor", "have no clue how to interpret the number below...");
          Log.w("SimpleGuidoEditor", Integer.toString(head));
          place = -1;
        }
      }
      //console.log("finished drawing");
      return drawCommands;
    } 

    public static int BeginDraw(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      //////////////////////////
      drawCommands.add(new BeginDrawCommand());
      return place;
    }

    public static int EndDraw(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      ///////////////////////
      drawCommands.add(new EndDrawCommand());
      return place;
    }

    public static int Line(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      float x1 = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      float y1 = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      float x2 = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      float y2 = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      ///////////////////////////
      drawCommands.add(new LineCommand(x1, y1, x2, y2));
      return place;
    }

    public static int Polygon(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      int count = GuidoBinaryParser.getInt32(data, place);
      place = GuidoBinaryParser.moveReadPositionByInt32(place);
      List<Float> xCoords = new ArrayList<Float>();
      List<Float> yCoords = new ArrayList<Float>();
      for (int i = 0; i < count; i++) {
        float x = GuidoBinaryParser.getFloat(data, place);
        place = GuidoBinaryParser.moveReadPositionByFloat(place);
        xCoords.add(x);
      }
      for (int i = 0; i < count; i++) {
        float y = GuidoBinaryParser.getFloat(data, place);
        place = GuidoBinaryParser.moveReadPositionByFloat(place);
        yCoords.add(y);
      }
      ///////////////////////////
      drawCommands.add(new PolygonCommand(xCoords, yCoords, count));
      return place;
    }

    public static int Rectangle(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      float left = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      float top = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      float right = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      float bottom = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      ///////////////////////////
      drawCommands.add(new RectangleCommand(left, top, right, bottom));
      return place;
    }

    public static int SetMusicFont(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      String name = GuidoBinaryParser.getString(data, place, -1);
      place = GuidoBinaryParser.moveReadPositionByString(place, name);
      int size = GuidoBinaryParser.getInt32(data, place);
      place = GuidoBinaryParser.moveReadPositionByInt32(place);
      int properties = GuidoBinaryParser.getInt32(data, place);
      place = GuidoBinaryParser.moveReadPositionByInt32(place);
      ///////////////////////////
      drawCommands.add(new SetMusicFontCommand(name, size, properties));
      return place;
    }

    public static int GetMusicFont(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      ///////////////////////////
      drawCommands.add(new GetMusicFontCommand());
      return place;
    }

    public static int SetTextFont(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      String name = GuidoBinaryParser.getString(data, place, -1);
      place = GuidoBinaryParser.moveReadPositionByString(place, name);
      int size = GuidoBinaryParser.getInt32(data, place);
      place = GuidoBinaryParser.moveReadPositionByInt32(place);
      int properties = GuidoBinaryParser.getInt32(data, place);
      place = GuidoBinaryParser.moveReadPositionByInt32(place);
      ///////////////////////////
      drawCommands.add(new SetTextFontCommand(name, size, properties));
      return place;
    }

    public static int GetTextFont(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      ///////////////////////////
      drawCommands.add(new GetTextFontCommand());
      return place;
    }

    public static int SetScale(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      float x = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      float y = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      ///////////////////////////
      drawCommands.add(new SetScaleCommand(x, y));
      return place;
    }

    public static int SetOrigin(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      float x = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      float y = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      ///////////////////////////
      drawCommands.add(new SetOriginCommand(x, y));
      return place;
    }

    public static int OffsetOrigin(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      float x = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      float y = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      ///////////////////////////
      drawCommands.add(new OffsetOriginCommand(x, y));
      return place;
    }

    public static int NotifySize(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      int width = GuidoBinaryParser.getInt32(data, place);
      place = GuidoBinaryParser.moveReadPositionByInt32(place);
      int height = GuidoBinaryParser.getInt32(data, place);
      place = GuidoBinaryParser.moveReadPositionByInt32(place);
      ///////////////////////////
      drawCommands.add(new NotifySizeCommand(width, height));
      return place;
    }

    public static int DrawMusicSymbol(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      float x = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      float y = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      int inSymbolID = GuidoBinaryParser.getUnsignedInt32(data, place);
      place = GuidoBinaryParser.moveReadPositionByUnsignedInt32(place);
      ///////////////////////////
      drawCommands.add(new DrawMusicSymbolCommand(x, y, inSymbolID));
      return place;
    }

    public static int DrawString(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      float x = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      float y = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      int inCharCount = GuidoBinaryParser.getInt32(data, place);
      place = GuidoBinaryParser.moveReadPositionByInt32(place);
      String s = GuidoBinaryParser.getString(data, place, inCharCount);
      place = GuidoBinaryParser.moveReadPositionByString(place, s);
      // because not null terminated...
      place = place - 1;
      ///////////////////////////
      drawCommands.add(new DrawStringCommand(x, y, s, inCharCount));
      return place;
    }

    public static int SetFontColor(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      char alpha = GuidoBinaryParser.getUnsignedChar(data, place);
      place = GuidoBinaryParser.moveReadPositionByChar(place);
      char red = GuidoBinaryParser.getUnsignedChar(data, place);
      place = GuidoBinaryParser.moveReadPositionByChar(place);
      char green = GuidoBinaryParser.getUnsignedChar(data, place);
      place = GuidoBinaryParser.moveReadPositionByChar(place);
      char blue = GuidoBinaryParser.getUnsignedChar(data, place);
      place = GuidoBinaryParser.moveReadPositionByChar(place);
      /////////////////////////////////////////
      drawCommands.add(new SetFontColorCommand(alpha, red, green, blue));
      return place;
    }

    public static int GetFontColor(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      ///////////////////////////
      drawCommands.add(new GetFontColorCommand());
      return place;
    }

    public static int SetFontAlign(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      int inAlign = GuidoBinaryParser.getUnsignedInt32(data, place);
      place = GuidoBinaryParser.moveReadPositionByUnsignedInt32(place);
      /////////////////////////////////////////
      drawCommands.add(new SetFontAlignCommand(inAlign));
      return place;
    }

    public static int SelectPenColor(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      char alpha = GuidoBinaryParser.getUnsignedChar(data, place);
      place = GuidoBinaryParser.moveReadPositionByChar(place);
      char red = GuidoBinaryParser.getUnsignedChar(data, place);
      place = GuidoBinaryParser.moveReadPositionByChar(place);
      char green = GuidoBinaryParser.getUnsignedChar(data, place);
      place = GuidoBinaryParser.moveReadPositionByChar(place);
      char blue = GuidoBinaryParser.getUnsignedChar(data, place);
      place = GuidoBinaryParser.moveReadPositionByChar(place);
      /////////////////////////////////////////
      drawCommands.add(new SelectPenColorCommand(alpha, red, green, blue));
      return place;
    }

    public static int PopPenColor(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      /////////////////////////////////////////
      drawCommands.add(new PopPenColorCommand());
      return place;
    }

    public static int PushPenWidth(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      float width = GuidoBinaryParser.getFloat(data, place);
      place = GuidoBinaryParser.moveReadPositionByFloat(place);
      ///////////////////////////
      drawCommands.add(new PushPenWidthCommand(width));
      return place;
    }

    public static int PopPenWidth(byte[] data, int place, List<GuidoDrawCommand> drawCommands) {
      /////////////////////////////////////////
      drawCommands.add(new PopPenWidthCommand());
      return place;
    }


}