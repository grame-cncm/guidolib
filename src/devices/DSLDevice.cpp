/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

/*
  To write a decoder for this binary data format, the decoder should do the following process.
  Read a character.  The character, ranging from 0-54, will say what drawing command we are decoding.
  Based on this command, we know exactly what type of data follows, and we decode it.
  Repeat this until all the commands are read in.
  
  The functions below show the order in which the data is written for each command.
  and a complete decoder implementing the above process is provided in tools/guidodsl2decoded.cpp.
*/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <assert.h>

#include "DSLDevice.h"
#include "VGFont.h"
#include "GUIDOEngine.h"
#include "base64.h"

using namespace std;

//______________________________________________________________________________
// DSLDevice
//______________________________________________________________________________
DSLDevice::DSLDevice(std::ostream& outstream, DSLSystem* system) : 
	fSystem (system),
	fStream(outstream)
{
}

DSLDevice::~DSLDevice() 
{
}


//______________________________________________________________________________
// - Drawing services
//______________________________________________________________________________
bool DSLDevice::BeginDraw()
{
  unsigned char id = 0;
  fStream.write((char *)&id, sizeof(unsigned char));
  return true;
}

void DSLDevice::EndDraw()
{
  unsigned char id = 1;
  fStream.write((char *)&id, sizeof(unsigned char));
}

void DSLDevice::InvalidateRect( float left, float top, float right, float bottom ) {
  unsigned char id = 2;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&left, sizeof(float));
  fStream.write((char *)&top, sizeof(float));
  fStream.write((char *)&right, sizeof(float));
  fStream.write((char *)&bottom, sizeof(float));
}

//______________________________________________________________________________
// - Standard graphic primitives
//______________________________________________________________________________
void DSLDevice::MoveTo( float x, float y )  {
  unsigned char id = 3;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x, sizeof(float));
  fStream.write((char *)&y, sizeof(float));
}
void DSLDevice::LineTo( float x, float y ) {
  unsigned char id = 4;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x, sizeof(float));
  fStream.write((char *)&y, sizeof(float));
}
void DSLDevice::Line( float x1, float y1, float x2, float y2 )
{
  unsigned char id = 5;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x1, sizeof(float));
  fStream.write((char *)&y1, sizeof(float));
  fStream.write((char *)&x2, sizeof(float));
  fStream.write((char *)&y2, sizeof(float));
}

void DSLDevice::Frame( float left, float top, float right, float bottom )
{
  unsigned char id = 6;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&left, sizeof(float));
  fStream.write((char *)&top, sizeof(float));
  fStream.write((char *)&right, sizeof(float));
  fStream.write((char *)&bottom, sizeof(float));
}

void DSLDevice::Arc( float left, float top, float right, float bottom, float startX, float startY, float endX, float endY )
{
  unsigned char id = 7;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&left, sizeof(float));
  fStream.write((char *)&top, sizeof(float));
  fStream.write((char *)&right, sizeof(float));
  fStream.write((char *)&bottom, sizeof(float));
  fStream.write((char *)&startX, sizeof(float));
  fStream.write((char *)&startY, sizeof(float));
  fStream.write((char *)&endX, sizeof(float));
  fStream.write((char *)&endY, sizeof(float));
}

//______________________________________________________________________________
// - Filled surfaces
//______________________________________________________________________________
void DSLDevice::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 )
{
  unsigned char id = 8;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x1, sizeof(float));
  fStream.write((char *)&y1, sizeof(float));
  fStream.write((char *)&x2, sizeof(float));
  fStream.write((char *)&y2, sizeof(float));
  fStream.write((char *)&x3, sizeof(float));
  fStream.write((char *)&y3, sizeof(float));
}

void DSLDevice::Polygon( const float * xCoords, const float * yCoords, int count )
{
  unsigned char id = 9;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&count, sizeof(int));
  for (int i = 0; i < count; i++) {
    fStream.write((char *)(xCoords + i), sizeof(float));
  }
  for (int i = 0; i < count; i++) {
    fStream.write((char *)(yCoords + i), sizeof(float));
  }
}

