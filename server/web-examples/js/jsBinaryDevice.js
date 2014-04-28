/*
  If you want to use dynamic resizing, make sure that the div containing the
  canvas has the id "canvasContainer".
*/

// abstraction such that javascript-specific code is higher, general
// code is lower
var GUIDO_BEGIN_DRAW_CODE = 0;
var GUIDO_END_DRAW_CODE = 1;
var GUIDO_LINE_CODE = 5;
var GUIDO_POLYGON_CODE = 9;
var GUIDO_RECTANGLE_CODE = 10;
var GUIDO_SET_MUSIC_FONT_CODE = 11;
var GUIDO_GET_MUSIC_FONT_CODE = 12;
var GUIDO_SET_TEXT_FONT_CODE = 13;
var GUIDO_GET_TEXT_FONT_CODE = 14;
var GUIDO_SET_SCALE_CODE = 27;
var GUIDO_SET_ORIGIN_CODE = 28;
var GUIDO_OFFSET_ORIGIN_CODE = 29;
var GUIDO_NOTIFY_SIZE_CODE = 36;
var GUIDO_DRAW_MUSIC_SYMBOL_CODE = 39;
var GUIDO_DRAW_STRING_CODE = 40;
var GUIDO_SET_FONT_COLOR_CODE = 41;
var GUIDO_GET_FONT_COLOR_CODE = 42;
var GUIDO_SET_FONT_ALIGN_CODE = 45;
var GUIDO_SELECT_PEN_COLOR_CODE = 49;
var GUIDO_POP_PEN_COLOR_CODE = 52;
var GUIDO_PUSH_PEN_WIDTH_CODE = 53;
var GUIDO_POP_PEN_WIDTH_CODE = 54;

