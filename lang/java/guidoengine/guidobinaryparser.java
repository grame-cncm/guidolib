package guidoengine;

import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

/**
 * Parse guido binary export and add draw command into guidocommandbattery object.
 *
 */
public class guidobinaryparser {

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
	public static char PUSH_PEN = 17;
	public static char POP_PEN = 18;
	public static char PUSH_FILL_COLOR = 19;
	public static char POP_FILL_COLOR = 20;
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
	public static char PUSH_PEN_COLOR_CODE = 51;
	public static char POP_PEN_COLOR_CODE = 52;
	public static char PUSH_PEN_WIDTH_CODE = 53;
	public static char POP_PEN_WIDTH_CODE = 54;

	public static char getUnsignedChar(byte[] data, int place) {

		// / ugh...chars in java are represented by two bytes
		byte[] holder = new byte[1];
		holder[0] = data[place];
		try {
			String kludge = new String(holder, "UTF-8");
			char[] chars = kludge.toCharArray();
			return chars[0];
		} catch (UnsupportedEncodingException e) {
			return 0;
		}
	}

	// no unsigned in java...
	public static int getUnsignedInt32(byte[] data, int place) {
		return guidobinaryparser.getInt32(data, place);
	}

	public static int getInt32(byte[] data, int place) {
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
		StringBuilder str = new StringBuilder();
		if (n < 0) {
			n = 1000000;
		}
		int i = 0;
		while ((guidobinaryparser.getUnsignedChar(data, place) != '\0') && (i < n)) {
			str.append(new String(data, place, 1));
			if (place == 100) {
				// Log.w("SimpleGuidoEditor", "INTRACTABLE string");
				break;
			}
			place ++;
			i++;
		}
		return str.toString();
	}

	public static int moveReadPositionByChar(int place) {
		return place + guidobinaryparser.C_CHAR_SIZE;
	}

	public static int moveReadPositionByUnsignedInt32(int place) {
		return place + guidobinaryparser.C_UNSIGNED_INT_SIZE;
	}

	public static int moveReadPositionByInt32(int place) {
		return place + guidobinaryparser.C_INT_SIZE;
	}

	public static int moveReadPositionByFloat(int place) {
		return place + guidobinaryparser.C_FLOAT_SIZE;
	}

	public static int moveReadPositionByString(int place, String str) {
		return place + str.length() + 1; // 1 for the escape char
	}

