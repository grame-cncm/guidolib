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

#include "BinaryDevice.h"
#include "VGFont.h"
#include "GUIDOEngine.h"

// we want to make sure all ints are 32 bit
#include <stdint.h>

using namespace std;

//______________________________________________________________________________
// BinaryDevice
//______________________________________________________________________________
BinaryDevice::BinaryDevice(std::ostream& outstream, BinarySystem* system) : 
	fSystem (system),
	fStream(outstream)
{
}

BinaryDevice::~BinaryDevice() 
{
}


//______________________________________________________________________________
// - Drawing services
//______________________________________________________________________________
bool BinaryDevice::BeginDraw()
{
  unsigned char id = 0;
  fStream.write((char *)&id, sizeof(unsigned char));
  return true;
}

void BinaryDevice::EndDraw()
{
  unsigned char id = 1;
  fStream.write((char *)&id, sizeof(unsigned char));
}

void BinaryDevice::InvalidateRect( float left, float top, float right, float bottom ) {
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
void BinaryDevice::MoveTo( float x, float y )  {
  unsigned char id = 3;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x, sizeof(float));
  fStream.write((char *)&y, sizeof(float));
}
void BinaryDevice::LineTo( float x, float y ) {
  unsigned char id = 4;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x, sizeof(float));
  fStream.write((char *)&y, sizeof(float));
}
void BinaryDevice::Line( float x1, float y1, float x2, float y2 )
{
  unsigned char id = 5;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x1, sizeof(float));
  fStream.write((char *)&y1, sizeof(float));
  fStream.write((char *)&x2, sizeof(float));
  fStream.write((char *)&y2, sizeof(float));
}

void BinaryDevice::Frame( float left, float top, float right, float bottom )
{
  unsigned char id = 6;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&left, sizeof(float));
  fStream.write((char *)&top, sizeof(float));
  fStream.write((char *)&right, sizeof(float));
  fStream.write((char *)&bottom, sizeof(float));
}

void BinaryDevice::Arc( float left, float top, float right, float bottom, float startX, float startY, float endX, float endY )
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
void BinaryDevice::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 )
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

void BinaryDevice::Polygon( const float * xCoords, const float * yCoords, int count )
{
  unsigned char id = 9;
  int32_t count32 = (int32_t)count;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&count32, sizeof(int32_t));
  for (int i = 0; i < count; i++) {
    fStream.write((char *)(xCoords + i), sizeof(float));
  }
  for (int i = 0; i < count; i++) {
    fStream.write((char *)(yCoords + i), sizeof(float));
  }
}

void BinaryDevice::Rectangle( float left,  float top, float right, float bottom )
{
  unsigned char id = 10;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&left, sizeof(float));
  fStream.write((char *)&top, sizeof(float));
  fStream.write((char *)&right, sizeof(float));
  fStream.write((char *)&bottom, sizeof(float));
}

//______________________________________________________________________________
void BinaryDevice::writeColor (const VGColor& color) const
{
	fStream.write((char *)&color.mAlpha, sizeof(unsigned char));
	fStream.write((char *)&color.mRed, sizeof(unsigned char));
	fStream.write((char *)&color.mGreen, sizeof(unsigned char));
	fStream.write((char *)&color.mBlue, sizeof(unsigned char));
}

//______________________________________________________________________________
// - Font services
//______________________________________________________________________________

void BinaryDevice::writeString(const char *str) const {
  int len = strlen(str);
  char null = '\0';
  fStream.write(str, sizeof(char) * len);
  fStream.write(&null, sizeof(char));
}

void BinaryDevice::writeFont(const VGFont *font) const {
  const char *name = font->GetName();
  int32_t size = font->GetSize();
  int32_t prop = font->GetProperties();
  
  writeString(name);
  fStream.write((char *)&size, sizeof(int32_t));
  fStream.write((char *)&prop, sizeof(int32_t));
}

