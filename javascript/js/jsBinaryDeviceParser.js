/*
  If you want to use dynamic resizing, make sure that the div containing the
  canvas has the id "canvasContainer".
*/

/* creates guido namespace */
var gU1D0pAR$ER = {}

/* Command codes for parser */
gU1D0pAR$ER.BEGIN_DRAW_CODE = 0;
gU1D0pAR$ER.END_DRAW_CODE = 1;
gU1D0pAR$ER.LINE_CODE = 5;
gU1D0pAR$ER.POLYGON_CODE = 9;
gU1D0pAR$ER.RECTANGLE_CODE = 10;
gU1D0pAR$ER.SET_MUSIC_FONT_CODE = 11;
gU1D0pAR$ER.GET_MUSIC_FONT_CODE = 12;
gU1D0pAR$ER.SET_TEXT_FONT_CODE = 13;
gU1D0pAR$ER.GET_TEXT_FONT_CODE = 14;
gU1D0pAR$ER.PUSH_PEN = 17;
gU1D0pAR$ER.POP_PEN = 18;
gU1D0pAR$ER.PUSH_FILL_COLOR = 19;
gU1D0pAR$ER.POP_FILL_COLOR = 20;
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
gU1D0pAR$ER.PUSH_PEN_COLOR_CODE = 51;
gU1D0pAR$ER.POP_PEN_COLOR_CODE = 52;
gU1D0pAR$ER.PUSH_PEN_WIDTH_CODE = 53;
gU1D0pAR$ER.POP_PEN_WIDTH_CODE = 54;

/* holds registered functions */
gU1D0pAR$ER.drawFunctions = [];

/* getters for data types from buffer 'data' at index 'place' */
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

/* 
 * Parse the binary export 
 * data : the binary
 * place : index to begin 
 * opts : options (only verbose mode )
 */
