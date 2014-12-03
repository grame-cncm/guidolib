/* creates guido canvas namespace */

var gU1D0cANVA$ = {}

gU1D0cANVA$.CHAR_MAX_AS_FLOAT = 255.0;
gU1D0cANVA$.GLOBAL_RESCALE_FACTOR = 1.0;
gU1D0cANVA$.GLOBAL_VERBOSE_FLAG = 0;
gU1D0cANVA$.GLOBAL_SCALE_TO_DIV_SIZE = 1;
gU1D0cANVA$.CURRENT_TRANSFORM_MATRIX = [1, 0, 0, 1, 0, 0];
gU1D0cANVA$.PEN_COLORS = [];
gU1D0cANVA$.FONT_COLOR = 'rgba(0, 0, 0, 255)';
gU1D0cANVA$.FILL_COLOR = 'rgba(255, 255, 255, 255)';
gU1D0cANVA$.FILL_COLORS = [];
gU1D0cANVA$.PEN_WIDTH = 1;
gU1D0cANVA$.PEN_WIDTHS = [];
gU1D0cANVA$.PEN = [];
gU1D0cANVA$.MUSIC_FONT = ['normal','12px','Guido2'];
gU1D0cANVA$.TEXT_FONT = ['normal','12px','Times'];
gU1D0cANVA$.CANVAS = 0;
gU1D0cANVA$.CONTEXT = 0;

gU1D0cANVA$.updateMatrices = function(context, a, b, c, d, e, f, g) {
  gU1D0cANVA$.CURRENT_TRANSFORM_MATRIX[0] = a;
  gU1D0cANVA$.CURRENT_TRANSFORM_MATRIX[1] = b;
  gU1D0cANVA$.CURRENT_TRANSFORM_MATRIX[2] = c;
  gU1D0cANVA$.CURRENT_TRANSFORM_MATRIX[3] = d;
  gU1D0cANVA$.CURRENT_TRANSFORM_MATRIX[4] = e;
  gU1D0cANVA$.CURRENT_TRANSFORM_MATRIX[5] = f;
  context.setTransform(a, b, c, d, e, f, g);
}

gU1D0cANVA$.matrixAt = function(idx) {
  return gU1D0cANVA$.CURRENT_TRANSFORM_MATRIX[idx];
}