	public static void parseIntoDrawCommands(byte[] data, guidocommandbattery commandBattery) {
		int place = 0;
		while ((place >= 0) && (place < data.length)) {
			char head = guidobinaryparser.getUnsignedChar(data, place);
			place = guidobinaryparser.moveReadPositionByChar(place);
			if (head == guidobinaryparser.BEGIN_DRAW_CODE) {
				place = guidobinaryparser.BeginDraw(data, place, commandBattery);
			} else if (head == guidobinaryparser.END_DRAW_CODE) {
				place = guidobinaryparser.EndDraw(data, place, commandBattery);
			} else if (head == guidobinaryparser.LINE_CODE) {
				place = guidobinaryparser.Line(data, place, commandBattery);
			} else if (head == guidobinaryparser.POLYGON_CODE) {
				place = guidobinaryparser.Polygon(data, place, commandBattery);
			} else if (head == guidobinaryparser.RECTANGLE_CODE) {
				place = guidobinaryparser.Rectangle(data, place, commandBattery);
			} else if (head == guidobinaryparser.SET_MUSIC_FONT_CODE) {
				place = guidobinaryparser.SetMusicFont(data, place, commandBattery);
			} else if (head == guidobinaryparser.GET_MUSIC_FONT_CODE) {
				place = guidobinaryparser.GetMusicFont(data, place, commandBattery);
			} else if (head == guidobinaryparser.SET_TEXT_FONT_CODE) {
				place = guidobinaryparser.SetTextFont(data, place, commandBattery);
			} else if (head == guidobinaryparser.GET_TEXT_FONT_CODE) {
				place = guidobinaryparser.GetTextFont(data, place, commandBattery);
			} else if (head == guidobinaryparser.PUSH_PEN) {
				place = guidobinaryparser.PushPen(data, place, commandBattery);
			} else if (head == guidobinaryparser.POP_PEN) {
				place = guidobinaryparser.PopPen(data, place, commandBattery);
			} else if (head == guidobinaryparser.PUSH_FILL_COLOR) {
				place = guidobinaryparser.PushFillColor(data, place, commandBattery);
			} else if (head == guidobinaryparser.POP_FILL_COLOR) {
				place = guidobinaryparser.PopFillColor(data, place, commandBattery);
			} else if (head == guidobinaryparser.SET_SCALE_CODE) {
				place = guidobinaryparser.SetScale(data, place, commandBattery);
			} else if (head == guidobinaryparser.SET_ORIGIN_CODE) {
				place = guidobinaryparser.SetOrigin(data, place, commandBattery);
			} else if (head == guidobinaryparser.OFFSET_ORIGIN_CODE) {
				place = guidobinaryparser.OffsetOrigin(data, place, commandBattery);
			} else if (head == guidobinaryparser.NOTIFY_SIZE_CODE) {
				place = guidobinaryparser.NotifySize(data, place, commandBattery);
			} else if (head == guidobinaryparser.DRAW_MUSIC_SYMBOL_CODE) {
				place = guidobinaryparser.DrawMusicSymbol(data, place, commandBattery);
			} else if (head == guidobinaryparser.DRAW_STRING_CODE) {
				place = guidobinaryparser.DrawString(data, place, commandBattery);
			} else if (head == guidobinaryparser.SET_FONT_COLOR_CODE) {
				place = guidobinaryparser.SetFontColor(data, place, commandBattery);
			} else if (head == guidobinaryparser.GET_FONT_COLOR_CODE) {
				place = guidobinaryparser.GetFontColor(data, place, commandBattery);
			} else if (head == guidobinaryparser.SET_FONT_ALIGN_CODE) {
				place = guidobinaryparser.SetFontAlign(data, place, commandBattery);
			} else if (head == guidobinaryparser.SELECT_PEN_COLOR_CODE) {
				place = guidobinaryparser.SelectPenColor(data, place, commandBattery);
			} else if (head == guidobinaryparser.PUSH_PEN_COLOR_CODE) {
				place = guidobinaryparser.PushPenColor(data, place, commandBattery);
			} else if (head == guidobinaryparser.POP_PEN_COLOR_CODE) {
				place = guidobinaryparser.PopPenColor(data, place, commandBattery);
			} else if (head == guidobinaryparser.PUSH_PEN_WIDTH_CODE) {
				place = guidobinaryparser.PushPenWidth(data, place, commandBattery);
			} else if (head == guidobinaryparser.POP_PEN_WIDTH_CODE) {
				place = guidobinaryparser.PopPenWidth(data, place, commandBattery);
			} else {
				// panic
				// Log.w("SimpleGuidoEditor",
				// "have no clue how to interpret the number below...");
				// Log.w("SimpleGuidoEditor", Integer.toString(head));
				place = -1;
			}
		}
	}

	public static int BeginDraw(byte[] data, int place, guidocommandbattery commandBattery) {
		// ////////////////////////
		commandBattery.storeBeginDrawCommand();
		return place;
	}

	public static int EndDraw(byte[] data, int place, guidocommandbattery commandBattery) {
		// /////////////////////
		commandBattery.storeEndDrawCommand();
		return place;
	}

	public static int Line(byte[] data, int place, guidocommandbattery commandBattery) {
		float x1 = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		float y1 = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		float x2 = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		float y2 = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		// /////////////////////////
		commandBattery.storeLineCommand(x1, y1, x2, y2);
		return place;
	}

