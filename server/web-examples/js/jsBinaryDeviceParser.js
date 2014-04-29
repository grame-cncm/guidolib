/*
  If you want to use dynamic resizing, make sure that the div containing the
  canvas has the id "canvasContainer".
*/

/* creates guido namespace */
var gU1D0pAR$ER = {}

/* codes for parser */
gU1D0pAR$ER.BEGIN_DRAW_CODE = 0;
gU1D0pAR$ER.END_DRAW_CODE = 1;
gU1D0pAR$ER.LINE_CODE = 5;
gU1D0pAR$ER.POLYGON_CODE = 9;
gU1D0pAR$ER.RECTANGLE_CODE = 10;
gU1D0pAR$ER.SET_MUSIC_FONT_CODE = 11;
gU1D0pAR$ER.GET_MUSIC_FONT_CODE = 12;
gU1D0pAR$ER.SET_TEXT_FONT_CODE = 13;
gU1D0pAR$ER.GET_TEXT_FONT_CODE = 14;
gU1D0pAR$ER.SET_SCALE_CODE = 27;
gU1D0pAR$ER.SET_ORIGIN_CODE = 28;
gU1D0pAR$ER.OFFSET_ORIGIN_CODE = 29;
gU1D0pAR$ER.NOTIFY_SIZE_CODE = 36;
gU1D0pAR$ER.DRAW_MUSIC_SYMBOL_CODE = 39;
gU1D0pAR$ER.DRAW_STRING_CODE = 40;
gU1D0pAR$ER.SET_FONT_COLOR_CODE = 41;
gU1D0pAR$ER.GET_FONT_COLOR_CODE = 42;
gU1D0pAR$ER.SET_FONT_ALIGN_CODE = 45;
gU1D0pAR$ER.SELECT_PEN_COLOR_CODE = 49;
gU1D0pAR$ER.POP_PEN_COLOR_CODE = 52;
gU1D0pAR$ER.PUSH_PEN_WIDTH_CODE = 53;
gU1D0pAR$ER.POP_PEN_WIDTH_CODE = 54;

/* holds registered functions */
gU1D0pAR$ER.drawFunctions = [];

/* getters for data types */

gU1D0pAR$ER.getUnsignedChar = function(data, place) {
  var head = new Uint8Array(data, place, Uint8Array.BYTES_PER_ELEMENT);
  return head[0];
}

gU1D0pAR$ER.getUnsignedInt32 = function(data, place) {
  var head = data.slice(place, place + Uint32Array.BYTES_PER_ELEMENT);
  var intar = new Uint32Array(head, 0, 1);
  return intar[0];
}

gU1D0pAR$ER.getInt32 = function(data, place) {
  var head = data.slice(place, place + Int32Array.BYTES_PER_ELEMENT);
  var intar = new Int32Array(head, 0, 1);
  return intar[0];
}

gU1D0pAR$ER.getFloat = function(data, place) {
  var head = data.slice(place, place + Float32Array.BYTES_PER_ELEMENT);
  var floatar = new Float32Array(head, 0, 1);
  return floatar[0];
}

gU1D0pAR$ER.getString = function(data, place, n) {
  var head = new Uint8Array(data, place);
  var str = "";
  var i = 0;
  if (!n) {
    n = 1000000;
  }
  while ((String.fromCharCode(head[i]) != '\0') && (i < n)) {
    str += String.fromCharCode(head[i]);
    if (i == 100) {
      console.log("INTRACTABLE string", str);
      break;
    }
    i = i + 1;
  }
  return str;
}

/* move read position */

gU1D0pAR$ER.moveReadPositionByChar = function(place) {
  return place + Uint8Array.BYTES_PER_ELEMENT;
}

gU1D0pAR$ER.moveReadPositionByUnsignedInt32 = function(place) {
  return place + Uint32Array.BYTES_PER_ELEMENT;
}

gU1D0pAR$ER.moveReadPositionByInt32 = function(place) {
  return place + Int32Array.BYTES_PER_ELEMENT;
}

gU1D0pAR$ER.moveReadPositionByFloat = function(place) {
  return place + Float32Array.BYTES_PER_ELEMENT;
}

gU1D0pAR$ER.moveReadPositionByString = function(place, str) {
  return place + str.length + 1; // 1 for the escape char
}

