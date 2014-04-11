/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <assert.h>

#include "AbstractDevice.h"
#include "VGFont.h"
#include "GUIDOEngine.h"
#include "base64.h"

using namespace std;

//______________________________________________________________________________
// AbstractDevice
//______________________________________________________________________________
AbstractDevice::AbstractDevice(std::ostream& outstream, AbstractSystem* system) : 
	fSystem (system),
	fStream(outstream), 
	fSpace(" ")
{
}

AbstractDevice::~AbstractDevice() 
{
}


//______________________________________________________________________________
// - Drawing services
//______________________________________________________________________________
bool AbstractDevice::BeginDraw()
{
  fStream << "BeginDraw" << endl;
  return true;
}

void AbstractDevice::EndDraw()
{
  fStream << "EndDraw" << endl;
}

void AbstractDevice::InvalidateRect( float left, float top, float right, float bottom ) {
  fStream << "InvalidateRect" << fSpace << left << fSpace << top << fSpace << right << fSpace << bottom << endl;
}

//______________________________________________________________________________
// - Standard graphic primitives
//______________________________________________________________________________
void AbstractDevice::MoveTo( float x, float y )  {
  fStream << "MoveTo" << fSpace << x << fSpace << y << endl;
}
void AbstractDevice::LineTo( float x, float y ) {
  fStream << "LineTo" << fSpace << x << fSpace << y << endl;
}
void AbstractDevice::Line( float x1, float y1, float x2, float y2 )
{
  fStream << "Line" << fSpace << x1 << fSpace << y1 << fSpace << x2 << fSpace << y2 << endl;
}

void AbstractDevice::Frame( float left, float top, float right, float bottom )
{
	fStream << "Frame" << fSpace << left << fSpace << top << fSpace << right << fSpace << bottom << endl;
}

void AbstractDevice::Arc( float left, float top, float right, float bottom, float startX, float startY, float endX, float endY )
{
	fStream << "Arc" << fSpace << left << fSpace << top << fSpace << right << fSpace << bottom << fSpace << startX << fSpace << startY << fSpace << endX << fSpace << endY << endl;
}

//______________________________________________________________________________
// - Filled surfaces
//______________________________________________________________________________
void AbstractDevice::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 )
{
	fStream << "Triangle" << fSpace << x1 << y1 << x2 << y2 << x3 << y3 << endl;
}

void AbstractDevice::Polygon( const float * xCoords, const float * yCoords, int count )
{
	fStream << "Polygon" << fSpace;
	for (int i = 0; i < count; i++) {
		fStream << xCoords[i] << fSpace;
	}
	for (int i = 0; i < count; i++) {
		fStream << yCoords[i] << fSpace;
	}
	fStream << count << endl;
}

void AbstractDevice::Rectangle( float left,  float top, float right, float bottom )
{
	fStream << "Rectangle" << fSpace << left << fSpace << top << fSpace << right << fSpace << bottom << endl;
}

//______________________________________________________________________________
void AbstractDevice::writeColor (const VGColor& color) const
{
	char buff[13];
	sprintf (buff, "#%02x%02x%02x%02x",  int(color.mAlpha), int(color.mRed), int(color.mGreen), int(color.mBlue));
	fStream << buff;
}

//______________________________________________________________________________
// - Font services
//______________________________________________________________________________

void AbstractDevice::writeFont(const VGFont *font) const {
    fStream << font->GetName() << fSpace << font->GetSize() << fSpace << font->GetProperties();
}

void	AbstractDevice::SetMusicFont( const VGFont * font )	{
    fStream << "SetMusicFont" << fSpace;
    writeFont(font);
    fStream << endl;
}
const VGFont *	AbstractDevice::GetMusicFont() const {
    fStream << "GetMusicFont" << endl;
    return 0;
}
void			AbstractDevice::SetTextFont( const VGFont * font )	{
    fStream << "SetTextFont" << fSpace;
    writeFont(font);
    fStream << endl;
}
const VGFont *	AbstractDevice::GetTextFont() const	{
    fStream << "GetMusicFont" << endl;
    return 0;
}