function getUnsignedChar(data, place) {
  var head = new Uint8Array(data, place, Uint8Array.BYTES_PER_ELEMENT);
  return head[0];
}
function getUnsignedInt32(data, place) {
  var head = data.slice(place, place + Uint32Array.BYTES_PER_ELEMENT);
  var intar = new Uint32Array(head, 0, 1);
  return intar[0];
}
function getInt32(data, place) {
  var head = data.slice(place, place + Int32Array.BYTES_PER_ELEMENT);
  var intar = new Int32Array(head, 0, 1);
  return intar[0];
}
function getFloat(data, place) {
  var head = data.slice(place, place + Float32Array.BYTES_PER_ELEMENT);
  var floatar = new Float32Array(head, 0, 1);
  return floatar[0];
}
function getString(data, place, n) {
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
function moveReadPositionByChar(place) {
  return place + Uint8Array.BYTES_PER_ELEMENT;
}
function moveReadPositionByUnsignedInt32(place) {
  return place + Uint32Array.BYTES_PER_ELEMENT;
}
function moveReadPositionByInt32(place) {
  return place + Int32Array.BYTES_PER_ELEMENT;
}
function moveReadPositionByFloat(place) {
  return place + Float32Array.BYTES_PER_ELEMENT;
}
function moveReadPositionByString(place, str) {
  return place + str.length + 1; // 1 for the escape char
}

var gU1D0 = {}
gU1D0.CHAR_MAX_AS_FLOAT = 255.0;
gU1D0.GLOBAL_RESCALE_FACTOR = 1.0;
gU1D0.GLOBAL_VERBOSE_FLAG = 0;
gU1D0.GLOBAL_SCALE_TO_DIV_SIZE = 1;
gU1D0.CURRENT_TRANSFORM_MATRIX = [1, 0, 0, 1, 0, 0];
gU1D0.PEN_COLORS = [];
gU1D0.FONT_COLOR = 'rgba(0, 0, 0, 255)';
gU1D0.PEN_WIDTHS = [];
gU1D0.MUSIC_FONT = ['normal','12px','Guido2'];
gU1D0.TEXT_FONT = ['normal','12px','Times'];
gU1D0.CANVAS = 0;
gU1D0.CONTEXT = 0;
function updateMatrices(context, a, b, c, d, e, f, g) {
  gU1D0.CURRENT_TRANSFORM_MATRIX[0] = a;
  gU1D0.CURRENT_TRANSFORM_MATRIX[1] = b;
  gU1D0.CURRENT_TRANSFORM_MATRIX[2] = c;
  gU1D0.CURRENT_TRANSFORM_MATRIX[3] = d;
  gU1D0.CURRENT_TRANSFORM_MATRIX[4] = e;
  gU1D0.CURRENT_TRANSFORM_MATRIX[5] = f;
  context.setTransform(a, b, c, d, e, f, g);
}
function matrixAt(idx) {
  return gU1D0.CURRENT_TRANSFORM_MATRIX[idx];
}
function propertiesToFontProperties(properties) {
  if (properties == 0)
    return 'normal';
  if (properties == 1)
    return 'bold';
  if (properties == 2)
    return 'italic';
  if (properties == 3) // hack
    return 'normal';
  return 'normal';
}
function makeFont(font) {
  return font[0]+' '+font[1]+' '+font[2];
}
function correctTransformMatrix(context) {
  context.save();
  context.scale(gU1D0.GLOBAL_RESCALE_FACTOR, gU1D0.GLOBAL_RESCALE_FACTOR);
}
function resetTransformMatrix(context) {
  context.restore();
}
function verbose(a, b, c, d, e, f) {
  if (gU1D0.GLOBAL_VERBOSE_FLAG) {
    console.log(a, b, c, d, e, f);
  }
}
function initGuidoCanvas() {
  gU1D0.CANVAS = $('canvas')[0];
  gU1D0.CONTEXT = gU1D0.CANVAS.getContext('2d');
}

function _BeginDraw() {
  updateMatrices(gU1D0.CONTEXT, 1, 0, 0, 1, 0, 0);
  gU1D0.CONTEXT.clearRect(0, 0, gU1D0.CANVAS.width, gU1D0.CANVAS.height);
}

function _EndDraw() {
  gU1D0.PEN_COLORS.length = 0;
  gU1D0.PEN_WIDTHS.length = 0;
}

function _Line(x1, y1, x2, y2) {
  correctTransformMatrix(gU1D0.CONTEXT);
  gU1D0.CONTEXT.beginPath();
  gU1D0.CONTEXT.moveTo(x1, y1);
  gU1D0.CONTEXT.lineTo(x2, y2);
  gU1D0.CONTEXT.stroke();
  resetTransformMatrix(gU1D0.CONTEXT);
}

function _Polygon(xCoords, yCoords, count) {
  correctTransformMatrix(gU1D0.CONTEXT);
  gU1D0.CONTEXT.beginPath();
  gU1D0.CONTEXT.moveTo(xCoords[0], yCoords[0]);
  for (var i = 1; i < xCoords.length; i++) {
    gU1D0.CONTEXT.lineTo(xCoords[i], yCoords[i]);
  }
  gU1D0.CONTEXT.closePath();
  gU1D0.CONTEXT.fill();
  resetTransformMatrix(gU1D0.CONTEXT);
}

function _Rectangle(left, top, right, bottom) {
  correctTransformMatrix(gU1D0.CONTEXT);
  gU1D0.CONTEXT.beginPath();
  gU1D0.CONTEXT.rect(left, top, right - left, bottom - top);
  gU1D0.CONTEXT.fill();
  resetTransformMatrix(gU1D0.CONTEXT);
}

function _SetMusicFont(name, size, properties) {
  gU1D0.MUSIC_FONT[0] = propertiesToFontProperties(properties);
  gU1D0.MUSIC_FONT[1] = size+"px";
  gU1D0.MUSIC_FONT[2] = name;
}

function _GetMusicFont() {
  ////////////////////
}

function _SetTextFont(name, size, properties) {
  gU1D0.TEXT_FONT[0] = propertiesToFontProperties(properties);
  gU1D0.TEXT_FONT[1] = size+"px";
  gU1D0.TEXT_FONT[2] = name;
}

function _GetTextFont() {
  ////////////////////
}

function _SetScale(x, y) {
  updateMatrices(gU1D0.CONTEXT, x, 0, 0, y, matrixAt(4), matrixAt(5));
}

function _SetOrigin(x, y) {
  verbose("SetOrigin", x, y);
  x = x * gU1D0.GLOBAL_RESCALE_FACTOR;
  y = y * gU1D0.GLOBAL_RESCALE_FACTOR;
  updateMatrices(gU1D0.CONTEXT, matrixAt(0), 0, 0, matrixAt(3), x, y);
}

function _OffsetOrigin(x, y) {
  verbose("OffsetOrigin", x, y);
  x = x * gU1D0.GLOBAL_RESCALE_FACTOR;
  y = y * gU1D0.GLOBAL_RESCALE_FACTOR;
  updateMatrices(gU1D0.CONTEXT, matrixAt(0), 0, 0, matrixAt(3), matrixAt(4) + x, matrixAt(5) + y);
}

function _NotifySize(width, height) {
  if (gU1D0.GLOBAL_SCALE_TO_DIV_SIZE) {
    var w = $("#canvasContainer").width();
    gU1D0.CANVAS.width = w;
    gU1D0.CANVAS.height = height * w / width;
    gU1D0.GLOBAL_RESCALE_FACTOR = w / width;
  }
}

function _DrawMusicSymbol(x, y, inSymbolId) {
  correctTransformMatrix(gU1D0.CONTEXT);
  gU1D0.CONTEXT.font = makeFont(gU1D0.MUSIC_FONT);
  gU1D0.CONTEXT.fillText(String.fromCharCode(inSymbolId), x, y);
  resetTransformMatrix(gU1D0.CONTEXT);
}

function _DrawString(x, y, s, inCharCount) {
  correctTransformMatrix(gU1D0.CONTEXT);
  // now that save has happened, make change
  gU1D0.CONTEXT.font = makeFont(gU1D0.TEXT_FONT);
  gU1D0.CONTEXT.fillStyle = gU1D0.FONT_COLOR;
  gU1D0.CONTEXT.fillText(s.substring(0, inCharCount + 1), x, y);
  resetTransformMatrix(gU1D0.CONTEXT);
}

function _SetFontColor(alpha, red, green, blue) {
  gU1D0.FONT_COLOR = "rgb("+red+","+green+","+blue+","+(alpha/gU1D0.CHAR_MAX_AS_FLOAT)+")"
}


function _GetFontColor() {
}

function _SetFontAlign(inAlign) {
  var align = 'left';
  if (inAlign & 8) {
    align = 'center';
  }
  if (inAlign & 32) {
    align = 'right';
  }
  gU1D0.CONTEXT.textAlign = align;
}

function _SelectPenColor(alpha, red, green, blue) {
  var strokeStyle = "rgb("+red+","+green+","+blue+","+(alpha/gU1D0.CHAR_MAX_AS_FLOAT)+")"
  gU1D0.PEN_COLORS.length = 0;
  gU1D0.PEN_COLORS.push(strokeStyle);
  gU1D0.CANVAS.strokeStyle = strokeStyle;
}

function _PopPenColor() {
  gU1D0.PEN_COLORS.pop();
  if (gU1D0.PEN_COLORS.length > 0) {
    gU1D0.CANVAS.strokeStyle = PEN_COLORS[PEN_COLORS.length - 1];
  } else {
    console.log("Popping pen color but the queue is empty");
  }
}

function _PushPenWidth(width) {
  gU1D0.PEN_WIDTHS.push(width);
  gU1D0.CONTEXT.lineWidth = width;
}

function _PopPenWidth() {
  gU1D0.PEN_WIDTHS.pop();
  if (gU1D0.PEN_WIDTHS.length > 0) {
    gU1D0.CANVAS.lineWidth = PEN_WIDTHS[PEN_WIDTHS.length - 1];
  } else {
    console.log("Popping pen width but the queue is empty");
  }
}

function parseGuidoBinary(data, place) {
  while ((place >= 0) && (place < data.byteLength)) {
    var head = getUnsignedChar(data, place);
    place = moveReadPositionByChar(place);
    if (head == GUIDO_BEGIN_DRAW_CODE) {
      place = BeginDraw(data, place);
    } else if (head == GUIDO_END_DRAW_CODE) {
      place = EndDraw(data, place);
    } else if (head == GUIDO_LINE_CODE) {
      place = Line(data, place);
    } else if (head == GUIDO_POLYGON_CODE) {
      place = Polygon(data, place);
    } else if (head == GUIDO_RECTANGLE_CODE) {
      place = Rectangle(data, place);
    } else if (head == GUIDO_SET_MUSIC_FONT_CODE) {
      place = SetMusicFont(data, place);
    } else if (head == GUIDO_GET_MUSIC_FONT_CODE) {
      place = GetMusicFont(data, place);
    } else if (head == GUIDO_SET_TEXT_FONT_CODE) {
      place = SetTextFont(data, place);
    } else if (head == GUIDO_GET_TEXT_FONT_CODE) {
      place = GetTextFont(data, place);
    } else if (head == GUIDO_SET_SCALE_CODE) {
      place = SetScale(data, place);
    } else if (head == GUIDO_SET_ORIGIN_CODE) {
      place = SetOrigin(data, place);
    } else if (head == GUIDO_OFFSET_ORIGIN_CODE) {
      place = OffsetOrigin(data, place);
    } else if (head == GUIDO_NOTIFY_SIZE_CODE) {
      place = NotifySize(data, place);
    } else if (head == GUIDO_DRAW_MUSIC_SYMBOL_CODE) {
      place = DrawMusicSymbol(data, place);
    } else if (head == GUIDO_DRAW_STRING_CODE) {
      place = DrawString(data, place);
    } else if (head == GUIDO_SET_FONT_COLOR_CODE) {
      place = SetFontColor(data, place);
    } else if (head == GUIDO_GET_FONT_COLOR_CODE) {
      place = GetFontColor(data, place);
    } else if (head == GUIDO_SET_FONT_ALIGN_CODE) {
      place = SetFontAlign(data, place);
    } else if (head == GUIDO_SELECT_PEN_COLOR_CODE) {
      place = SelectPenColor(data, place);
    } else if (head == GUIDO_POP_PEN_COLOR_CODE) {
      place = PopPenColor(data, place);
    } else if (head == GUIDO_PUSH_PEN_WIDTH_CODE) {
      place = PushPenWidth(data, place);
    } else if (head == GUIDO_POP_PEN_WIDTH_CODE) {
      place = PopPenWidth(data, place);
    } else {
      // panic
      console.log("don't know ", head);
      place = -1;
    }
  }
  console.log("finished drawing");
}

function BeginDraw(data, place) {
  //////////////////////////
  _BeginDraw();
  return place;
}

function EndDraw(data, place) {
  ///////////////////////
  _EndDraw();
  return place;
}

function Line(data, place) {
  var x1 = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  var y1 = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  var x2 = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  var y2 = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  ///////////////////////////
  _Line(x1, y1, x2, y2);
  return place;
}

function Polygon(data, place) {
  var count = getInt32(data, place);
  place = moveReadPositionByInt32(place);
  xCoords = [];
  yCoords = [];
  for (var i = 0; i < count; i++) {
    var x = getFloat(data, place);
    place = moveReadPositionByFloat(place);
    xCoords.push(x);
  }
  for (var i = 0; i < count; i++) {
    var y = getFloat(data, place);
    place = moveReadPositionByFloat(place);
    yCoords.push(y);
  }
  ///////////////////////////
  _Polygon(xCoords, yCoords, count);
  return place;
}

function Rectangle(data, place) {
  var left = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  var top = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  var right = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  var bottom = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  ///////////////////////////
  _Rectangle(left, top, right, bottom);
  return place;
}

function SetMusicFont(data, place) {
  var name = getString(data, place);
  place = moveReadPositionByString(place, name);
  var size = getInt32(data, place);
  place = moveReadPositionByInt32(place);
  var properties = getInt32(data, place);
  place = moveReadPositionByInt32(place);
  ///////////////////////////
  _SetMusicFont(name, size, properties);
  return place;
}

function GetMusicFont(data, place) {
  ///////////////////////////
  _GetMusicFont();
  return place;
}

function SetTextFont(data, place) {
  var name = getString(data, place);
  place = moveReadPositionByString(place, name);
  var size = getInt32(data, place);
  place = moveReadPositionByInt32(place);
  var properties = getInt32(data, place);
  place = moveReadPositionByInt32(place);
  ///////////////////////////
  _SetTextFont(name, size, properties);
  return place;
}

function GetTextFont(data, place) {
  ///////////////////////////
  _GetTextFont();
  return place;
}

function SetScale(data, place) {
  var x = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  var y = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  ///////////////////////////
  _SetScale(x, y);
  return place;
}

function SetOrigin(data, place) {
  var x = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  var y = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  ///////////////////////////
  _SetOrigin(x, y);
  return place;
}

function OffsetOrigin(data, place) {
  var x = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  var y = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  ///////////////////////////
  _OffsetOrigin(x, y);
  return place;
}

function NotifySize(data, place) {
  var width = getInt32(data, place);
  place = moveReadPositionByInt32(place);
  var height = getInt32(data, place);
  place = moveReadPositionByInt32(place);
  ///////////////////////////
  _NotifySize(width, height);
  return place;
}

function DrawMusicSymbol(data, place) {
  var x = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  var y = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  var inSymbolID = getUnsignedInt32(data, place);
  place = moveReadPositionByUnsignedInt32(place);
  ///////////////////////////
  _DrawMusicSymbol(x, y, inSymbolID);
  return place;
}

function DrawString(data, place) {
  var x = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  var y = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  var inCharCount = getInt32(data, place);
  place = moveReadPositionByInt32(place);
  var s = getString(data, place, inCharCount);
  place = moveReadPositionByString(place, s);
  // because not null terminated...
  place = place - 1;
  ///////////////////////////
  _DrawString(x, y, s, inCharCount);
  return place;
}

function SetFontColor(data, place) {
  var alpha = getUnsignedChar(data, place);
  place = moveReadPositionByChar(place);
  var red = getUnsignedChar(data, place);
  place = moveReadPositionByChar(place);
  var green = getUnsignedChar(data, place);
  place = moveReadPositionByChar(place);
  var blue = getUnsignedChar(data, place);
  place = moveReadPositionByChar(place);
  /////////////////////////////////////////
  _SetFontColor(alpha, red, green, blue);
  return place;
}

function GetFontColor(data, place) {
  ///////////////////////////
  _GetFontColor();
  return place;
}

function SetFontAlign(data, place) {
  var inAlign = getUnsignedInt32(data, place);
  place = moveReadPositionByUnsignedInt32(place);
  /////////////////////////////////////////
  _SetFontAlign(inAlign);
  return place;
}

function SelectPenColor(data, place) {
  var alpha = getUnsignedChar(data, place);
  place = moveReadPositionByChar(place);
  var red = getUnsignedChar(data, place);
  place = moveReadPositionByChar(place);
  var green = getUnsignedChar(data, place);
  place = moveReadPositionByChar(place);
  var blue = getUnsignedChar(data, place);
  place = moveReadPositionByChar(place);
  /////////////////////////////////////////
  _SelectPenColor(alpha, red, green, blue);
  return place;
}

function PopPenColor(data, place) {
  /////////////////////////////////////////
  _PopPenColor();
  return place;
}

function PushPenWidth(data, place) {
  var width = getFloat(data, place);
  place = moveReadPositionByFloat(place);
  ///////////////////////////
  _PushPenWidth(width);
  return place;
}

function PopPenWidth(data, place) {
  /////////////////////////////////////////
  _PopPenWidth();
  return place;
}