gU1D0pAR$ER.parseGuidoBinary = function(data, place) {
  while ((place >= 0) && (place < data.byteLength)) {
    var head = gU1D0pAR$ER.getUnsignedChar(data, place);
    place = gU1D0pAR$ER.moveReadPositionByChar(place);
    if (head == gU1D0pAR$ER.BEGIN_DRAW_CODE) {
      place = gU1D0pAR$ER.BeginDraw(data, place);
    } else if (head == gU1D0pAR$ER.END_DRAW_CODE) {
      place = gU1D0pAR$ER.EndDraw(data, place);
    } else if (head == gU1D0pAR$ER.LINE_CODE) {
      place = gU1D0pAR$ER.Line(data, place);
    } else if (head == gU1D0pAR$ER.POLYGON_CODE) {
      place = gU1D0pAR$ER.Polygon(data, place);
    } else if (head == gU1D0pAR$ER.RECTANGLE_CODE) {
      place = gU1D0pAR$ER.Rectangle(data, place);
    } else if (head == gU1D0pAR$ER.SET_MUSIC_FONT_CODE) {
      place = gU1D0pAR$ER.SetMusicFont(data, place);
    } else if (head == gU1D0pAR$ER.GET_MUSIC_FONT_CODE) {
      place = gU1D0pAR$ER.GetMusicFont(data, place);
    } else if (head == gU1D0pAR$ER.SET_TEXT_FONT_CODE) {
      place = gU1D0pAR$ER.SetTextFont(data, place);
    } else if (head == gU1D0pAR$ER.GET_TEXT_FONT_CODE) {
      place = gU1D0pAR$ER.GetTextFont(data, place);
    } else if (head == gU1D0pAR$ER.SET_SCALE_CODE) {
      place = gU1D0pAR$ER.SetScale(data, place);
    } else if (head == gU1D0pAR$ER.SET_ORIGIN_CODE) {
      place = gU1D0pAR$ER.SetOrigin(data, place);
    } else if (head == gU1D0pAR$ER.OFFSET_ORIGIN_CODE) {
      place = gU1D0pAR$ER.OffsetOrigin(data, place);
    } else if (head == gU1D0pAR$ER.NOTIFY_SIZE_CODE) {
      place = gU1D0pAR$ER.NotifySize(data, place);
    } else if (head == gU1D0pAR$ER.DRAW_MUSIC_SYMBOL_CODE) {
      place = gU1D0pAR$ER.DrawMusicSymbol(data, place);
    } else if (head == gU1D0pAR$ER.DRAW_STRING_CODE) {
      place = gU1D0pAR$ER.DrawString(data, place);
    } else if (head == gU1D0pAR$ER.SET_FONT_COLOR_CODE) {
      place = gU1D0pAR$ER.SetFontColor(data, place);
    } else if (head == gU1D0pAR$ER.GET_FONT_COLOR_CODE) {
      place = gU1D0pAR$ER.GetFontColor(data, place);
    } else if (head == gU1D0pAR$ER.SET_FONT_ALIGN_CODE) {
      place = gU1D0pAR$ER.SetFontAlign(data, place);
    } else if (head == gU1D0pAR$ER.SELECT_PEN_COLOR_CODE) {
      place = gU1D0pAR$ER.SelectPenColor(data, place);
    } else if (head == gU1D0pAR$ER.POP_PEN_COLOR_CODE) {
      place = gU1D0pAR$ER.PopPenColor(data, place);
    } else if (head == gU1D0pAR$ER.PUSH_PEN_WIDTH_CODE) {
      place = gU1D0pAR$ER.PushPenWidth(data, place);
    } else if (head == gU1D0pAR$ER.POP_PEN_WIDTH_CODE) {
      place = gU1D0pAR$ER.PopPenWidth(data, place);
    } else {
      // panic
      console.log("don't know ", head);
      place = -1;
    }
  }
  console.log("finished drawing");
}

gU1D0pAR$ER.BeginDraw = function(data, place) {
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["BeginDraw"]();
  return place;
}

gU1D0pAR$ER.EndDraw = function(data, place) {
  ///////////////////////
  gU1D0pAR$ER.drawFunctions["EndDraw"]();
  return place;
}