//______________________________________________________________________________
// - Pen & brush services
//______________________________________________________________________________
void AbstractDevice::SelectPen( const VGColor & c, float w )
{
	fStream << "SelectPen" << fSpace;
	writeColor(c);
	fStream << fSpace << w << endl;
}
void AbstractDevice::SelectFillColor( const VGColor & c )
{
	fStream << "SelectFillColor" << fSpace;
	writeColor(c);
	fStream << endl;
}

void AbstractDevice::PushPen( const VGColor & color, float width )
{
	fStream << "PushPen" << fSpace;
	writeColor(color);
	fStream << fSpace << width << endl;
}
void AbstractDevice::PushFillColor( const VGColor & color )
{
	fStream << "PushFillColor" << fSpace;
	writeColor(color);
	fStream << endl;
}

void AbstractDevice::PopPen() { fStream << "PopPen" << endl; }
void AbstractDevice::PopFillColor() { fStream << "PopFillColor" << endl; }

void AbstractDevice::writeRasterOpModeToString(VRasterOpMode mode) const {
  if (mode == kUnknown) fStream << "unknown";
  else if (mode == kOpCopy) fStream << "copy";
  else if (mode == kOpAnd) fStream << "and";
  else if (mode == kOpXOr) fStream << "xor";
  else if (mode == kOpInvert) fStream << "invert";
  else if (mode == kOpOr) fStream << "or";
  else fStream << "unknown";
}

void AbstractDevice::SetRasterOpMode( VRasterOpMode mode) {
	fStream << "SetRasterOpMode" << fSpace;
    writeRasterOpModeToString(mode);
    fStream << endl;
}
VGDevice::VRasterOpMode AbstractDevice::GetRasterOpMode() const {
	fStream << "GetRasterOpMode" << endl;
    return kUnknown;
}


//______________________________________________________________________________
// - Bitmap services (bit-block copy methods)
//______________________________________________________________________________
void AbstractDevice::writeFormattedImage (VGDevice* dev) const
{
	int length = 0;
	const char * data;
	const char* mimetype = dev->GetImageData (data, length);
	if (mimetype && length) {
		fStream << dev->GetWidth() << fSpace << dev->GetHeight()
			<< mimetype << fSpace << length << fSpace;
		for (int i = 0; i < length; i++) {
			fStream << data[i];
		}
		ReleaseImageData (mimetype);
	}
}

bool AbstractDevice::CopyPixels( VGDevice* pSrcDC, float alpha)
{
	fStream << "CopyPixels" << fSpace;
	writeFormattedImage( pSrcDC);
	fStream << fSpace << alpha << endl;
	return true;
}

bool AbstractDevice::CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha)
{
	fStream << "CopyPixels" << fSpace << xDest << fSpace << yDest << fSpace;
	writeFormattedImage( pSrcDC);
	fStream << fSpace << xSrc << fSpace << ySrc << fSpace << nSrcWidth << fSpace << nSrcHeight << fSpace << alpha << endl;
	return true;
}

bool AbstractDevice::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha)
{
	fStream << "CopyPixels" << fSpace << xDest << fSpace << yDest << fSpace << dstWidth << fSpace << dstHeight << fSpace;
	writeFormattedImage( pSrcDC);
	fStream << fSpace << alpha << endl;
	return true;
}

bool AbstractDevice::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha)
{
	fStream << "CopyPixels" << fSpace << xDest << fSpace << yDest << fSpace << dstWidth << fSpace << dstHeight << fSpace;
	writeFormattedImage( pSrcDC);
	fStream << fSpace << xSrc << fSpace << ySrc << fSpace << nSrcWidth << fSpace << nSrcHeight << fSpace << alpha << endl;
	return true;
}

//______________________________________________________________________________
// - Coordinate services
//______________________________________________________________________________
void AbstractDevice::SetScale( float x, float y )
{
  fStream << "SetScale" << fSpace << x << fSpace << y << endl;
}

void AbstractDevice::SetOrigin( float x, float y )
{ 
  fStream << "SetOrigin" << fSpace << x << fSpace << y << endl;
}