void	BinaryDevice::SetMusicFont( const VGFont * font )	{
  unsigned char id = 11;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeFont(font);
}
const VGFont *	BinaryDevice::GetMusicFont() const {
  unsigned char id = 12;
  fStream.write((char *)&id, sizeof(unsigned char));
  return 0;
}
void			BinaryDevice::SetTextFont( const VGFont * font )	{
  unsigned char id = 13;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeFont(font);
}
const VGFont *	BinaryDevice::GetTextFont() const	{
  unsigned char id = 14;
  fStream.write((char *)&id, sizeof(unsigned char));
  return 0;
}

//______________________________________________________________________________
// - Pen & brush services
//______________________________________________________________________________
void BinaryDevice::SelectPen( const VGColor & c, float w )
{
  unsigned char id = 15;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(c);
  fStream.write((char *)&w, sizeof(float));
}

void BinaryDevice::SelectFillColor( const VGColor & c )
{
  unsigned char id = 16;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(c);
}

void BinaryDevice::PushPen( const VGColor & color, float width )
{
  unsigned char id = 17;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(color);
  fStream.write((char *)&width, sizeof(float));
}

void BinaryDevice::PopPen() {
  unsigned char id = 18;
  fStream.write((char *)&id, sizeof(unsigned char));
}

void BinaryDevice::PushFillColor( const VGColor & color )
{
  unsigned char id = 19;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(color);
}

void BinaryDevice::PopFillColor() {
  unsigned char id = 20;
  fStream.write((char *)&id, sizeof(unsigned char));
}

void BinaryDevice::writeRasterOpModeToString(VRasterOpMode mode) const {
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

void BinaryDevice::SetRasterOpMode( VRasterOpMode mode) {
  unsigned char id = 21;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeRasterOpModeToString(mode);
}
VGDevice::VRasterOpMode BinaryDevice::GetRasterOpMode() const {
  unsigned char id = 22;
  fStream.write((char *)&id, sizeof(unsigned char));
  return kUnknown;
}


//______________________________________________________________________________
// - Bitmap services (bit-block copy methods)
//______________________________________________________________________________
void BinaryDevice::writeFormattedImage (VGDevice* dev) const
{
  int length = 0;
  const char * data;
  const char* mimetype = dev->GetImageData (data, length);
  int32_t length32 = (int32_t)length;
  if (mimetype && length) {
    int32_t width = dev->GetWidth();
    int32_t height = dev->GetHeight();
    fStream.write((char *)&width, sizeof(int32_t));
    fStream.write((char *)&height, sizeof(int32_t));
    writeString(mimetype);
    fStream.write((char *)&length32, sizeof(int32_t));
    fStream.write((char *)&data, sizeof(char) * length);
    ReleaseImageData (mimetype);
  } else {
    int width = 0;
    int height = 0;
    char null = '\0';
    fStream.write((char *)&width, sizeof(int32_t));
    fStream.write((char *)&height, sizeof(int32_t));
    fStream.write(&null, sizeof(char));
    fStream.write((char *)&length32, sizeof(int32_t));
    fStream.write((char *)&data, sizeof(char) * length);    
  }
}

bool BinaryDevice::CopyPixels( VGDevice* pSrcDC, float alpha)
{
  unsigned char id = 23;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeFormattedImage(pSrcDC);
  fStream.write((char *)&alpha, sizeof(float));
  return true;
}

bool BinaryDevice::CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha)
{
  unsigned char id = 24;
  int32_t xDest32 = (int32_t)xDest;
  int32_t yDest32 = (int32_t)yDest;
  int32_t xSrc32 = (int32_t)xSrc;
  int32_t ySrc32 = (int32_t)ySrc;
  int32_t nSrcWidth32 = (int32_t)nSrcWidth;
  int32_t nSrcHeight32 = (int32_t)nSrcHeight;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&xDest32, sizeof(int32_t));
  fStream.write((char *)&yDest32, sizeof(int32_t));
  writeFormattedImage(pSrcDC);
  fStream.write((char *)&xSrc32, sizeof(int32_t));
  fStream.write((char *)&ySrc32, sizeof(int32_t));
  fStream.write((char *)&nSrcWidth32, sizeof(int32_t));
  fStream.write((char *)&nSrcHeight32, sizeof(int32_t));
  fStream.write((char *)&alpha, sizeof(float));
  return true;
}