gU1D0pAR$ER.parseGuidoBinary = function(data, place, opts) {
  opts = opts ? opts : {};
  while ((place >= 0) && (place < data.byteLength)) {
    var head = gU1D0pAR$ER.getUnsignedChar(data, place);
    place = gU1D0pAR$ER.moveReadPositionByChar(place);
    if (head == gU1D0pAR$ER.BEGIN_DRAW_CODE) {
      place = gU1D0pAR$ER.BeginDraw(data, place, opts);
    } else if (head == gU1D0pAR$ER.END_DRAW_CODE) {
      place = gU1D0pAR$ER.EndDraw(data, place, opts);
    } else if (head == gU1D0pAR$ER.LINE_CODE) {
      place = gU1D0pAR$ER.Line(data, place, opts);
    } else if (head == gU1D0pAR$ER.POLYGON_CODE) {
      place = gU1D0pAR$ER.Polygon(data, place, opts);
    } else if (head == gU1D0pAR$ER.RECTANGLE_CODE) {
      place = gU1D0pAR$ER.Rectangle(data, place, opts);
    } else if (head == gU1D0pAR$ER.SET_MUSIC_FONT_CODE) {
      place = gU1D0pAR$ER.SetMusicFont(data, place, opts);
    } else if (head == gU1D0pAR$ER.GET_MUSIC_FONT_CODE) {
      place = gU1D0pAR$ER.GetMusicFont(data, place, opts);
    } else if (head == gU1D0pAR$ER.SET_TEXT_FONT_CODE) {
      place = gU1D0pAR$ER.SetTextFont(data, place, opts);
    } else if (head == gU1D0pAR$ER.GET_TEXT_FONT_CODE) {
      place = gU1D0pAR$ER.GetTextFont(data, place, opts);
    } else if (head == gU1D0pAR$ER.POP_PEN) {
      place = gU1D0pAR$ER.PopPen(data, place, opts);
    } else if (head == gU1D0pAR$ER.PUSH_PEN) {
      place = gU1D0pAR$ER.PushPen(data, place, opts);
    } else if (head == gU1D0pAR$ER.PUSH_FILL_COLOR) {
      place = gU1D0pAR$ER.PushFillColor(data, place, opts);
    } else if (head == gU1D0pAR$ER.POP_FILL_COLOR) {
      place = gU1D0pAR$ER.PopFillColor(data, place, opts);
    } else if (head == gU1D0pAR$ER.SET_SCALE_CODE) {
      place = gU1D0pAR$ER.SetScale(data, place, opts);
    } else if (head == gU1D0pAR$ER.SET_ORIGIN_CODE) {
      place = gU1D0pAR$ER.SetOrigin(data, place, opts);
    } else if (head == gU1D0pAR$ER.OFFSET_ORIGIN_CODE) {
      place = gU1D0pAR$ER.OffsetOrigin(data, place, opts);
    } else if (head == gU1D0pAR$ER.NOTIFY_SIZE_CODE) {
      place = gU1D0pAR$ER.NotifySize(data, place, opts);
    } else if (head == gU1D0pAR$ER.DRAW_MUSIC_SYMBOL_CODE) {
      place = gU1D0pAR$ER.DrawMusicSymbol(data, place, opts);
    } else if (head == gU1D0pAR$ER.DRAW_STRING_CODE) {
      place = gU1D0pAR$ER.DrawString(data, place, opts);
    } else if (head == gU1D0pAR$ER.SET_FONT_COLOR_CODE) {
      place = gU1D0pAR$ER.SetFontColor(data, place, opts);
    } else if (head == gU1D0pAR$ER.GET_FONT_COLOR_CODE) {
      place = gU1D0pAR$ER.GetFontColor(data, place, opts);
    } else if (head == gU1D0pAR$ER.SET_FONT_ALIGN_CODE) {
      place = gU1D0pAR$ER.SetFontAlign(data, place, opts);
    } else if (head == gU1D0pAR$ER.SELECT_PEN_COLOR_CODE) {
      place = gU1D0pAR$ER.SelectPenColor(data, place, opts);
    } else if (head == gU1D0pAR$ER.PUSH_PEN_COLOR_CODE) {
      place = gU1D0pAR$ER.PushPenColor(data, place, opts);
    } else if (head == gU1D0pAR$ER.POP_PEN_COLOR_CODE) {
      place = gU1D0pAR$ER.PopPenColor(data, place, opts);
    } else if (head == gU1D0pAR$ER.PUSH_PEN_WIDTH_CODE) {
      place = gU1D0pAR$ER.PushPenWidth(data, place, opts);
    } else if (head == gU1D0pAR$ER.POP_PEN_WIDTH_CODE) {
      place = gU1D0pAR$ER.PopPenWidth(data, place, opts);
    } else {
      // panic
      console.log("don't know ", head);
      place = -1;
    }
  }
  console.log("finished drawing");
}