	public static int Polygon(byte[] data, int place, guidocommandbattery commandBattery) {
		int count = guidobinaryparser.getInt32(data, place);
		place = guidobinaryparser.moveReadPositionByInt32(place);
		List<Float> xCoords = new ArrayList<Float>();
		List<Float> yCoords = new ArrayList<Float>();
		for (int i = 0; i < count; i++) {
			float x = guidobinaryparser.getFloat(data, place);
			place = guidobinaryparser.moveReadPositionByFloat(place);
			xCoords.add(x);
		}
		for (int i = 0; i < count; i++) {
			float y = guidobinaryparser.getFloat(data, place);
			place = guidobinaryparser.moveReadPositionByFloat(place);
			yCoords.add(y);
		}
		// /////////////////////////
		commandBattery.storePolygonCommand(xCoords, yCoords, count);
		return place;
	}

	public static int Rectangle(byte[] data, int place, guidocommandbattery commandBattery) {
		float left = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		float top = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		float right = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		float bottom = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		// /////////////////////////
		commandBattery.storeRectangleCommand(left, top, right, bottom);
		return place;
	}

	public static int SetMusicFont(byte[] data, int place, guidocommandbattery commandBattery) {
		String name = guidobinaryparser.getString(data, place, -1);
		place = guidobinaryparser.moveReadPositionByString(place, name);
		int size = guidobinaryparser.getInt32(data, place);
		place = guidobinaryparser.moveReadPositionByInt32(place);
		int properties = guidobinaryparser.getInt32(data, place);
		place = guidobinaryparser.moveReadPositionByInt32(place);
		// /////////////////////////
		commandBattery.storeSetMusicFontCommand(name, size, properties);
		return place;
	}

	public static int GetMusicFont(byte[] data, int place, guidocommandbattery commandBattery) {
		// /////////////////////////
		commandBattery.storeGetMusicFontCommand();
		return place;
	}

	public static int SetTextFont(byte[] data, int place, guidocommandbattery commandBattery) {
		String name = guidobinaryparser.getString(data, place, -1);
		place = guidobinaryparser.moveReadPositionByString(place, name);
		int size = guidobinaryparser.getInt32(data, place);
		place = guidobinaryparser.moveReadPositionByInt32(place);
		int properties = guidobinaryparser.getInt32(data, place);
		place = guidobinaryparser.moveReadPositionByInt32(place);
		// /////////////////////////
		commandBattery.storeSetTextFontCommand(name, size, properties);
		return place;
	}

	public static int GetTextFont(byte[] data, int place, guidocommandbattery commandBattery) {
		// /////////////////////////
		commandBattery.storeGetTextFontCommand();
		return place;
	}

	public static int PushPen(byte[] data, int place, guidocommandbattery commandBattery) {
		// read Color
		char alpha = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char red = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char green = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char blue = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);

