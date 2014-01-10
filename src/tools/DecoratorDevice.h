#ifndef __DecoratorDevice__
#define __DecoratorDevice__

/*
  GUIDO Library
  Copyright (C) 2003, 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "VGDevice.h"

// --------------------------------------------------------------
class DecoratorDevice : public VGDevice
{
	protected:
		bool					fDevOwner;
		VGDevice *				fDevice;
		virtual void *			GetNativeContext() const;

	public:
								DecoratorDevice(VGDevice * dev, bool owner=true) : fDevOwner(owner), fDevice(dev)  {}
		virtual					~DecoratorDevice()	{ if (fDevOwner) delete fDevice; }

		virtual bool			IsValid() const;

		// - Drawing services ------------------------------------------------
		virtual bool			BeginDraw();
		virtual void			EndDraw();
		virtual void			InvalidateRect( float left, float top, 
									   float right, float bottom );

					// - Standard graphic primitives -------------------------
		virtual void			MoveTo( float x, float y );
		virtual void			LineTo( float x, float y );
		virtual void			Line( float x1, float y1, 
									  float x2, float y2 );
		virtual void			Frame( float left, float top, 
									   float right, float bottom );	
		virtual void			Arc( float left,   float top, 
									 float right,  float bottom, 
									 float startX, float startY, 
									 float endX,   float endY );

					// - Filled surfaces --------------------------------------
		virtual	void			Triangle( float x1, float y1, 
										  float x2, float y2, 
										  float x3, float y3 );	
		virtual	void			Polygon( const float * xCoords, 
										 const float * yCoords, int count );	
		virtual void			Rectangle( float left,  float top, 
										   float right, float bottom );

		// - Font services ---------------------------------------------------
		virtual	void			SetMusicFont( const VGFont * font );
		virtual	const VGFont *	GetMusicFont() const;
		virtual	void			SetTextFont( const VGFont * font );
		virtual	const VGFont *	GetTextFont() const;
	  


		// - Pen & brush services --------------------------------------------
		virtual	void			SelectPen( const VGColor & inColor, float witdh ); 
		virtual	void			SelectFillColor( const VGColor & c );		
		virtual	void			PushPen( const VGColor & inColor, float inWidth );
		virtual	void			PopPen();
		virtual	void			PushFillColor( const VGColor & inColor );
		virtual	void			PopFillColor();
		virtual	void			SetRasterOpMode( VRasterOpMode ROpMode);
		virtual	VRasterOpMode	GetRasterOpMode() const;


		// - Bitmap services (bit-block copy methods) --------------------------
		virtual bool			CopyPixels( VGDevice* pSrcDC, float alpha = -1.0); 
		virtual bool			CopyPixels( int xDest, int yDest,
											VGDevice* pSrcDC, 
											int xSrc, int ySrc, 
											int nSrcWidth, int nSrcHeight, float alpha = -1.0);	
		virtual bool			CopyPixels( int xDest, int yDest, 
											int dstWidth, int dstHeight, 
											VGDevice* pSrcDC, float alpha = -1.0);	
		virtual bool			CopyPixels( int xDest, int yDest, 
											int dstWidth, int dstHeight, 
											VGDevice* pSrcDC, 
											int xSrc, int ySrc, 
											int nSrcWidth, int nSrcHeight, float alpha = -1.0);

		// - Coordinate services ------------------------------------------------
		virtual	void			SetScale( float x, float y );
		virtual	void			SetOrigin( float x, float y );
		virtual	void			OffsetOrigin( float x, float y );
		virtual	void			LogicalToDevice( float * x, float * y ) const;
		virtual	void			DeviceToLogical( float * x, float * y ) const;	
		virtual float			GetXScale() const;
		virtual	float			GetYScale() const;
		virtual	float			GetXOrigin() const;
		virtual	float			GetYOrigin() const;
		virtual	void			NotifySize( int inWidth, int inHeight );
		virtual	int				GetWidth() const;
		virtual	int				GetHeight() const;


		// - Text and music symbols services -------------------------------------
		virtual void			DrawMusicSymbol(float x, float y, 
												unsigned int inSymbolID );
		virtual	void			DrawString( float x, float y, 
										    const char * s, 
										    int inCharCount ); 
		virtual	void			SetFontColor( const VGColor & inColor );
		virtual	VGColor			GetFontColor() const;
		virtual	void			SetFontBackgroundColor( const VGColor & inColor );
		virtual	VGColor			GetFontBackgroundColor() const;
		virtual	void			SetFontAlign( unsigned int inAlign );
		virtual	unsigned int	GetFontAlign() const;

		// - Printer informations services ----------------------------------------
		virtual	void			SetDPITag( float inDPI );
		virtual	float			GetDPITag() const;

		virtual void*			GetBitMapPixels();
		virtual void			ReleaseBitMapPixels();
		virtual const char*		GetImageData(const char* & outDataPtr, int& outLength);
		virtual void			ReleaseImageData(const char *) const;
	
		virtual	VGSystem *		getVGSystem() const;

		// - Data export services ----------------------------------------
//		virtual	void			ExportToFile( const char * inFilePath );
		
		// - VGDevice extension
		virtual	void			SelectPenColor(const VGColor & inColor);
		virtual	void			SelectPenWidth(float witdh);
		virtual	void			PushPenColor( const VGColor & inColor);
		virtual	void			PopPenColor();
		virtual	void			PushPenWidth( float width);
		virtual	void			PopPenWidth();
};

// --------------------------------------------------------------
inline bool DecoratorDevice::IsValid() const {
	return fDevice->IsValid ();
}

// --------------------------------------------------------------
inline bool DecoratorDevice::BeginDraw() {
	return fDevice->BeginDraw();
}
inline void DecoratorDevice::EndDraw() {
	fDevice->EndDraw();
}
inline void DecoratorDevice::InvalidateRect(float left, float top, float right, float bottom) {
	fDevice->InvalidateRect(left, top, right, bottom);
}
// --------------------------------------------------------------
inline void DecoratorDevice::MoveTo( float x, float y ) {
	fDevice->MoveTo (x, y);
}
inline void DecoratorDevice::LineTo( float x, float y ) {
	fDevice->LineTo (x, y);
}
inline void DecoratorDevice::Line( float x1, float y1, float x2, float y2 ) {
	fDevice->Line (x1, y1, x2, y2);
}
inline void DecoratorDevice::Frame( float left, float top, float right, float bottom ) {
	fDevice->Frame (left, top, right, bottom);
}
inline void DecoratorDevice::Arc( float left, float top, float right, float bottom, float startX, float startY, float endX, float endY ) {
	fDevice->Arc (left, top, right, bottom, startX, startY, endX, endY);
}
inline void DecoratorDevice::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 ) {
	fDevice->Triangle (x1, y1, x2, y2, x3, y3);
}
inline void DecoratorDevice::Polygon( const float * x, const float * y, int count ) {
	fDevice->Polygon (x, y, count);
}
inline void DecoratorDevice::Rectangle( float left, float top, float right, float bottom ) {
	fDevice->Rectangle (left, top, right, bottom);
}

// --------------------------------------------------------------
inline void DecoratorDevice::SetMusicFont(const VGFont * font) {
	fDevice->SetMusicFont(font);
}
inline const VGFont* DecoratorDevice::GetMusicFont() const {
	return fDevice->GetMusicFont();
}
inline void DecoratorDevice::SetTextFont(const VGFont * font) {
	fDevice->SetTextFont(font);
}
inline const VGFont* DecoratorDevice::GetTextFont() const {
	return fDevice->GetTextFont();
}

// --------------------------------------------------------------
inline void DecoratorDevice::SelectPen(const VGColor & inColor, float witdh) {
	fDevice->SelectPen(inColor, witdh);
}
inline void DecoratorDevice::SelectPenColor(const VGColor & inColor) {
	fDevice->SelectPenColor(inColor);
}
inline void DecoratorDevice::SelectPenWidth(float witdh) {
	fDevice->SelectPenWidth(witdh);
}
inline void DecoratorDevice::PushPenColor(const VGColor & inColor) {
	fDevice->PushPenColor(inColor);
}
inline void DecoratorDevice::PushPenWidth(float witdh) {
	fDevice->PushPenWidth(witdh);
}
inline void DecoratorDevice::PopPenColor() {
	fDevice->PopPenColor();
}
inline void DecoratorDevice::PopPenWidth() {
	fDevice->PopPenWidth();
}

inline void DecoratorDevice::SelectFillColor(const VGColor & inColor) {
	fDevice->SelectFillColor(inColor);
}
inline void DecoratorDevice::PushPen(const VGColor & inColor, float witdh) {
	fDevice->PushPen(inColor, witdh);
}
inline void DecoratorDevice::PopPen() {
	fDevice->PopPen();
}
inline void DecoratorDevice::PushFillColor(const VGColor & inColor) {
	fDevice->PushFillColor(inColor);
}
inline void DecoratorDevice::PopFillColor() {
	fDevice->PopFillColor();
}
inline void DecoratorDevice::SetRasterOpMode(VGDevice::VRasterOpMode ROpMode) {
	fDevice->SetRasterOpMode(ROpMode);
}
inline VGDevice::VRasterOpMode DecoratorDevice::GetRasterOpMode() const{
	return fDevice->GetRasterOpMode();
}

// --------------------------------------------------------------
inline bool DecoratorDevice::CopyPixels(VGDevice* pSrcDC, float alpha) {
	return fDevice->CopyPixels(pSrcDC, alpha);
}
inline bool DecoratorDevice::CopyPixels(int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc, 
										int nSrcWidth, int nSrcHeight, float alpha) {
	return fDevice->CopyPixels(xDest, yDest, pSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, alpha);
}
inline bool DecoratorDevice::CopyPixels(int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha) {
	return fDevice->CopyPixels(xDest, yDest, dstWidth, dstHeight, pSrcDC, alpha);
}

inline bool DecoratorDevice::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, 
										int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha) {
	return fDevice->CopyPixels(xDest, yDest, dstWidth, dstHeight, pSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, alpha);
}

// --------------------------------------------------------------
inline void DecoratorDevice::SetScale(float x, float y) {
	fDevice->SetScale(x, y);
}
inline void DecoratorDevice::SetOrigin(float x, float y) {
	fDevice->SetOrigin(x, y);
}
inline void DecoratorDevice::OffsetOrigin(float x, float y) {
	fDevice->OffsetOrigin(x, y);
}
inline void DecoratorDevice::LogicalToDevice(float * x, float * y) const {
	fDevice->LogicalToDevice(x, y);
}
inline void DecoratorDevice::DeviceToLogical(float * x, float * y) const {
	fDevice->DeviceToLogical(x, y);
}
inline float DecoratorDevice::GetXScale() const {
	return fDevice->GetXScale();
}
inline float DecoratorDevice::GetYScale() const {
	return fDevice->GetYScale();
}
inline float DecoratorDevice::GetXOrigin() const {
	return fDevice->GetXOrigin();
}
inline float DecoratorDevice::GetYOrigin() const {
	return fDevice->GetYOrigin();
}
inline void DecoratorDevice::NotifySize( int inWidth, int inHeight ) {
	fDevice->NotifySize (inWidth, inHeight);
}
inline int DecoratorDevice::GetWidth() const {
	return fDevice->GetWidth();
}
inline int DecoratorDevice::GetHeight() const {
	return fDevice->GetHeight();
}

// --------------------------------------------------------------
inline void DecoratorDevice::DrawMusicSymbol(float x, float y, unsigned int inSymbolID) {
	fDevice->DrawMusicSymbol(x, y, inSymbolID);
}
inline void DecoratorDevice::DrawString(float x, float y, const char * s, int inCharCount) {
	fDevice->DrawString(x, y, s, inCharCount);
}
inline void DecoratorDevice::SetFontColor(const VGColor & inColor) {
	fDevice->SetFontColor(inColor);
}
inline VGColor DecoratorDevice::GetFontColor() const {
	return fDevice->GetFontColor();
}
inline void DecoratorDevice::SetFontBackgroundColor(const VGColor & inColor) {
	fDevice->SetFontBackgroundColor(inColor);
}
inline VGColor DecoratorDevice::GetFontBackgroundColor() const {
	return fDevice->GetFontBackgroundColor();
}
inline void DecoratorDevice::SetFontAlign(unsigned int inAlign) {
	fDevice->SetFontAlign(inAlign);
}
inline unsigned int DecoratorDevice::GetFontAlign() const {
	return fDevice->GetFontAlign();
}

// --------------------------------------------------------------
inline void DecoratorDevice::SetDPITag(float inDPI) {
	fDevice->SetDPITag(inDPI);
}
inline float DecoratorDevice::GetDPITag() const {
	return fDevice->GetDPITag();
}
inline  void* DecoratorDevice::GetBitMapPixels() {
	return fDevice->GetBitMapPixels();
}
inline void DecoratorDevice::ReleaseBitMapPixels() {
	fDevice->ReleaseBitMapPixels();
}
inline const char* DecoratorDevice::GetImageData(const char* & outDataPtr, int& outLength) {
	return fDevice->GetImageData(outDataPtr, outLength);
}
inline void DecoratorDevice::ReleaseImageData(const char * ptr) const {
	fDevice->ReleaseImageData(ptr);
}

inline VGSystem * DecoratorDevice::getVGSystem() const {
	return fDevice->getVGSystem();
}

// --------------------------------------------------------------
/*inline void DecoratorDevice::ExportToFile( const char * inFilePath ) {
	fDevice->ExportToFile(inFilePath);
} */

// --------------------------------------------------------------
inline void* DecoratorDevice::GetNativeContext() const {
	return fDevice->GetNativeContext();
}

#endif
