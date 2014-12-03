/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "canvasdevice.h"

CanvasDevice::CanvasDevice(CanvasSystem* system):fSystem (system)
{
}

CanvasDevice::~CanvasDevice()
{
}

// - Drawing services ------------------------------------------------
// 0
bool CanvasDevice::BeginDraw()
{
	EM_ASM(
		gU1D0cANVA$.updateMatrices(gU1D0cANVA$.CONTEXT, 1, 0, 0, 1, 0, 0);
		gU1D0cANVA$.CONTEXT.clearRect(0, 0, gU1D0cANVA$.CANVAS.width, gU1D0cANVA$.CANVAS.height);

		gU1D0cANVA$.PEN_COLORS = [gU1D0cANVA$.FONT_COLOR];
		gU1D0cANVA$.FILL_COLORS = [gU1D0cANVA$.FILL_COLOR];
		gU1D0cANVA$.PEN_WIDTHS = [gU1D0cANVA$.PEN_WIDTH];
		gU1D0cANVA$.PEN = [{ "color" : gU1D0cANVA$.FONT_COLOR, "width" : gU1D0cANVA$.PEN_WIDTH}];
	);
		return true;
}

// 1
void CanvasDevice::EndDraw()
{
	EM_ASM(
		gU1D0cANVA$.PEN_WIDTHS.length = 0;
		gU1D0cANVA$.FILL_COLORS = 0;
		gU1D0cANVA$.PEN_WIDTHS = 0;
		gU1D0cANVA$.PEN = 0;
	);
}
// 2
void CanvasDevice::InvalidateRect(float left, float top, float right, float bottom)
{
}

// - Standard graphic primitives -------------------------
// 3
void CanvasDevice::MoveTo(float x, float y)
{
}
// 4
void CanvasDevice::LineTo(float x, float y)
{
}
// 5
void CanvasDevice::Line(float x1, float y1, float x2, float y2)
{
	EM_ASM_ARGS({
		gU1D0cANVA$.correctTransformMatrix(gU1D0cANVA$.CONTEXT);
		gU1D0cANVA$.CONTEXT.beginPath();
		gU1D0cANVA$.CONTEXT.moveTo($0, $1);
		gU1D0cANVA$.CONTEXT.lineTo($2, $3);
		gU1D0cANVA$.CONTEXT.stroke();
		gU1D0cANVA$.resetTransformMatrix(gU1D0cANVA$.CONTEXT);
	}, x1, y1, x2, y2);
}
// 6
void CanvasDevice::Frame(float left, float top, float right, float bottom)
{
}
// 7
void CanvasDevice::Arc(float left,   float top, float right,  float bottom,
						float startX, float startY, float endX,   float endY)
{
}

// - Filled surfaces --------------------------------------
// 8
void CanvasDevice::Triangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
}
// 9
void CanvasDevice::Polygon(const float * xCoords, const float * yCoords, int count)
{
	EM_ASM_ARGS({
		gU1D0cANVA$.correctTransformMatrix(gU1D0cANVA$.CONTEXT);
		gU1D0cANVA$.CONTEXT.beginPath();
		gU1D0cANVA$.CONTEXT.moveTo($0, $1);
	}, xCoords[0], yCoords[0]);

	for (int i = 1; i < count; i++) {
		EM_ASM_ARGS({
			gU1D0cANVA$.CONTEXT.lineTo($0, $1);
		}, xCoords[i], yCoords[i]);
	}
	EM_ASM(
		gU1D0cANVA$.CONTEXT.closePath();
		gU1D0cANVA$.CANVAS.fillStyle = gU1D0cANVA$.FILL_COLOR;
		gU1D0cANVA$.CONTEXT.fill();
		gU1D0cANVA$.resetTransformMatrix(gU1D0cANVA$.CONTEXT);
	);
}
// 10
void CanvasDevice::Rectangle(float left,  float top, float right, float bottom)
{
	EM_ASM_ARGS({
		gU1D0cANVA$.correctTransformMatrix(gU1D0cANVA$.CONTEXT);
		gU1D0cANVA$.CONTEXT.beginPath();
		gU1D0cANVA$.CONTEXT.rect($0, $1, $2 - $0, $3 - $1);
		gU1D0cANVA$.CANVAS.fillStyle = gU1D0cANVA$.FILL_COLOR;
		gU1D0cANVA$.CONTEXT.fill();
		gU1D0cANVA$.resetTransformMatrix(gU1D0cANVA$.CONTEXT);
	}, left, top, right, bottom);
}