		// read Width
		float width = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);

		commandBattery.storePushPenCommand(alpha, red, green, blue, width);
		return place;
	}

	public static int PopPen(byte[] data, int place, guidocommandbattery commandBattery) {
		commandBattery.storePopPenCommand();
		return place;
	}

	public static int PushFillColor(byte[] data, int place, guidocommandbattery commandBattery) {
		// read Color
		char alpha = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char red = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char green = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char blue = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);

		commandBattery.storePushFillColorCommand(alpha, red, green, blue);
		return place;
	}

	public static int PopFillColor(byte[] data, int place, guidocommandbattery commandBattery) {
		commandBattery.storePopFillColorCommand();
		return place;
	}

	public static int SetScale(byte[] data, int place, guidocommandbattery commandBattery) {
		float x = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		float y = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		// /////////////////////////
		commandBattery.storeSetScaleCommand(x, y);
		return place;
	}

	public static int SetOrigin(byte[] data, int place, guidocommandbattery commandBattery) {
		float x = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		float y = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		// /////////////////////////
		commandBattery.storeSetOriginCommand(x, y);
		return place;
	}

	public static int OffsetOrigin(byte[] data, int place, guidocommandbattery commandBattery) {
		float x = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		float y = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		// /////////////////////////
		commandBattery.storeOffsetOriginCommand(x, y);
		return place;
	}

	public static int NotifySize(byte[] data, int place, guidocommandbattery commandBattery) {
		int width = guidobinaryparser.getInt32(data, place);
		place = guidobinaryparser.moveReadPositionByInt32(place);
		int height = guidobinaryparser.getInt32(data, place);
		place = guidobinaryparser.moveReadPositionByInt32(place);
		// /////////////////////////
		commandBattery.storeNotifySizeCommand(width, height);
		return place;
	}

	public static int DrawMusicSymbol(byte[] data, int place, guidocommandbattery commandBattery) {
		float x = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		float y = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		int inSymbolID = guidobinaryparser.getUnsignedInt32(data, place);
		place = guidobinaryparser.moveReadPositionByUnsignedInt32(place);
		// /////////////////////////
		commandBattery.storeDrawMusicSymbolCommand(x, y, inSymbolID);
		return place;
	}

	public static int DrawString(byte[] data, int place, guidocommandbattery commandBattery) {
		float x = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		float y = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		int inCharCount = guidobinaryparser.getInt32(data, place);
		place = guidobinaryparser.moveReadPositionByInt32(place);
		String s = guidobinaryparser.getString(data, place, inCharCount);
		place = guidobinaryparser.moveReadPositionByString(place, s);
		// because not null terminated...
		place = place - 1;
		// /////////////////////////
		commandBattery.storeDrawStringCommand(x, y, s, inCharCount);
		return place;
	}

	public static int SetFontColor(byte[] data, int place, guidocommandbattery commandBattery) {
		char alpha = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char red = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char green = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char blue = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		// ///////////////////////////////////////
		commandBattery.storeSetFontColorCommand(alpha, red, green, blue);
		return place;
	}

	public static int GetFontColor(byte[] data, int place, guidocommandbattery commandBattery) {
		// /////////////////////////
		commandBattery.storeGetFontColorCommand();
		return place;
	}

	public static int SetFontAlign(byte[] data, int place, guidocommandbattery commandBattery) {
		int inAlign = guidobinaryparser.getUnsignedInt32(data, place);
		place = guidobinaryparser.moveReadPositionByUnsignedInt32(place);
		// ///////////////////////////////////////
		commandBattery.storeSetFontAlignCommand(inAlign);
		return place;
	}

	public static int SelectPenColor(byte[] data, int place, guidocommandbattery commandBattery) {
		char alpha = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char red = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char green = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char blue = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		// ///////////////////////////////////////
		commandBattery.storeSelectPenColorCommand(alpha, red, green, blue);
		return place;
	}

	public static int PushPenColor(byte[] data, int place, guidocommandbattery commandBattery) {
		char alpha = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char red = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char green = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		char blue = guidobinaryparser.getUnsignedChar(data, place);
		place = guidobinaryparser.moveReadPositionByChar(place);
		// ///////////////////////////////////////
		commandBattery.storePushPenColorCommand(alpha, red, green, blue);
		return place;
	}

	public static int PopPenColor(byte[] data, int place, guidocommandbattery commandBattery) {
		// ///////////////////////////////////////
		commandBattery.storePopPenColorCommand();
		return place;
	}

	public static int PushPenWidth(byte[] data, int place, guidocommandbattery commandBattery) {
		float width = guidobinaryparser.getFloat(data, place);
		place = guidobinaryparser.moveReadPositionByFloat(place);
		// /////////////////////////
		commandBattery.storePushPenWidthCommand(width);
		return place;
	}

	public static int PopPenWidth(byte[] data, int place, guidocommandbattery commandBattery) {
		// ///////////////////////////////////////
		commandBattery.storePopPenWidthCommand();
		return place;
	}

}