bool BinaryDevice::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha)
{
  unsigned char id = 25;
  int32_t xDest32 = (int32_t)xDest;
  int32_t yDest32 = (int32_t)yDest;
  int32_t dstWidth32 = (int32_t)dstWidth;
  int32_t dstHeight32 = (int32_t)dstHeight;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&xDest32, sizeof(int32_t));
  fStream.write((char *)&yDest32, sizeof(int32_t));
  fStream.write((char *)&dstWidth32, sizeof(int32_t));
  fStream.write((char *)&dstHeight32, sizeof(int32_t));
  writeFormattedImage(pSrcDC);
  fStream.write((char *)&alpha, sizeof(float));
  return true;
}

bool BinaryDevice::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha)
{
  unsigned char id = 26;
  int32_t xDest32 = (int32_t)xDest;
  int32_t yDest32 = (int32_t)yDest;
  int32_t xSrc32 = (int32_t)xSrc;
  int32_t ySrc32 = (int32_t)ySrc;
  int32_t nSrcWidth32 = (int32_t)nSrcWidth;
  int32_t nSrcHeight32 = (int32_t)nSrcHeight;
  int32_t dstWidth32 = (int32_t)dstWidth;
  int32_t dstHeight32 = (int32_t)dstHeight;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&xDest32, sizeof(int32_t));
  fStream.write((char *)&yDest32, sizeof(int32_t));
  fStream.write((char *)&dstWidth32, sizeof(int32_t));
  fStream.write((char *)&dstHeight32, sizeof(int32_t));
  writeFormattedImage(pSrcDC);
  fStream.write((char *)&xSrc32, sizeof(int32_t));
  fStream.write((char *)&ySrc32, sizeof(int32_t));
  fStream.write((char *)&nSrcWidth32, sizeof(int32_t));
  fStream.write((char *)&nSrcHeight32, sizeof(int32_t));
  fStream.write((char *)&alpha, sizeof(float));
  return true;
}

//______________________________________________________________________________
// - Coordinate services
//______________________________________________________________________________
void BinaryDevice::SetScale( float x, float y )
{
  unsigned char id = 27;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x, sizeof(float));
  fStream.write((char *)&y, sizeof(float));
}

void BinaryDevice::SetOrigin( float x, float y )
{ 
  unsigned char id = 28;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x, sizeof(float));
  fStream.write((char *)&y, sizeof(float));
}

void BinaryDevice::OffsetOrigin( float x, float y )	
{ 
  unsigned char id = 29;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x, sizeof(float));
  fStream.write((char *)&y, sizeof(float));
}

void BinaryDevice::LogicalToDevice( float * x, float * y ) const {
  unsigned char id = 30;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)x, sizeof(float));
  fStream.write((char *)y, sizeof(float));
}

void BinaryDevice::DeviceToLogical( float * x, float * y ) const {
  unsigned char id = 31;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)x, sizeof(float));
  fStream.write((char *)y, sizeof(float));
}

float BinaryDevice::GetXScale() const {
  unsigned char id = 32;
  fStream.write((char *)&id, sizeof(unsigned char));
    return 0.0;
}
float BinaryDevice::GetYScale() const {
  unsigned char id = 33;
  fStream.write((char *)&id, sizeof(unsigned char));
    return 0.0;
}
float BinaryDevice::GetXOrigin() const {
  unsigned char id = 34;
  fStream.write((char *)&id, sizeof(unsigned char));
    return 0.0;
}
float BinaryDevice::GetYOrigin() const {
  unsigned char id = 35;
  fStream.write((char *)&id, sizeof(unsigned char));
    return 0.0;
}