// - Font services ---------------------------------------------------
// 11
void CanvasDevice::SetMusicFont(const VGFont * font)
{
	EM_ASM_ARGS({
		gU1D0cANVA$.MUSIC_FONT[0] = gU1D0cANVA$.propertiesToFontProperties($2);
		gU1D0cANVA$.MUSIC_FONT[1] = $1+"px";
		gU1D0cANVA$.MUSIC_FONT[2] = Pointer_stringify($0);
	}, font->GetName(), font->GetSize(), font->GetProperties());
}
// 12
const VGFont * CanvasDevice::GetMusicFont() const
{
	return 0;
}
// 13
void CanvasDevice::SetTextFont(const VGFont * font)
{
	EM_ASM_ARGS({
		gU1D0cANVA$.TEXT_FONT[0] = gU1D0cANVA$.propertiesToFontProperties($2);
		gU1D0cANVA$.TEXT_FONT[1] = $1+"px";
		gU1D0cANVA$.TEXT_FONT[2] = Pointer_stringify($0);
	}, font->GetName(), font->GetSize(), font->GetProperties());
}
// 14
const VGFont * CanvasDevice::GetTextFont() const
{
	return 0;
}

// - Pen & brush services --------------------------------------------
// 15
void CanvasDevice::SelectPen(const VGColor & inColor, float witdh)
{
}
// 16
void CanvasDevice::SelectFillColor(const VGColor & c)
{
}
// 17
void CanvasDevice::PushPen(const VGColor & inColor, float inWidth)
{
	EM_ASM_ARGS({
		var strokeStyle = "rgba("+$0+","+$1+","+$2+","+($3/gU1D0cANVA$.CHAR_MAX_AS_FLOAT)+")";
		gU1D0cANVA$.CANVAS.strokeStyle = strokeStyle;
		gU1D0cANVA$.CONTEXT.lineWidth = $4;
		gU1D0cANVA$.PEN.push({ "color" : strokeStyle, "width" : $4});
	}, inColor.mRed, inColor.mGreen, inColor.mBlue, inColor.mAlpha, inWidth);
}
// 18
void CanvasDevice::PopPen()
{
	EM_ASM(
		gU1D0cANVA$.PEN.pop();
		if (gU1D0cANVA$.PEN.length > 0) {
			var pen = gU1D0cANVA$.PEN[gU1D0cANVA$.PEN.length - 1];
			gU1D0cANVA$.CANVAS.strokeStyle = pen.color;
			gU1D0cANVA$.CONTEXT.lineWidth = pen.width;
		} else {
			console.log("Popping pen but the queue is empty");
		}
	);
}
// 19
void CanvasDevice::PushFillColor(const VGColor & inColor)
{
	EM_ASM_ARGS({
		var fillStyle = "rgba("+$0+","+$1+","+$2+","+($3/gU1D0cANVA$.CHAR_MAX_AS_FLOAT)+")";
		gU1D0cANVA$.FILL_COLOR = fillStyle;
		gU1D0cANVA$.FILL_COLORS.push(fillStyle);
	}, inColor.mRed, inColor.mGreen, inColor.mBlue, inColor.mAlpha);
}
// 20
void CanvasDevice::PopFillColor()
{
	EM_ASM(
		gU1D0cANVA$.FILL_COLORS.pop();
		if (gU1D0cANVA$.FILL_COLORS.length > 0) {
			var fillStyle = gU1D0cANVA$.FILL_COLORS[gU1D0cANVA$.FILL_COLORS.length - 1];
			gU1D0cANVA$.FILL_COLOR = fillStyle;
		} else {
			console.log("Popping fillColor but the queue is empty");
		}
	);
}
// 21
void CanvasDevice::SetRasterOpMode(VRasterOpMode ROpMode)
{
	fOpMode = ROpMode;
}
// 22
VGDevice::VRasterOpMode CanvasDevice::GetRasterOpMode() const
{
	return fOpMode;
}


// - Bitmap services (bit-block copy methods) --------------------------
// 23
bool CanvasDevice::CopyPixels(VGDevice* pSrcDC, float alpha)
{
	return true;
}
// 24
bool CanvasDevice::CopyPixels(int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc,
							   int nSrcWidth, int nSrcHeight, float alpha)
{
	return true;
}
// 25
bool CanvasDevice::CopyPixels(int xDest, int yDest, int dstWidth, int dstHeight,
							   VGDevice* pSrcDC, float alpha)
{
	return true;
}
// 26
bool CanvasDevice::CopyPixels(int xDest, int yDest, int dstWidth, int dstHeight,
							   VGDevice* pSrcDC, int xSrc, int ySrc,
							   int nSrcWidth, int nSrcHeight, float alpha)
{
	return true;
}