void AbstractDevice::OffsetOrigin( float x, float y )	
{ 
  fStream << "OffsetOrigin" << fSpace << x << fSpace << y << endl;
}

void AbstractDevice::LogicalToDevice( float * x, float * y ) const {
  fStream << "LogicalToDevice" << fSpace << x << fSpace << y << endl;
}

void AbstractDevice::DeviceToLogical( float * x, float * y ) const {
  fStream << "DeviceToLogical" << fSpace << x << fSpace << y << endl;
}

float AbstractDevice::GetXScale() const {
  fStream << "GetXScale" << endl;
  return 0.0;
}
float AbstractDevice::GetYScale() const {
  fStream << "GetYSclae" << endl;
  return 0.0;
}
float AbstractDevice::GetXOrigin() const {
  fStream << "GetXOrigin" << endl;
  return 0.0;
}
float AbstractDevice::GetYOrigin() const {
  fStream << "GetYOrigin" << endl;
  return 0.0;
}

void AbstractDevice::NotifySize( int w, int h ) {
  fStream << "NotifySize" << fSpace << w << fSpace << h << endl;
}

int AbstractDevice::GetWidth() const {
  fStream << "GetWidth" << endl;
  return 0;
}

int AbstractDevice::GetHeight() const {
  fStream << "GetWidth" << endl;
  return 0;
}


//______________________________________________________________________________
// - Text and music symbols services
//______________________________________________________________________________
void AbstractDevice::DrawMusicSymbol(float x, float y, unsigned int inSymbolID )
{
	fStream << "DrawMusicSymbol" << fSpace << x << fSpace << y << fSpace << inSymbolID << endl;
}

void AbstractDevice::DrawString( float x, float y, const char * s, int inCharCount )
{
	fStream << "DrawString" << fSpace << x << fSpace << y << fSpace << s << fSpace << inCharCount << endl;
}

void	AbstractDevice::SetFontColor( const VGColor & color ) {
	fStream << "SetFontColor" << fSpace;
	writeColor(color);
	fStream << endl;
}
VGColor AbstractDevice::GetFontColor() const	{
  fStream << "GetFontColor" << endl;
  return VGColor();
}

void	AbstractDevice::SetFontBackgroundColor( const VGColor & color ) {
	fStream << "SetFontBackgroundColor" << fSpace;
	writeColor(color);
	fStream << endl;
}

VGColor AbstractDevice::GetFontBackgroundColor() const	{
  fStream << "GetFontBackgroundColor" << endl;
  return VGColor();
}

void	AbstractDevice::SetFontAlign( unsigned int inAlign ) {
	fStream << "SetFontAlign" << fSpace << inAlign << endl;
}

unsigned int AbstractDevice::GetFontAlign() const	{
  fStream << "GetFontAlign" << endl;
  return 0;
}

//______________________________________________________________________________
// - Printer informations services - useless in svg context
//______________________________________________________________________________
void	AbstractDevice::SetDPITag( float inDPI ) {
	fStream << "SetDPITag" << fSpace << inDPI << endl;
}

float AbstractDevice::GetDPITag() const	{
  fStream << "GetDPITag" << endl;
  return 0.0;
}

//______________________________________________________________________________
// - VGDevice extension
//______________________________________________________________________________
void AbstractDevice::SelectPenColor( const VGColor & color)
{
	fStream << "SelectPenColor" << fSpace;
	writeColor(color);
	fStream << endl;
}

void AbstractDevice::SelectPenWidth( float width)
{
	fStream << "SelectPenWidth" << fSpace << width << endl;
}

void AbstractDevice::PushPenColor( const VGColor & color)
{
	fStream << "PushPenColor" << fSpace;
	writeColor(color);
	fStream << endl;
}

void AbstractDevice::PushPenWidth( float width)
{
	fStream << "PushPenWidth" << fSpace << width << endl;
}
void AbstractDevice::PopPenColor() {
	fStream << "PopPenColor" << endl;
}
void AbstractDevice::PopPenWidth() {
	fStream << "PopPenWidth" << endl;
}