void BinaryDevice::NotifySize( int w, int h ) {
  unsigned char id = 36;
  int32_t w32 = (int32_t)w;
  int32_t h32 = (int32_t)h;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&w32, sizeof(int32_t));
  fStream.write((char *)&h32, sizeof(int32_t));
}

int BinaryDevice::GetWidth() const {
  unsigned char id = 37;
  fStream.write((char *)&id, sizeof(unsigned char));
  return 0;
}

int BinaryDevice::GetHeight() const {
  unsigned char id = 38;
  fStream.write((char *)&id, sizeof(unsigned char));
  return 0;
}


//______________________________________________________________________________
// - Text and music symbols services
//______________________________________________________________________________
void BinaryDevice::DrawMusicSymbol(float x, float y, unsigned int inSymbolID )
{
  unsigned char id = 39;
  uint32_t inSymbolID32 = (uint32_t)inSymbolID;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x, sizeof(float));
  fStream.write((char *)&y, sizeof(float));
  fStream.write((char *)&inSymbolID32, sizeof(uint32_t));
}

void BinaryDevice::DrawString( float x, float y, const char * s, int inCharCount )
{
  unsigned char id = 40;
  int32_t inCharCount32 = (int32_t)inCharCount;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&x, sizeof(float));
  fStream.write((char *)&y, sizeof(float));
  fStream.write((char *)&inCharCount32, sizeof(int32_t));
  fStream.write(s, sizeof(char) * inCharCount);
}

void	BinaryDevice::SetFontColor( const VGColor & color ) {
  unsigned char id = 41;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(color);
}

VGColor BinaryDevice::GetFontColor() const	{
  unsigned char id = 42;
  fStream.write((char *)&id, sizeof(unsigned char));
  return VGColor();
}

void	BinaryDevice::SetFontBackgroundColor( const VGColor & color ) {
  unsigned char id = 43;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(color);
}

VGColor BinaryDevice::GetFontBackgroundColor() const	{
  unsigned char id = 44;
  fStream.write((char *)&id, sizeof(unsigned char));
  return VGColor();
}

void	BinaryDevice::SetFontAlign( unsigned int inAlign ) {
  unsigned char id = 45;
  uint32_t inAlign32 = (uint32_t)inAlign;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&inAlign32, sizeof(uint32_t));
}

unsigned int BinaryDevice::GetFontAlign() const	{
  unsigned char id = 46;
  fStream.write((char *)&id, sizeof(unsigned char));
  return 0;
}

//______________________________________________________________________________
// - Printer informations services - useless in svg context
//______________________________________________________________________________
void	BinaryDevice::SetDPITag( float inDPI ) {
  unsigned char id = 47;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&inDPI, sizeof(float));
}

float BinaryDevice::GetDPITag() const	{
  unsigned char id = 48;
  fStream.write((char *)&id, sizeof(unsigned char));
  return 0.0;
}

//______________________________________________________________________________
// - VGDevice extension
//______________________________________________________________________________
void BinaryDevice::SelectPenColor( const VGColor & color)
{
  unsigned char id = 49;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(color);
}

void BinaryDevice::SelectPenWidth( float width)
{
  unsigned char id = 50;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&width, sizeof(float));
}

void BinaryDevice::PushPenColor( const VGColor & color)
{
  unsigned char id = 51;
  fStream.write((char *)&id, sizeof(unsigned char));
  writeColor(color);
}

void BinaryDevice::PopPenColor() {
  unsigned char id = 52;
  fStream.write((char *)&id, sizeof(unsigned char));
}

void BinaryDevice::PushPenWidth( float width)
{
  unsigned char id = 53;
  fStream.write((char *)&id, sizeof(unsigned char));
  fStream.write((char *)&width, sizeof(float));
}

void BinaryDevice::PopPenWidth() {
  unsigned char id = 54;
  fStream.write((char *)&id, sizeof(unsigned char));
}