gU1D0pAR$ER.Line = function(data, place) {
  var x1 = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var y1 = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var x2 = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var y2 = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["Line"](x1, y1, x2, y2);
  return place;
}

gU1D0pAR$ER.Polygon = function(data, place) {
  var count = gU1D0pAR$ER.getInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByInt32(place);
  xCoords = [];
  yCoords = [];
  for (var i = 0; i < count; i++) {
    var x = gU1D0pAR$ER.getFloat(data, place);
    place = gU1D0pAR$ER.moveReadPositionByFloat(place);
    xCoords.push(x);
  }
  for (var i = 0; i < count; i++) {
    var y = gU1D0pAR$ER.getFloat(data, place);
    place = gU1D0pAR$ER.moveReadPositionByFloat(place);
    yCoords.push(y);
  }
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["Polygon"](xCoords, yCoords, count);
  return place;
}

gU1D0pAR$ER.Rectangle = function(data, place) {
  var left = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var top = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var right = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var bottom = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["Rectangle"](left, top, right, bottom);
  return place;
}

gU1D0pAR$ER.SetMusicFont = function(data, place) {
  var name = gU1D0pAR$ER.getString(data, place);
  place = gU1D0pAR$ER.moveReadPositionByString(place, name);
  var size = gU1D0pAR$ER.getInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByInt32(place);
  var properties = gU1D0pAR$ER.getInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByInt32(place);
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["SetMusicFont"](name, size, properties);
  return place;
}

gU1D0pAR$ER.GetMusicFont = function(data, place) {
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["GetMusicFont"]();
  return place;
}

gU1D0pAR$ER.SetTextFont = function(data, place) {
  var name = gU1D0pAR$ER.getString(data, place);
  place = gU1D0pAR$ER.moveReadPositionByString(place, name);
  var size = gU1D0pAR$ER.getInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByInt32(place);
  var properties = gU1D0pAR$ER.getInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByInt32(place);
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["SetTextFont"](name, size, properties);
  return place;
}

gU1D0pAR$ER.GetTextFont = function(data, place) {
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["GetTextFont"]();
  return place;
}

gU1D0pAR$ER.SetScale = function(data, place) {
  var x = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var y = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["SetScale"](x, y);
  return place;
}

gU1D0pAR$ER.SetOrigin = function(data, place) {
  var x = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var y = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["SetOrigin"](x, y);
  return place;
}

gU1D0pAR$ER.OffsetOrigin = function(data, place) {
  var x = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var y = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["OffsetOrigin"](x, y);
  return place;
}

gU1D0pAR$ER.NotifySize = function(data, place) {
  var width = gU1D0pAR$ER.getInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByInt32(place);
  var height = gU1D0pAR$ER.getInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByInt32(place);
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["NotifySize"](width, height);
  return place;
}

gU1D0pAR$ER.DrawMusicSymbol = function(data, place) {
  var x = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var y = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var inSymbolID = gU1D0pAR$ER.getUnsignedInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByUnsignedInt32(place);
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["DrawMusicSymbol"](x, y, inSymbolID);
  return place;
}

gU1D0pAR$ER.DrawString = function(data, place) {
  var x = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var y = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var inCharCount = gU1D0pAR$ER.getInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByInt32(place);
  var s = gU1D0pAR$ER.getString(data, place, inCharCount);
  place = gU1D0pAR$ER.moveReadPositionByString(place, s);
  // because not null terminated...
  place = place - 1;
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["DrawString"](x, y, s, inCharCount);
  return place;
}

gU1D0pAR$ER.SetFontColor = function(data, place) {
  var alpha = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var red = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var green = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var blue = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  /////////////////////////////////////////
  gU1D0pAR$ER.drawFunctions["SetFontColor"](alpha, red, green, blue);
  return place;
}

gU1D0pAR$ER.GetFontColor = function(data, place) {
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["GetFontColor"]();
  return place;
}

gU1D0pAR$ER.SetFontAlign = function(data, place) {
  var inAlign = gU1D0pAR$ER.getUnsignedInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByUnsignedInt32(place);
  /////////////////////////////////////////
  gU1D0pAR$ER.drawFunctions["SetFontAlign"](inAlign);
  return place;
}