gU1D0cANVA$.propertiesToFontProperties = function(properties) {
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

gU1D0cANVA$.makeFont = function(font) {
  return font[0]+' '+font[1]+' '+font[2];
}

gU1D0cANVA$.correctTransformMatrix = function(context) {
  context.save();
  context.scale(gU1D0cANVA$.GLOBAL_RESCALE_FACTOR, gU1D0cANVA$.GLOBAL_RESCALE_FACTOR);
}

gU1D0cANVA$.resetTransformMatrix = function(context) {
  context.restore();
}

gU1D0cANVA$.initGuidoCanvas = function() {
  gU1D0cANVA$.CANVAS = $('canvas')[0];
  gU1D0cANVA$.CONTEXT = gU1D0cANVA$.CANVAS.getContext('2d');
}

gU1D0cANVA$.BeginDraw = function() {
  gU1D0cANVA$.updateMatrices(gU1D0cANVA$.CONTEXT, 1, 0, 0, 1, 0, 0);
  gU1D0cANVA$.CONTEXT.clearRect(0, 0, gU1D0cANVA$.CANVAS.width, gU1D0cANVA$.CANVAS.height);
  
  gU1D0cANVA$.PEN_COLORS = [gU1D0cANVA$.FONT_COLOR];
  gU1D0cANVA$.FILL_COLORS = [gU1D0cANVA$.FILL_COLOR];
  gU1D0cANVA$.PEN_WIDTHS = [gU1D0cANVA$.PEN_WIDTH];
  gU1D0cANVA$.PEN = [{ "color" : gU1D0cANVA$.FONT_COLOR, "width" : gU1D0cANVA$.PEN_WIDTH}];
}

gU1D0cANVA$.EndDraw = function() {
  gU1D0cANVA$.PEN_WIDTHS.length = 0;
  gU1D0cANVA$.FILL_COLORS = 0;
  gU1D0cANVA$.PEN_WIDTHS = 0;
  gU1D0cANVA$.PEN = 0;
}

gU1D0cANVA$.Line = function(x1, y1, x2, y2) {
  gU1D0cANVA$.correctTransformMatrix(gU1D0cANVA$.CONTEXT);
  gU1D0cANVA$.CONTEXT.beginPath();
  gU1D0cANVA$.CONTEXT.moveTo(x1, y1);
  gU1D0cANVA$.CONTEXT.lineTo(x2, y2);
  gU1D0cANVA$.CONTEXT.stroke();
  gU1D0cANVA$.resetTransformMatrix(gU1D0cANVA$.CONTEXT);
}

gU1D0cANVA$.Polygon = function(xCoords, yCoords, count) {
  gU1D0cANVA$.correctTransformMatrix(gU1D0cANVA$.CONTEXT);
  gU1D0cANVA$.CONTEXT.beginPath();
  gU1D0cANVA$.CONTEXT.moveTo(xCoords[0], yCoords[0]);
  for (var i = 1; i < xCoords.length; i++) {
    gU1D0cANVA$.CONTEXT.lineTo(xCoords[i], yCoords[i]);
  }
  gU1D0cANVA$.CONTEXT.closePath();
  gU1D0cANVA$.CANVAS.fillStyle = gU1D0cANVA$.FILL_COLOR;
  gU1D0cANVA$.CONTEXT.fill();
  gU1D0cANVA$.resetTransformMatrix(gU1D0cANVA$.CONTEXT);
}

gU1D0cANVA$.Rectangle = function(left, top, right, bottom) {
  gU1D0cANVA$.correctTransformMatrix(gU1D0cANVA$.CONTEXT);
  gU1D0cANVA$.CONTEXT.beginPath();
  gU1D0cANVA$.CONTEXT.rect(left, top, right - left, bottom - top);
  gU1D0cANVA$.CANVAS.fillStyle = gU1D0cANVA$.FILL_COLOR;
  gU1D0cANVA$.CONTEXT.fill();
  gU1D0cANVA$.resetTransformMatrix(gU1D0cANVA$.CONTEXT);
}

gU1D0cANVA$.SetMusicFont = function(name, size, properties) {
  gU1D0cANVA$.MUSIC_FONT[0] = gU1D0cANVA$.propertiesToFontProperties(properties);
  gU1D0cANVA$.MUSIC_FONT[1] = size+"px";
  gU1D0cANVA$.MUSIC_FONT[2] = name;
}

gU1D0cANVA$.GetMusicFont = function() {
  ////////////////////
}

gU1D0cANVA$.SetTextFont = function(name, size, properties) {
  gU1D0cANVA$.TEXT_FONT[0] = gU1D0cANVA$.propertiesToFontProperties(properties);
  gU1D0cANVA$.TEXT_FONT[1] = size+"px";
  gU1D0cANVA$.TEXT_FONT[2] = name;
}

gU1D0cANVA$.GetTextFont = function() {
  ////////////////////
}

gU1D0cANVA$.PushPen = function(alpha, red, green, blue, width) {
  var strokeStyle = "rgba("+red+","+green+","+blue+","+(alpha/gU1D0cANVA$.CHAR_MAX_AS_FLOAT)+")"
  gU1D0cANVA$.CANVAS.strokeStyle = strokeStyle;
  gU1D0cANVA$.CONTEXT.lineWidth = width;
  gU1D0cANVA$.PEN.push({ "color" : strokeStyle, "width" : width});
}

gU1D0cANVA$.PopPen = function() {
  gU1D0cANVA$.PEN.pop();
  if (gU1D0cANVA$.PEN.length > 0) {
	var pen = gU1D0cANVA$.PEN[gU1D0cANVA$.PEN.length - 1];
	gU1D0cANVA$.CANVAS.strokeStyle = pen.color;
	gU1D0cANVA$.CONTEXT.lineWidth = pen.width;
  } else {
    console.log("Popping pen but the queue is empty");
  }
}

gU1D0cANVA$.PushFillColor = function(alpha, red, green, blue) {
  var fillStyle = "rgba("+red+","+green+","+blue+","+(alpha/gU1D0cANVA$.CHAR_MAX_AS_FLOAT)+")"
  gU1D0cANVA$.FILL_COLOR = fillStyle;
  gU1D0cANVA$.FILL_COLORS.push(fillStyle);
}

gU1D0cANVA$.PopFillColor = function() {
  gU1D0cANVA$.FILL_COLORS.pop();
  if (gU1D0cANVA$.FILL_COLORS.length > 0) {
	var fillStyle = gU1D0cANVA$.FILL_COLORS[gU1D0cANVA$.FILL_COLORS.length - 1];
	gU1D0cANVA$.FILL_COLOR = fillStyle;
  } else {
    console.log("Popping fillColor but the queue is empty");
  }
}

gU1D0cANVA$.SetScale = function(x, y) {
  gU1D0cANVA$.updateMatrices(gU1D0cANVA$.CONTEXT, x, 0, 0, y, gU1D0cANVA$.matrixAt(4), gU1D0cANVA$.matrixAt(5));
}

gU1D0cANVA$.SetOrigin = function(x, y) {
  x = x * gU1D0cANVA$.GLOBAL_RESCALE_FACTOR;
  y = y * gU1D0cANVA$.GLOBAL_RESCALE_FACTOR;
  gU1D0cANVA$.updateMatrices(gU1D0cANVA$.CONTEXT, gU1D0cANVA$.matrixAt(0), 0, 0, gU1D0cANVA$.matrixAt(3), x, y);
}

gU1D0cANVA$.OffsetOrigin = function(x, y) {
  x = x * gU1D0cANVA$.GLOBAL_RESCALE_FACTOR;
  y = y * gU1D0cANVA$.GLOBAL_RESCALE_FACTOR;
  gU1D0cANVA$.updateMatrices(gU1D0cANVA$.CONTEXT, gU1D0cANVA$.matrixAt(0), 0, 0, gU1D0cANVA$.matrixAt(3), gU1D0cANVA$.matrixAt(4) + x, gU1D0cANVA$.matrixAt(5) + y);
}

gU1D0cANVA$.NotifySize = function(width, height) {
  if (gU1D0cANVA$.GLOBAL_SCALE_TO_DIV_SIZE) {
    var w = $("#canvasContainer").width();
    gU1D0cANVA$.CANVAS.width = w;
    gU1D0cANVA$.CANVAS.height = height * w / width;
    gU1D0cANVA$.GLOBAL_RESCALE_FACTOR = w / width;
  }
}

gU1D0cANVA$.DrawMusicSymbol = function(x, y, inSymbolId) {
  gU1D0cANVA$.correctTransformMatrix(gU1D0cANVA$.CONTEXT);
  gU1D0cANVA$.CONTEXT.font = gU1D0cANVA$.makeFont(gU1D0cANVA$.MUSIC_FONT);
  gU1D0cANVA$.CONTEXT.fillText(String.fromCharCode(inSymbolId), x, y);
  gU1D0cANVA$.resetTransformMatrix(gU1D0cANVA$.CONTEXT);
}

gU1D0cANVA$.DrawString = function(x, y, s, inCharCount) {
  gU1D0cANVA$.correctTransformMatrix(gU1D0cANVA$.CONTEXT);
  // now that save has happened, make change
    
  gU1D0cANVA$.CONTEXT.font = gU1D0cANVA$.makeFont(gU1D0cANVA$.TEXT_FONT);
  gU1D0cANVA$.CONTEXT.fillStyle = gU1D0cANVA$.FONT_COLOR;
  gU1D0cANVA$.CONTEXT.fillText(s.substring(0, inCharCount + 1), x, y);
  gU1D0cANVA$.resetTransformMatrix(gU1D0cANVA$.CONTEXT);
}

gU1D0cANVA$.SetFontColor = function(alpha, red, green, blue) {
  gU1D0cANVA$.FONT_COLOR = "rgba("+red+","+green+","+blue+","+(alpha/gU1D0cANVA$.CHAR_MAX_AS_FLOAT)+")";
}

gU1D0cANVA$.GetFontColor = function() {
}

gU1D0cANVA$.SetFontAlign = function(inAlign) {
  var align = 'left';
  if (inAlign & 8) {
    align = 'center';
  }
  if (inAlign & 32) {
    align = 'right';
  }
  if (inAlign & 4) {
	gU1D0cANVA$.CONTEXT.textBaseline = 'hanging';
  } else {
	// Default value
	gU1D0cANVA$.CONTEXT.textBaseline = 'alphabetic';
  }
  gU1D0cANVA$.CONTEXT.textAlign = align;
}

gU1D0cANVA$.SelectPenColor = function(alpha, red, green, blue) {
  var strokeStyle = "rgba("+red+","+green+","+blue+","+(alpha/gU1D0cANVA$.CHAR_MAX_AS_FLOAT)+")"
  gU1D0cANVA$.PEN_COLORS.length = 0;
  gU1D0cANVA$.PEN_COLORS.push(strokeStyle);
  gU1D0cANVA$.CANVAS.strokeStyle = strokeStyle;
}

gU1D0cANVA$.PushPenColor = function(alpha, red, green, blue) {
  var strokeStyle = "rgba("+red+","+green+","+blue+","+(alpha/gU1D0cANVA$.CHAR_MAX_AS_FLOAT)+")"
  gU1D0cANVA$.PEN_COLORS.push(strokeStyle);
  gU1D0cANVA$.CANVAS.strokeStyle = strokeStyle;
}

gU1D0cANVA$.PopPenColor = function() {
  gU1D0cANVA$.PEN_COLORS.pop();
  if (gU1D0cANVA$.PEN_COLORS.length > 0) {
    gU1D0cANVA$.CANVAS.strokeStyle = gU1D0cANVA$.PEN_COLORS[gU1D0cANVA$.PEN_COLORS.length - 1];
  } else {
    console.log("Popping pen color but the queue is empty");
  }
}

gU1D0cANVA$.PushPenWidth = function(width) {
  gU1D0cANVA$.PEN_WIDTHS.push(width);
  gU1D0cANVA$.CONTEXT.lineWidth = width;
}

gU1D0cANVA$.PopPenWidth = function() {
  gU1D0cANVA$.PEN_WIDTHS.pop();
  if (gU1D0cANVA$.PEN_WIDTHS.length > 0) {
    gU1D0cANVA$.CANVAS.lineWidth = gU1D0cANVA$.PEN_WIDTHS[gU1D0cANVA$.PEN_WIDTHS.length - 1];
  } else {
    console.log("Popping pen width but the queue is empty");
  }
}

/* register draw functions */
gU1D0pAR$ER.registerBeginDraw(gU1D0cANVA$.BeginDraw);
gU1D0pAR$ER.registerEndDraw(gU1D0cANVA$.EndDraw);
gU1D0pAR$ER.registerLine(gU1D0cANVA$.Line);
gU1D0pAR$ER.registerPolygon(gU1D0cANVA$.Polygon);
gU1D0pAR$ER.registerRectangle(gU1D0cANVA$.Rectangle);
gU1D0pAR$ER.registerSetMusicFont(gU1D0cANVA$.SetMusicFont);
gU1D0pAR$ER.registerGetMusicFont(gU1D0cANVA$.GetMusicFont);
gU1D0pAR$ER.registerSetTextFont(gU1D0cANVA$.SetTextFont);
gU1D0pAR$ER.registerGetTextFont(gU1D0cANVA$.GetTextFont);
gU1D0pAR$ER.registerPushPen(gU1D0cANVA$.PushPen);
gU1D0pAR$ER.registerPopPen(gU1D0cANVA$.PopPen);
gU1D0pAR$ER.registerPushFillColor(gU1D0cANVA$.PushFillColor);
gU1D0pAR$ER.registerPopFillColor(gU1D0cANVA$.PopFillColor);
gU1D0pAR$ER.registerSetScale(gU1D0cANVA$.SetScale);
gU1D0pAR$ER.registerSetOrigin(gU1D0cANVA$.SetOrigin);
gU1D0pAR$ER.registerOffsetOrigin(gU1D0cANVA$.OffsetOrigin);
gU1D0pAR$ER.registerNotifySize(gU1D0cANVA$.NotifySize);
gU1D0pAR$ER.registerDrawMusicSymbol(gU1D0cANVA$.DrawMusicSymbol);
gU1D0pAR$ER.registerDrawString(gU1D0cANVA$.DrawString);
gU1D0pAR$ER.registerSetFontColor(gU1D0cANVA$.SetFontColor);
gU1D0pAR$ER.registerGetFontColor(gU1D0cANVA$.GetFontColor);
gU1D0pAR$ER.registerSetFontAlign(gU1D0cANVA$.SetFontAlign);
gU1D0pAR$ER.registerSelectPenColor(gU1D0cANVA$.GetFontColor);
gU1D0pAR$ER.registerPushPenColor(gU1D0cANVA$.PushPenColor);
gU1D0pAR$ER.registerPopPenColor(gU1D0cANVA$.PopPenColor);
gU1D0pAR$ER.registerPushPenWidth(gU1D0cANVA$.PushPenWidth);
gU1D0pAR$ER.registerPopPenWidth(gU1D0cANVA$.PopPenWidth);
