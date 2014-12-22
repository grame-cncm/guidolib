/* creates guido canvas namespace */

var gU1D0cANVA$ = {}

gU1D0cANVA$.CHAR_MAX_AS_FLOAT = 255.0;
gU1D0cANVA$.GLOBAL_RESCALE_FACTOR = 1.0;
gU1D0cANVA$.GLOBAL_VERBOSE_FLAG = 0;
gU1D0cANVA$.GLOBAL_SCALE_TO_DIV_SIZE = 1;
gU1D0cANVA$.CURRENT_TRANSFORM_MATRIX = [1, 0, 0, 1, 0, 0];
gU1D0cANVA$.PEN_COLORS = [];
gU1D0cANVA$.FONT_COLOR = 'rgba(0, 0, 0, 255)';
gU1D0cANVA$.FILL_COLOR = 'rgba(0, 0, 0, 255)';
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
