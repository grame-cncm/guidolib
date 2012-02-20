#ifndef __DebugDevice__
#define __DebugDevice__

/*
	GUIDO Library
	Copyright (C) 2003, 2004	Grame
	
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "DecoratorDevice.h"
#include "VGFont.h"

#include <iostream>
#include <fstream>
using namespace std;

// --------------------------------------------------------------
class DebugDevice : public DecoratorDevice
{

	public:
					 DebugDevice(VGDevice& dev, const char* outfile=0) : DecoratorDevice(&dev, false), filestream(0)
							 { if (outfile) filestream = new ofstream(outfile); }
					 DebugDevice(VGDevice * dev, const char* outfile=0) : DecoratorDevice(dev), filestream(0) 
							 { if (outfile) filestream = new ofstream(outfile); }
		virtual		~DebugDevice() 
							 { if (filestream) { filestream->close(); delete filestream; } }

		// Returns the ability of the current VGdevice to be drawn into.	
		virtual bool		IsValid() const;
		
		// - Drawing services		
		virtual bool		BeginDraw();
		virtual void		EndDraw();
		
		virtual void		MoveTo(float x, float y);
		virtual void		LineTo(float x, float y);
		virtual void		Line(float x1, float y1, float x2, float y2);
		virtual void		Frame(float left, float top, float right, float bottom);
		virtual void		Arc(float left, float top, float right, float bottom, float startX, float startY, float endX, float endY);
		virtual	void		Triangle(float x1, float y1, float x2, float y2, float x3, float y3);
		virtual	void		Polygon(const float* xCoords, const float* yCoords, int count);
		virtual void		Rectangle(float left, float top, float right, float bottom);	
	
 		// - Musical symbol services
		virtual	void		SetMusicFont(const VGFont* font);
		virtual	const VGFont*	GetMusicFont() const;	
		virtual	void		SetTextFont(const VGFont* font);		
		virtual	const VGFont*	GetTextFont() const;	
		
		// - Pen & brush services 
		virtual	void		SelectPen(const VGColor & inColor,  float witdh); 
		virtual	void		SelectFillColor(const VGColor& c);	
		virtual	void		PushPen(const VGColor & inColor, float inWidth);
		virtual	void		PopPen();
		virtual	void		PushFillColor(const VGColor& inColor);
		virtual	void		PopFillColor();
		virtual	void		SetRasterOpMode(VRasterOpMode ROpMode);
		virtual	VRasterOpMode	GetRasterOpMode() const;
 	
		// - Bitmap services (bit-block copy methods) 
		virtual bool		CopyPixels(VGDevice* pSrcDC, float alpha = -1.0); 
		virtual bool		CopyPixels(int xDest, int yDest,
										VGDevice* pSrcDC, 
										int xSrc, int ySrc, 
										int nSrcWidth, int nSrcHeight, float alpha = -1.0);	
		virtual bool		CopyPixels(int xDest, int yDest, 
										int dstWidth, int dstHeight, 
										VGDevice* pSrcDC, float alpha = -1.0);	
		
		// - Coordinate services
		virtual	void		SetScale(float x, float y);
		virtual	void		SetOrigin(float x, float y);
		virtual	void		OffsetOrigin(float x, float y);
		virtual	void		LogicalToDevice(float * x, float * y) const;
		virtual	void		DeviceToLogical(float * x, float * y) const;
		virtual float		GetXScale() const;
		virtual	float		GetYScale() const;
		virtual	float		GetXOrigin() const;
		virtual	float		GetYOrigin() const;
		virtual	void		NotifySize(int inWidth, int inHeight);
		virtual	int			GetWidth() const;
		virtual	int			GetHeight() const;
		
		// - Text and music symbols services
		virtual void		DrawMusicSymbol(float x, float y, unsigned int inSymbolID);
		virtual	void		DrawString(float x, float y, const char * s, int inCharCount);
		virtual	void		SetFontColor(const VGColor & inColor);
		virtual	VGColor		GetFontColor() const;
		virtual	void		SetFontBackgroundColor(const VGColor & inColor);
		virtual	VGColor		GetFontBackgroundColor() const;
		virtual	void		SetFontAlign(unsigned int inAlign);
		virtual	unsigned int GetFontAlign() const;
		
		// - Printer informations services 
		virtual	void		SetDPITag(float inDPI);
		virtual	float		GetDPITag() const;
		virtual void*		GetBitMapPixels();
		virtual void		ReleaseBitMapPixels();
		virtual	VGSystem *	getVGSystem() const;
		
		ofstream * filestream;

		// - VGDevice extension
		virtual	void			SelectPenColor(const VGColor & inColor);
		virtual	void			SelectPenWidth(float witdh);
		virtual	void			PushPenColor( const VGColor & inColor);
		virtual	void			PopPenColor();
		virtual	void			PushPenWidth( float width);
		virtual	void			PopPenWidth();
};


#ifdef WIN32
# define dbgStream cout
#else
# define dbgStream   (filestream ? (*filestream) : cout)
#endif

// --------------------------------------------------------------
inline bool DebugDevice::IsValid() const {
	dbgStream << "Device::IsValid " << endl;
	return fDevice ? fDevice->IsValid () : false;
}

// --------------------------------------------------------------
inline bool DebugDevice::BeginDraw() {
	dbgStream << "Device::BeginDraw " << endl;
	return (fDevice) ? fDevice->BeginDraw() : false;
	return true;
}

inline void DebugDevice::EndDraw() {
	dbgStream << "Device::EndDraw " << endl;
	if (fDevice) fDevice->EndDraw();
}

// --------------------------------------------------------------
inline void DebugDevice::MoveTo( float x, float y ) {
	dbgStream << "Device::MoveTo (" << x << ", " << y << ")" << endl;
	if (fDevice) fDevice->MoveTo (x, y);
}
inline void DebugDevice::LineTo( float x, float y ) {
	dbgStream << "Device::LineTo (" << x << ", " << y << ")" << endl;
	if (fDevice) fDevice->LineTo (x, y);
}
inline void DebugDevice::Line( float x1, float y1, float x2, float y2 ) {
	dbgStream << "Device::Line (" << x1 << ", " << y1 << ", " << x2 << ", " << y2 << ")" << endl;
	if (fDevice) fDevice->Line (x1, y1, x2, y2);
}
inline void DebugDevice::Frame( float left, float top, float right, float bottom ) {
	dbgStream << "Device::Frame (" << left << ", " << top << ", " << right << ", " << bottom << ")" << endl;
	if (fDevice) fDevice->Frame (left, top, right, bottom);
}
inline void DebugDevice::Arc( float left, float top, float right, float bottom, float startX, float startY, float endX, float endY ) {
	dbgStream << "Device::Arc (" << left << ", " << top << ", " << right << ", " << bottom << "," << startX << ", " << startY << ", " << endX << ", " << endY << ")" << endl;
	if (fDevice) fDevice->Arc (left, top, right, bottom, startX, startY, endX, endY);
}
inline void DebugDevice::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 ) {
	dbgStream << "Device::Triangle (" << x1 << ", " << y1 << ", " << x2 << ", " << y2 << ", " << x3 << ", " << y3 << ")" << endl;
	if (fDevice) fDevice->Triangle (x1, y1, x2, y2, x3, y3);
}
inline void DebugDevice::Polygon( const float * x, const float * y, int count ) {
	dbgStream << "Device::Polygon count: " << count << endl;
	if (fDevice) fDevice->Polygon (x, y, count);
}
inline void DebugDevice::Rectangle( float left, float top, float right, float bottom ) {
	dbgStream << "Device::Rectangle (" << left << ", " << top << ", " << right << ", " << bottom << ")" << endl;
	if (fDevice) fDevice->Rectangle (left, top, right, bottom);
}

// --------------------------------------------------------------
inline void DebugDevice::SetMusicFont(const VGFont * font) {
	dbgStream << "Device::SetMusicFont " << (void*)font;
	if (font) cout << " " << font->GetName() << " " << font->GetSize ();
	cout << endl;
	if (fDevice) fDevice->SetMusicFont(font);
}
inline const VGFont* DebugDevice::GetMusicFont() const {
	dbgStream << "Device::GetMusicFont" << endl;
	return (fDevice) ? fDevice->GetMusicFont(): NULL;
}
inline void DebugDevice::SetTextFont(const VGFont * font) {
	dbgStream << "Device::SetTextFont " << (void*)font;
	if (font) cout << " " << font->GetName() << " " << font->GetSize ();
	cout << endl;
	if (fDevice) fDevice->SetTextFont(font);
}
inline const VGFont* DebugDevice::GetTextFont() const {
	dbgStream << "Device::GetTextFont" << endl;
	return (fDevice) ? fDevice->GetTextFont(): NULL;
}

// --------------------------------------------------------------
inline void DebugDevice::SelectPen(const VGColor & inColor, float witdh) {
	dbgStream << "Device::SelectPen (" << witdh << ")" << endl;
	if (fDevice) fDevice->SelectPen(inColor, witdh);
}
inline void DebugDevice::SelectPenColor(const VGColor & inColor) {
	dbgStream << "Device::SelectPenColor" << endl;
	if (fDevice) fDevice->SelectPenColor(inColor);
}
inline void DebugDevice::SelectPenWidth(float witdh) {
	dbgStream << "Device::SelectPenWidth (" << witdh << ")" << endl;
	if (fDevice) fDevice->SelectPenWidth(witdh);
}

inline void DebugDevice::PushPenColor(const VGColor & inColor) {
	dbgStream << "Device::PushPenColor " << inColor << endl;
	if (fDevice) fDevice->PushPenColor(inColor);
}
inline void DebugDevice::PushPenWidth(float witdh) {
	dbgStream << "Device::PushPenWidth (" << witdh << ")" << endl;
	if (fDevice) fDevice->PushPenWidth(witdh);
}
inline void DebugDevice::PopPenColor() {
	dbgStream << "Device::PopPenColor" << endl;
	if (fDevice) fDevice->PopPenColor();
}
inline void DebugDevice::PopPenWidth() {
	dbgStream << "Device::PopPenWidth" << endl;
	if (fDevice) fDevice->PopPenWidth();
}

inline void DebugDevice::SelectFillColor(const VGColor & inColor) {
	dbgStream << "Device::SelectFillColor " << inColor << endl;
	if (fDevice) fDevice->SelectFillColor(inColor);
}
inline void DebugDevice::PushPen(const VGColor & inColor, float witdh) {
	dbgStream << "Device::PushPen (" << witdh << ")" <<  endl;
	if (fDevice) fDevice->PushPen(inColor, witdh);
}
inline void DebugDevice::PopPen() {
	dbgStream << "Device::PopPen" << endl;
	if (fDevice) fDevice->PopPen();
}
inline void DebugDevice::PushFillColor(const VGColor & inColor) {
	dbgStream << "Device::PushFillColor " << inColor << endl;
	if (fDevice) fDevice->PushFillColor(inColor);
}
inline void DebugDevice::PopFillColor() {
	dbgStream << "Device::PopFillColor" << endl;
	if (fDevice) fDevice->PopFillColor();
}
inline void DebugDevice::SetRasterOpMode(VGDevice::VRasterOpMode ROpMode) {
	dbgStream << "Device::SetRasterOpMode (" << ROpMode << ")"<< endl;
	if (fDevice) fDevice->SetRasterOpMode(ROpMode);
}
inline VGDevice::VRasterOpMode DebugDevice::GetRasterOpMode() const{
	dbgStream << "Device::GetRasterOpMode" << endl;
	return (fDevice) ? fDevice->GetRasterOpMode() : VGDevice::kUnknown;
}

// --------------------------------------------------------------
inline bool DebugDevice::CopyPixels(VGDevice* pSrcDC, float alpha) {
	dbgStream << "Device::CopyPixels (" << alpha << ")" << endl;
	return (fDevice) ? fDevice->CopyPixels(pSrcDC, alpha) : false;
}
inline bool DebugDevice::CopyPixels(int xDest, int yDest,
									VGDevice* pSrcDC, 
									int xSrc, int ySrc, 
									int nSrcWidth, int nSrcHeight, float alpha) {
	dbgStream << "Device::CopyPixels (" << xDest << ", " << yDest << ", " << xSrc << ", " << ySrc << ", " << nSrcWidth << ", " << nSrcHeight << ", " << alpha << ")" << endl;
	return (fDevice) ? fDevice->CopyPixels(xDest, yDest, pSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, alpha) : false;
}
inline bool DebugDevice::CopyPixels(int xDest, int yDest, 
									int dstWidth, int dstHeight, 
									VGDevice* pSrcDC, float alpha) {
	dbgStream << "Device::CopyPixels (" << xDest << ", " << yDest <<  ", " << dstWidth << ", " << dstHeight << ", " << alpha << ")" << endl;
	return (fDevice) ? fDevice->CopyPixels(xDest, yDest, dstWidth, dstHeight, pSrcDC, alpha) : false;
}

// --------------------------------------------------------------
inline void DebugDevice::SetScale(float x, float y) {
	dbgStream << "Device::SetScale (" << x << ", " << y << ")" << endl;
	if (fDevice) fDevice->SetScale(x, y);
}
inline void DebugDevice::SetOrigin(float x, float y) {
	dbgStream << "Device::SetOrigin (" << x << ", " << y << ")" << endl;
	if (fDevice) fDevice->SetOrigin(x, y);
}
inline void DebugDevice::OffsetOrigin(float x, float y) {
	dbgStream << "Device::OffsetOrigin (" << x << ", " << y << ")" << endl;
	if (fDevice) fDevice->OffsetOrigin(x, y);
}
inline void DebugDevice::LogicalToDevice(float * x, float * y) const {
	if (fDevice) fDevice->LogicalToDevice(x, y);
	dbgStream << "Device::LogicalToDevice (" << *x << ", " << *y << ")" << endl;
}
inline void DebugDevice::DeviceToLogical(float * x, float * y) const {
	if (fDevice) fDevice->DeviceToLogical(x, y);
	dbgStream << "Device::DeviceToLogical (" << *x << ", " << *y << ")" << endl;
}
inline float DebugDevice::GetXScale() const {
	float xs = fDevice->GetXScale();
	dbgStream << "Device::GetXScale -> " << xs << endl;
	return xs;
}
inline float DebugDevice::GetYScale() const {
	float ys = fDevice->GetYScale();
	dbgStream << "Device::GetYScale -> " << ys << endl;
	return ys;
}
inline float DebugDevice::GetXOrigin() const {
	float x = fDevice->GetXOrigin();
	dbgStream << "Device::GetXOrigin -> " << x <<  endl;
	return x;
}
inline float DebugDevice::GetYOrigin() const {
	float y = fDevice->GetYOrigin();
	dbgStream << "Device::GetYOrigin -> " << y << endl;
	return y;
}
inline void DebugDevice::NotifySize( int inWidth, int inHeight ) {
	dbgStream << "Device::NotifySize (" << inWidth << ", " << inHeight << ")" << endl;
	if (fDevice) fDevice->NotifySize (inWidth, inHeight);
}
inline int DebugDevice::GetWidth() const {
	int width = fDevice->GetWidth();
	dbgStream << "Device::GetWidth -> " << width << endl;
	return width;
}
inline int DebugDevice::GetHeight() const {
	int height = fDevice->GetHeight();
	dbgStream << "Device::GetWidth -> " << height << endl;
	return height;
}

// --------------------------------------------------------------
inline void DebugDevice::DrawMusicSymbol(float x, float y, unsigned int inSymbolID) {
	dbgStream << "Device::DrawMusicSymbol (" << x << ", " << y << ", " << inSymbolID << ")" << endl;
	if (fDevice) fDevice->DrawMusicSymbol(x, y, inSymbolID);
}
inline void DebugDevice::DrawString(float x, float y, const char * s, int inCharCount) {
	dbgStream << "Device::DrawString (" << x << ", " << y << ", " << s  << ", "  << inCharCount << ")" << endl;
	if (fDevice) fDevice->DrawString(x, y, s, inCharCount);
}
inline void DebugDevice::SetFontColor(const VGColor & inColor) {
	dbgStream << "Device::SetFontColor " << inColor << endl;
	if (fDevice) fDevice->SetFontColor(inColor);
}
inline VGColor DebugDevice::GetFontColor() const {
	dbgStream << "Device::GetFontColor" << endl;
	return (fDevice) ? fDevice->GetFontColor() : VGColor();
}
inline void DebugDevice::SetFontBackgroundColor(const VGColor & inColor) {
	dbgStream << "Device::SetFontBackgroundColor" << inColor << endl;
	if (fDevice) fDevice->SetFontBackgroundColor(inColor);
}
inline VGColor DebugDevice::GetFontBackgroundColor() const {
	dbgStream << "Device::GetFontBackgroundColor" << endl;
	return (fDevice) ? fDevice->GetFontBackgroundColor() : VGColor();
}
inline void DebugDevice::SetFontAlign(unsigned int inAlign) {
	dbgStream << "Device::SetFontAlign " << inAlign << endl;
	if (fDevice) fDevice->SetFontAlign(inAlign);
}
inline unsigned int DebugDevice::GetFontAlign() const {
	dbgStream << "Device::GetFontAlign" << endl;
	return (fDevice) ? fDevice->GetFontAlign() : 0;
}

// --------------------------------------------------------------
inline void DebugDevice::SetDPITag(float inDPI) {
	dbgStream << "Device::SetDPITag" << endl;
	if (fDevice) fDevice->SetDPITag(inDPI);
}
inline float DebugDevice::GetDPITag() const {
	dbgStream << "Device::GetDPITag" << endl;
	return (fDevice) ? fDevice->GetDPITag() : 0.f;
}
inline  void* DebugDevice::GetBitMapPixels() {
	dbgStream << "Device::GetBitMapPixels" << endl;
	return (fDevice) ? fDevice->GetBitMapPixels() : NULL;
}
inline void DebugDevice::ReleaseBitMapPixels() {
	dbgStream << "Device::ReleaseBitMapPixels" << endl;
	if (fDevice) fDevice->ReleaseBitMapPixels();
}
inline VGSystem * DebugDevice::getVGSystem() const {
	dbgStream << "Device::getVGSystem" << endl;
	return (fDevice) ? fDevice->getVGSystem() : NULL;
}

#endif