// - Coordinate services ------------------------------------------------
// 27
void CanvasDevice::SetScale(float x, float y)
{
	EM_ASM_ARGS({
	gU1D0cANVA$.updateMatrices(gU1D0cANVA$.CONTEXT, $0, 0, 0, $1, gU1D0cANVA$.matrixAt(4), gU1D0cANVA$.matrixAt(5));
	}, x, y);
}
// 28
void CanvasDevice::SetOrigin(float x, float y)
{
	EM_ASM_ARGS({
		x = $0 * gU1D0cANVA$.GLOBAL_RESCALE_FACTOR;
		y = $1 * gU1D0cANVA$.GLOBAL_RESCALE_FACTOR;
		gU1D0cANVA$.updateMatrices(gU1D0cANVA$.CONTEXT, gU1D0cANVA$.matrixAt(0), 0, 0, gU1D0cANVA$.matrixAt(3), x, y);
	}, x, y);
}
//29
void CanvasDevice::OffsetOrigin(float x, float y)
{
	EM_ASM_ARGS({
		x = $0 * gU1D0cANVA$.GLOBAL_RESCALE_FACTOR;
		y = $1 * gU1D0cANVA$.GLOBAL_RESCALE_FACTOR;
		gU1D0cANVA$.updateMatrices(gU1D0cANVA$.CONTEXT, gU1D0cANVA$.matrixAt(0), 0, 0, gU1D0cANVA$.matrixAt(3), gU1D0cANVA$.matrixAt(4) + x, gU1D0cANVA$.matrixAt(5) + y);
	}, x, y);
}
// 30
void CanvasDevice::LogicalToDevice(float * x, float * y) const
{
}
// 31
void CanvasDevice::DeviceToLogical(float * x, float * y) const
{
}

// 32
float CanvasDevice::GetXScale() const
{
	return 0;
}
// 33
float CanvasDevice::GetYScale() const
{
	return 0;
}
// 34
float CanvasDevice::GetXOrigin() const
{
	return 0;
}
// 35
float CanvasDevice::GetYOrigin() const
{
	return 0;
}

// 36
void CanvasDevice::NotifySize(int inWidth, int inHeight)
{
	EM_ASM_ARGS({
		if (gU1D0cANVA$.GLOBAL_SCALE_TO_DIV_SIZE) {
			var w = $("#canvasContainer").width();
			gU1D0cANVA$.CANVAS.width = w;
			gU1D0cANVA$.CANVAS.height = $1 * w / $0;
			gU1D0cANVA$.GLOBAL_RESCALE_FACTOR = w / $0;
		}
	}, inWidth, inHeight);
}
// 37
int CanvasDevice::GetWidth() const
{
	return 0;
}
// 38
int CanvasDevice::GetHeight() const
{
	return 0;
}


// - Text and music symbols services -------------------------------------
// 39
void CanvasDevice::DrawMusicSymbol(float x, float y, unsigned int inSymbolID)
{
	EM_ASM_ARGS({
		gU1D0cANVA$.correctTransformMatrix(gU1D0cANVA$.CONTEXT);
		gU1D0cANVA$.CONTEXT.font = gU1D0cANVA$.makeFont(gU1D0cANVA$.MUSIC_FONT);
		gU1D0cANVA$.CONTEXT.fillText(String.fromCharCode($0), $1, $2);
		gU1D0cANVA$.resetTransformMatrix(gU1D0cANVA$.CONTEXT);
	}, inSymbolID, x, y);
}
// 40
void CanvasDevice::DrawString(float x, float y, const char * s, int inCharCount)
{
	EM_ASM_ARGS({
		gU1D0cANVA$.correctTransformMatrix(gU1D0cANVA$.CONTEXT);
		// now that save has happened, make change
		gU1D0cANVA$.CONTEXT.font = gU1D0cANVA$.makeFont(gU1D0cANVA$.TEXT_FONT);
		gU1D0cANVA$.CONTEXT.fillStyle = gU1D0cANVA$.FONT_COLOR;
		s = Pointer_stringify($2);
		gU1D0cANVA$.CONTEXT.fillText(s.substring(0, $3 + 1), $0, $1);
		gU1D0cANVA$.resetTransformMatrix(gU1D0cANVA$.CONTEXT);
	}, x, y, s, inCharCount);
}