gU1D0pAR$ER.BeginDraw = function(data, place, opts) {
  //////////////////////////
  if (opts.verbose == true) {
    console.log("BeginDraw");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["BeginDraw"]();
  return place;
}

gU1D0pAR$ER.EndDraw = function(data, place, opts) {
  ///////////////////////
  if (opts.verbose == true) {
    console.log("EndDraw");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["EndDraw"]();
  return place;
}

gU1D0pAR$ER.Line = function(data, place, opts) {
  var x1 = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var y1 = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var x2 = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var y2 = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  ///////////////////////////
  if (opts.verbose == true) {
    console.log("Line");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["Line"](x1, y1, x2, y2);
  return place;
}

gU1D0pAR$ER.Polygon = function(data, place, opts) {
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
  if (opts.verbose == true) {
    console.log("Polygon");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["Polygon"](xCoords, yCoords, count);
  return place;
}

gU1D0pAR$ER.Rectangle = function(data, place, opts) {
  var left = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var top = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var right = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var bottom = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  ///////////////////////////
  if (opts.verbose == true) {
    console.log("Rectangle");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["Rectangle"](left, top, right, bottom);
  return place;
}

gU1D0pAR$ER.SetMusicFont = function(data, place, opts) {
  var name = gU1D0pAR$ER.getString(data, place);
  place = gU1D0pAR$ER.moveReadPositionByString(place, name);
  var size = gU1D0pAR$ER.getInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByInt32(place);
  var properties = gU1D0pAR$ER.getInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByInt32(place);
  ///////////////////////////
  if (opts.verbose == true) {
    console.log("SetMusicFont", name, size, properties);
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["SetMusicFont"](name, size, properties);
  return place;
}

gU1D0pAR$ER.GetMusicFont = function(data, place, opts) {
  ///////////////////////////
  if (opts.verbose == true) {
    console.log("GetMusicFont");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["GetMusicFont"]();
  return place;
}

gU1D0pAR$ER.SetTextFont = function(data, place, opts) {
  var name = gU1D0pAR$ER.getString(data, place);
  place = gU1D0pAR$ER.moveReadPositionByString(place, name);
  var size = gU1D0pAR$ER.getInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByInt32(place);
  var properties = gU1D0pAR$ER.getInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByInt32(place);
  ///////////////////////////
  if (opts.verbose == true) {
    console.log("SetTextFont");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["SetTextFont"](name, size, properties);
  return place;
}

gU1D0pAR$ER.GetTextFont = function(data, place, opts) {
  ///////////////////////////
  if (opts.verbose == true) {
    console.log("GetTextFont");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["GetTextFont"]();
  return place;
}

// PUSH_PEN = 17;
gU1D0pAR$ER.PushPen = function(data, place, opts) {
  // read Color
  var alpha = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var red = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var green = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var blue = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  
  // read Width
  var width = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  
  if (opts.verbose == true) {
    console.log("PushPen");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["PushPen"](alpha, red, green, blue, width);
  return place;
}

// POP_PEN = 18;
gU1D0pAR$ER.PopPen = function(data, place, opts) {
  
  if (opts.verbose == true) {
    console.log("PopPen");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["PopPen"]();
  return place;
}

// PUSH_FILL_COLOR = 19
gU1D0pAR$ER.PushFillColor = function(data, place, opts) {
  // read Color
  var alpha = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var red = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var green = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var blue = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  if (opts.verbose == true) {
    console.log("PushFillColor");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["PushFillColor"](alpha, red, green, blue);
  return place;
}

// POP_FILL_COLOR = 20
gU1D0pAR$ER.PopFillColor = function(data, place, opts) {
  
  if (opts.verbose == true) {
    console.log("PopFillColor");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["PopFillColor"]();
  return place;
}

gU1D0pAR$ER.SetScale = function(data, place, opts) {
  var x = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var y = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  ///////////////////////////
  if (opts.verbose == true) {
    console.log("SetScale");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["SetScale"](x, y);
  return place;
}

gU1D0pAR$ER.SetOrigin = function(data, place, opts) {
  var x = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var y = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  ///////////////////////////
  if (opts.verbose == true) {
    console.log("SetOrigin");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["SetOrigin"](x, y);
  return place;
}

gU1D0pAR$ER.OffsetOrigin = function(data, place, opts) {
  var x = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var y = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  ///////////////////////////
  if (opts.verbose == true) {
    console.log("OffsetOrigin");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["OffsetOrigin"](x, y);
  return place;
}

gU1D0pAR$ER.NotifySize = function(data, place, opts) {
  var width = gU1D0pAR$ER.getInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByInt32(place);
  var height = gU1D0pAR$ER.getInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByInt32(place);
  ///////////////////////////
  if (opts.verbose == true) {
    console.log("NotifySize");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["NotifySize"](width, height);
  return place;
}

gU1D0pAR$ER.DrawMusicSymbol = function(data, place, opts) {
  var x = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var y = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  var inSymbolID = gU1D0pAR$ER.getUnsignedInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByUnsignedInt32(place);
  ///////////////////////////
  if (opts.verbose == true) {
    console.log("DrawMusicSymbol");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["DrawMusicSymbol"](x, y, inSymbolID);
  return place;
}

gU1D0pAR$ER.DrawString = function(data, place, opts) {
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
  if (opts.verbose == true) {
    console.log("DrawString");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["DrawString"](x, y, s, inCharCount);
  return place;
}

gU1D0pAR$ER.SetFontColor = function(data, place, opts) {
  var alpha = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var red = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var green = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var blue = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  /////////////////////////////////////////
  if (opts.verbose == true) {
    console.log("SetFontColor");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["SetFontColor"](alpha, red, green, blue);
  return place;
}

gU1D0pAR$ER.GetFontColor = function(data, place, opts) {
  ///////////////////////////
  if (opts.verbose == true) {
    console.log("GetFontColor");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["GetFontColor"]();
  return place;
}

gU1D0pAR$ER.SetFontAlign = function(data, place, opts) {
  var inAlign = gU1D0pAR$ER.getUnsignedInt32(data, place);
  place = gU1D0pAR$ER.moveReadPositionByUnsignedInt32(place);
  /////////////////////////////////////////
  if (opts.verbose == true) {
    console.log("SetFontAlign");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["SetFontAlign"](inAlign);
  return place;
}

gU1D0pAR$ER.SelectPenColor = function(data, place, opts) {
  var alpha = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var red = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var green = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var blue = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  /////////////////////////////////////////
  if (opts.verbose == true) {
    console.log("SelectPenColor");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["SelectPenColor"](alpha, red, green, blue);
  return place;
}

gU1D0pAR$ER.PushPenColor = function(data, place, opts) {
  var alpha = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var red = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var green = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  var blue = gU1D0pAR$ER.getUnsignedChar(data, place);
  place = gU1D0pAR$ER.moveReadPositionByChar(place);
  /////////////////////////////////////////
  if (opts.verbose == true) {
    console.log("PushPenColor");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["PushPenColor"](alpha, red, green, blue);
  return place;
}

gU1D0pAR$ER.PopPenColor = function(data, place, opts) {
  /////////////////////////////////////////
  if (opts.verbose == true) {
    console.log("PopPenColor");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["PopPenColor"]();
  return place;
}

gU1D0pAR$ER.PushPenWidth = function(data, place, opts) {
  var width = gU1D0pAR$ER.getFloat(data, place);
  place = gU1D0pAR$ER.moveReadPositionByFloat(place);
  ///////////////////////////
  if (opts.verbose == true) {
    console.log("PushPenWidth");
  }
  //////////////////////////
  gU1D0pAR$ER.drawFunctions["PushPenWidth"](width);
  return place;
}

gU1D0pAR$ER.PopPenWidth = function(data, place, opts) {
  /////////////////////////////////////////
  if (opts.verbose == true) {
    console.log("PopPenWidth");
  }
  //////////////////////////
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

gU1D0pAR$ER.registerPushPen = function(fn) {
  gU1D0pAR$ER.drawFunctions["PushPen"] = fn;
}

gU1D0pAR$ER.registerPopPen = function(fn) {
  gU1D0pAR$ER.drawFunctions["PopPen"] = fn;
}

gU1D0pAR$ER.registerPushFillColor = function(fn) {
  gU1D0pAR$ER.drawFunctions["PushFillColor"] = fn;
}

gU1D0pAR$ER.registerPopFillColor = function(fn) {
  gU1D0pAR$ER.drawFunctions["PopFillColor"] = fn;
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

gU1D0pAR$ER.registerPushPenColor = function(fn) {
  gU1D0pAR$ER.drawFunctions["PushPenColor"] = fn
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