void DSLDevice::Rectangle( float left,  float top, float right, float bottom )
{
  unsigned char id = 10;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&left, sizeof(float));
  fStream.write((char *)&top, sizeof(float));
  fStream.write((char *)&right, sizeof(float));
  fStream.write((char *)&bottom, sizeof(float));
}

//______________________________________________________________________________
void DSLDevice::writeColor (const VGColor& color) const
{
	fStream.write((char *)&color.mAlpha, sizeof(unsigned char));
	fStream.write((char *)&color.mRed, sizeof(unsigned char));
	fStream.write((char *)&color.mGreen, sizeof(unsigned char));
	fStream.write((char *)&color.mBlue, sizeof(unsigned char));
}

//______________________________________________________________________________
// - Font services
//______________________________________________________________________________

void DSLDevice::writeString(const char *str) const {
  int len = strlen(str);
  char null = '\0';
  fStream.write(str, sizeof(char) * len);
  fStream.write(&null, sizeof(char));
}

void DSLDevice::writeFont(const VGFont *font) const {
  const char *name = font->GetName();
  int size = font->GetSize();
  int prop = font->GetProperties();
  
  writeString(name);
  fStream.write((char *)&size, sizeof(int));
  fStream.write((char *)&prop, sizeof(int));
}

void	DSLDevice::SetMusicFont( const VGFont * font )	{
  unsigned char id = 11;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeFont(font);
}
const VGFont *	DSLDevice::GetMusicFont() const {
  unsigned char id = 12;
  fStream.write((char *)&id, sizeof(unsigned char));
  return 0;
}
void			DSLDevice::SetTextFont( const VGFont * font )	{
  unsigned char id = 13;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeFont(font);
}
const VGFont *	DSLDevice::GetTextFont() const	{
  unsigned char id = 14;
  fStream.write((char *)&id, sizeof(unsigned char));
  return 0;
}

//______________________________________________________________________________
// - Pen & brush services
//______________________________________________________________________________
void DSLDevice::SelectPen( const VGColor & c, float w )
{
  unsigned char id = 15;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(c);
  fStream.write((char *)&w, sizeof(float));
}

void DSLDevice::SelectFillColor( const VGColor & c )
{
  unsigned char id = 16;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(c);
}

void DSLDevice::PushPen( const VGColor & color, float width )
{
  unsigned char id = 17;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(color);
  fStream.write((char *)&width, sizeof(float));
}

void DSLDevice::PopPen() {
  unsigned char id = 18;
  fStream.write((char *)&id, sizeof(unsigned char));
}

void DSLDevice::PushFillColor( const VGColor & color )
{
  unsigned char id = 19;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(color);
}

void DSLDevice::PopFillColor() {
  unsigned char id = 20;
  fStream.write((char *)&id, sizeof(unsigned char));
}

void DSLDevice::writeRasterOpModeToString(VRasterOpMode mode) const {
  char mode_ch;
  if (mode == kUnknown) mode_ch = 0;
  else if (mode == kOpCopy) mode_ch = 1;
  else if (mode == kOpAnd) mode_ch = 2;
  else if (mode == kOpXOr) mode_ch = 3;
  else if (mode == kOpInvert) mode_ch = 4;
  else if (mode == kOpOr) mode_ch = 5;
  else mode_ch = 0;

  fStream.write(&mode_ch, sizeof(char));
}

void DSLDevice::SetRasterOpMode( VRasterOpMode mode) {
  unsigned char id = 21;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeRasterOpModeToString(mode);
}
VGDevice::VRasterOpMode DSLDevice::GetRasterOpMode() const {
  unsigned char id = 22;
  fStream.write((char *)&id, sizeof(unsigned char));
  return kUnknown;
}