// 41
void CanvasDevice::SetFontColor(const VGColor & inColor)
{
	EM_ASM_ARGS({
		gU1D0cANVA$.FONT_COLOR = "rgba("+$0+","+$1+","+$2+","+($3/gU1D0cANVA$.CHAR_MAX_AS_FLOAT)+")";
	}, inColor.mRed, inColor.mGreen, inColor.mBlue, inColor.mAlpha);
}
// 42
VGColor CanvasDevice::GetFontColor() const
{
	return 0;
}
// 43
void CanvasDevice::SetFontBackgroundColor(const VGColor & inColor)
{
}
// 44
VGColor CanvasDevice::GetFontBackgroundColor() const
{
	return 0;
}
// 45
void CanvasDevice::SetFontAlign(unsigned int inAlign)
{
	this->inAlign = inAlign;

	EM_ASM_ARGS({
		var align = 'left';
		if ($0 & 8) {
			align = 'center';
		}
		if ($0 & 32) {
			align = 'right';
		}
		if ($0 & 4) {
			gU1D0cANVA$.CONTEXT.textBaseline = 'hanging';
		} else {
			// Default value
			gU1D0cANVA$.CONTEXT.textBaseline = 'alphabetic';
		}
		gU1D0cANVA$.CONTEXT.textAlign = align;
	}, inAlign);
}
// 46
unsigned int CanvasDevice::GetFontAlign() const
{
	return inAlign;
}

// - Printer informations services ----------------------------------------
// 47
void CanvasDevice::SetDPITag(float inDPI)
{
}
// 48
float CanvasDevice::GetDPITag() const
{
	return 0;
}

// - VGDevice extension --------------------------------------------
// 49
void CanvasDevice::SelectPenColor(const VGColor & inColor)
{
	EM_ASM_ARGS({
		var strokeStyle = "rgba("+$0+","+$1+","+$2+","+($3/gU1D0cANVA$.CHAR_MAX_AS_FLOAT)+")";
		gU1D0cANVA$.PEN_COLORS.length = 0;
		gU1D0cANVA$.PEN_COLORS.push(strokeStyle);
		gU1D0cANVA$.CANVAS.strokeStyle = strokeStyle;
	}, inColor.mRed, inColor.mGreen, inColor.mBlue, inColor.mAlpha);
}
// 50
void CanvasDevice::SelectPenWidth(float witdh)
{
}
// 51
void CanvasDevice::PushPenColor(const VGColor & inColor)
{
	EM_ASM_ARGS({
		var strokeStyle = "rgba("+$0+","+$1+","+$2+","+($3/gU1D0cANVA$.CHAR_MAX_AS_FLOAT)+")";
		gU1D0cANVA$.PEN_COLORS.push(strokeStyle);
		gU1D0cANVA$.CANVAS.strokeStyle = strokeStyle;
	}, inColor.mRed, inColor.mGreen, inColor.mBlue, inColor.mAlpha);
}
// 52
void CanvasDevice::PopPenColor()
{
	EM_ASM(
		gU1D0cANVA$.PEN_COLORS.pop();
		if (gU1D0cANVA$.PEN_COLORS.length > 0) {
			gU1D0cANVA$.CANVAS.strokeStyle = gU1D0cANVA$.PEN_COLORS[gU1D0cANVA$.PEN_COLORS.length - 1];
		} else {
			console.log("Popping pen color but the queue is empty");
		}
	);
}
// 53
void CanvasDevice::PushPenWidth(float width)
{
	EM_ASM_ARGS({
		gU1D0cANVA$.PEN_WIDTHS.push($0);
		gU1D0cANVA$.CONTEXT.lineWidth = $0;
	}, width);
}
// 54
void CanvasDevice::PopPenWidth()
{
	EM_ASM(
		gU1D0cANVA$.PEN_WIDTHS.pop();
		if (gU1D0cANVA$.PEN_WIDTHS.length > 0) {
			gU1D0cANVA$.CANVAS.lineWidth = gU1D0cANVA$.PEN_WIDTHS[gU1D0cANVA$.PEN_WIDTHS.length - 1];
		} else {
		console.log("Popping pen width but the queue is empty");
		}
	);
}