gU1D0pAR$ER.SelectPenColor = function(data, place) {
  var alpha = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var red = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var green = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var blue = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  /////////////////////////////////////////
  gU1D0pAR$ER.drawFunctions["SelectPenColor"](alpha, red, green, blue);
  return place;
}

gU1D0pAR$ER.PopPenColor = function(data, place) {
  /////////////////////////////////////////
  gU1D0pAR$ER.drawFunctions["PopPenColor"]();
  return place;
}

gU1D0pAR$ER.PushPenWidth = function(data, place) {
  var width = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  ///////////////////////////
  gU1D0pAR$ER.drawFunctions["PushPenWidth"](width);
  return place;
}

gU1D0pAR$ER.PopPenWidth = function(data, place) {
  /////////////////////////////////////////
  gU1D0pAR$ER.drawFunctions["PopPenWidth"]();
  return place;
}


/* register functions */
gU1D0pAR$ER.registerBeginDraw = function(fn) {
  gU1D0pAR$ER.drawFunctions["BeginDraw"] = fn;
}

gU1D0pAR$ER.registerEndDraw = function(fn) {
  gU1D0pAR$ER.drawFunctions["EndDraw"] = fn;
}

gU1D0pAR$ER.registerLine = function(fn) {
  gU1D0pAR$ER.drawFunctions["Line"] = fn;
}

gU1D0pAR$ER.registerPolygon = function(fn) {
  gU1D0pAR$ER.drawFunctions["Polygon"] = fn;
}

gU1D0pAR$ER.registerRectangle = function(fn) {
  gU1D0pAR$ER.drawFunctions["Rectangle"] = fn
}

gU1D0pAR$ER.registerSetMusicFont = function(fn) {
  gU1D0pAR$ER.drawFunctions["SetMusicFont"] = fn
}

gU1D0pAR$ER.registerGetMusicFont = function(fn) {
  gU1D0pAR$ER.drawFunctions["GetMusicFont"] = fn
}

gU1D0pAR$ER.registerSetTextFont = function(fn) {
  gU1D0pAR$ER.drawFunctions["SetTextFont"] = fn
}

gU1D0pAR$ER.registerGetTextFont = function(fn) {
  gU1D0pAR$ER.drawFunctions["GetTextFont"] = fn
}

gU1D0pAR$ER.registerSetScale = function(fn) {
  gU1D0pAR$ER.drawFunctions["SetScale"] = fn
}

gU1D0pAR$ER.registerSetOrigin = function(fn) {
  gU1D0pAR$ER.drawFunctions["SetOrigin"] = fn
}

gU1D0pAR$ER.registerOffsetOrigin = function(fn) {
  gU1D0pAR$ER.drawFunctions["OffsetOrigin"] = fn
}

gU1D0pAR$ER.registerNotifySize = function(fn) {
  gU1D0pAR$ER.drawFunctions["NotifySize"] = fn
}

gU1D0pAR$ER.registerDrawMusicSymbol = function(fn) {
  gU1D0pAR$ER.drawFunctions["DrawMusicSymbol"] = fn
}

gU1D0pAR$ER.registerDrawString = function(fn) {
  gU1D0pAR$ER.drawFunctions["DrawString"] = fn
}

gU1D0pAR$ER.registerSetFontColor = function(fn) {
  gU1D0pAR$ER.drawFunctions["SetFontColor"] = fn
}

gU1D0pAR$ER.registerGetFontColor = function(fn) {
  gU1D0pAR$ER.drawFunctions["GetFontColor"] = fn
}

gU1D0pAR$ER.registerSetFontAlign = function(fn) {
  gU1D0pAR$ER.drawFunctions["SetFontAlign"] = fn
}

gU1D0pAR$ER.registerSelectPenColor = function(fn) {
  gU1D0pAR$ER.drawFunctions["SelectPenColor"] = fn
}

gU1D0pAR$ER.registerPopPenColor = function(fn) {
  gU1D0pAR$ER.drawFunctions["PopPenColor"] = fn
}

gU1D0pAR$ER.registerPushPenWidth = function(fn) {
  gU1D0pAR$ER.drawFunctions["PushPenWidth"] = fn
}

gU1D0pAR$ER.registerPopPenWidth = function(fn) {
  gU1D0pAR$ER.drawFunctions["PopPenWidth"] = fn
}