//______________________________________________________________________________
// - Bitmap services (bit-block copy methods)
//______________________________________________________________________________
void DSLDevice::writeFormattedImage (VGDevice* dev) const
{
  int length = 0;
  const char * data;
  const char* mimetype = dev->GetImageData (data, length);
  if (mimetype && length) {
    int width = dev->GetWidth();
    int height = dev->GetHeight();
    fStream.write((char *)&width, sizeof(int));
    fStream.write((char *)&height, sizeof(int));
    writeString(mimetype);
    fStream.write((char *)&length, sizeof(int));
    fStream.write((char *)&data, sizeof(char) * length);
    ReleaseImageData (mimetype);
  } else {
    int width = 0;
    int height = 0;
    char null = '\0';
    fStream.write((char *)&width, sizeof(int));
    fStream.write((char *)&height, sizeof(int));
    fStream.write(&null, sizeof(char));
    fStream.write((char *)&length, sizeof(int));
    fStream.write((char *)&data, sizeof(char) * length);    
  }
}

bool DSLDevice::CopyPixels( VGDevice* pSrcDC, float alpha)
{
  unsigned char id = 23;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeFormattedImage(pSrcDC);
  fStream.write((char *)&alpha, sizeof(float));
  return true;
}

bool DSLDevice::CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha)
{
  unsigned char id = 24;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&xDest, sizeof(int));
  fStream.write((char *)&yDest, sizeof(int));
  writeFormattedImage(pSrcDC);
  fStream.write((char *)&xSrc, sizeof(int));
  fStream.write((char *)&ySrc, sizeof(int));
  fStream.write((char *)&nSrcWidth, sizeof(int));
  fStream.write((char *)&nSrcHeight, sizeof(int));
  fStream.write((char *)&alpha, sizeof(float));
  return true;
}

bool DSLDevice::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha)
{
  unsigned char id = 25;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&xDest, sizeof(int));
  fStream.write((char *)&yDest, sizeof(int));
  fStream.write((char *)&dstWidth, sizeof(int));
  fStream.write((char *)&dstHeight, sizeof(int));
  writeFormattedImage(pSrcDC);
  fStream.write((char *)&alpha, sizeof(float));
  return true;
}

bool DSLDevice::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha)
{
  unsigned char id = 26;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&xDest, sizeof(int));
  fStream.write((char *)&yDest, sizeof(int));
  fStream.write((char *)&dstWidth, sizeof(int));
  fStream.write((char *)&dstHeight, sizeof(int));
  writeFormattedImage(pSrcDC);
  fStream.write((char *)&xSrc, sizeof(int));
  fStream.write((char *)&ySrc, sizeof(int));
  fStream.write((char *)&nSrcWidth, sizeof(int));
  fStream.write((char *)&nSrcHeight, sizeof(int));
  fStream.write((char *)&alpha, sizeof(float));
  return true;
}

//______________________________________________________________________________
// - Coordinate services
//______________________________________________________________________________
void DSLDevice::SetScale( float x, float y )
{
  unsigned char id = 27;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x, sizeof(float));
  fStream.write((char *)&y, sizeof(float));
}

void DSLDevice::SetOrigin( float x, float y )
{ 
  unsigned char id = 28;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x, sizeof(float));
  fStream.write((char *)&y, sizeof(float));
}

void DSLDevice::OffsetOrigin( float x, float y )	
{ 
  unsigned char id = 29;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x, sizeof(float));
  fStream.write((char *)&y, sizeof(float));
}

void DSLDevice::LogicalToDevice( float * x, float * y ) const {
  unsigned char id = 30;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)x, sizeof(float));
  fStream.write((char *)y, sizeof(float));
}

void DSLDevice::DeviceToLogical( float * x, float * y ) const {
  unsigned char id = 31;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)x, sizeof(float));
  fStream.write((char *)y, sizeof(float));
}

float DSLDevice::GetXScale() const {
  unsigned char id = 32;
  fStream.write((char *)&id, sizeof(unsigned char));
    return 0.0;
}
float DSLDevice::GetYScale() const {
  unsigned char id = 33;
  fStream.write((char *)&id, sizeof(unsigned char));
    return 0.0;
}
float DSLDevice::GetXOrigin() const {
  unsigned char id = 34;
  fStream.write((char *)&id, sizeof(unsigned char));
    return 0.0;
}
float DSLDevice::GetYOrigin() const {
  unsigned char id = 35;
  fStream.write((char *)&id, sizeof(unsigned char));
    return 0.0;
}

void DSLDevice::NotifySize( int w, int h ) {
  unsigned char id = 36;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&w, sizeof(int));
  fStream.write((char *)&h, sizeof(int));
}

int DSLDevice::GetWidth() const {
  unsigned char id = 37;
  fStream.write((char *)&id, sizeof(unsigned char));
  return 0;
}

int DSLDevice::GetHeight() const {
  unsigned char id = 38;
  fStream.write((char *)&id, sizeof(unsigned char));
  return 0;
}


//______________________________________________________________________________
// - Text and music symbols services
//______________________________________________________________________________
void DSLDevice::DrawMusicSymbol(float x, float y, unsigned int inSymbolID )
{
  unsigned char id = 39;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x, sizeof(float));
  fStream.write((char *)&y, sizeof(float));
  fStream.write((char *)&inSymbolID, sizeof(unsigned int));
}

void DSLDevice::DrawString( float x, float y, const char * s, int inCharCount )
{
  unsigned char id = 40;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x, sizeof(float));
  fStream.write((char *)&y, sizeof(float));
  fStream.write((char *)&inCharCount, sizeof(int));
  fStream.write(s, sizeof(char) * inCharCount);
}

void	DSLDevice::SetFontColor( const VGColor & color ) {
  unsigned char id = 41;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(color);
}

VGColor DSLDevice::GetFontColor() const	{
  unsigned char id = 42;
  fStream.write((char *)&id, sizeof(unsigned char));
  return VGColor();
}

void	DSLDevice::SetFontBackgroundColor( const VGColor & color ) {
  unsigned char id = 43;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(color);
}

VGColor DSLDevice::GetFontBackgroundColor() const	{
  unsigned char id = 44;
  fStream.write((char *)&id, sizeof(unsigned char));
  return VGColor();
}

void	DSLDevice::SetFontAlign( unsigned int inAlign ) {
  unsigned char id = 45;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&inAlign, sizeof(unsigned int));
}

unsigned int DSLDevice::GetFontAlign() const	{
  unsigned char id = 46;
  fStream.write((char *)&id, sizeof(unsigned char));
  return 0;
}

//______________________________________________________________________________
// - Printer informations services - useless in svg context
//______________________________________________________________________________
void	DSLDevice::SetDPITag( float inDPI ) {
  unsigned char id = 47;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&inDPI, sizeof(float));
}

float DSLDevice::GetDPITag() const	{
  unsigned char id = 48;
  fStream.write((char *)&id, sizeof(unsigned char));
  return 0.0;
}

//______________________________________________________________________________
// - VGDevice extension
//______________________________________________________________________________
void DSLDevice::SelectPenColor( const VGColor & color)
{
  unsigned char id = 49;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(color);
}

void DSLDevice::SelectPenWidth( float width)
{
  unsigned char id = 50;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&width, sizeof(float));
}

void DSLDevice::PushPenColor( const VGColor & color)
{
  unsigned char id = 51;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(color);
}

void DSLDevice::PushPenWidth( float width)
{
  unsigned char id = 52;
  fStream.write((char *)&id, sizeof(unsigned char));
}
void DSLDevice::PopPenColor() {
  unsigned char id = 53;
  fStream.write((char *)&id, sizeof(unsigned char));
}

void DSLDevice::PopPenWidth() {
  unsigned char id = 54;
  fStream.write((char *)&id, sizeof(unsigned char));
}
