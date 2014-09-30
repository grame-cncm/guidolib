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

#include "SVGDevice.h"
#include "VGFont.h"
#include "GUIDOEngine.h"
#include "base64.h"

using namespace std;

//______________________________________________________________________________
ostream& operator<< (ostream& os, const svgendl& endl)
{
    endl.print(os);
    return os;
}

//______________________________________________________________________________
void svgendl::print(std::ostream& os) const { 
	int i = fIndent;
    os << std::endl;
    while (i-- > 0)  os << "    ";
}

//______________________________________________________________________________
// SVGDevice
//______________________________________________________________________________
SVGDevice::SVGDevice(std::ostream& outstream, SVGSystem* system, const char* guidofontfile) : 
	fSystem (system), fGuidoFontFile(guidofontfile),
	fStream(outstream), 
	fWidth(1000), fHeight(1000),
	fMusicFont(0), fTextFont(0), fOpMode(kUnknown),
	fXScale(1), fYScale(1), fXOrigin(0), fYOrigin(0), fXPos(0), fYPos(0),
	fFontAlign(kAlignBase), fDPI(0),
	fPushedPen(false), fPushedPenColor(false), fPushedPenWidth(false), fPushedFill(false), fScaled(false), fOffset(false),
    fCurrFont (kNoFont), fCurrFontProperties (VGFont::kFontNone),
	fPendingStrokeColor(0),
	fBeginDone(false)
{
    fTagTypesVector = new std::vector<TagType>();
}

SVGDevice::~SVGDevice() 
{
	delete fPendingStrokeColor;
}

//______________________________________________________________________________
// Guido font output
//______________________________________________________________________________
void SVGDevice::getsvgfont (const char* ptr, string& str) const
{
	const char* fontstart = "<font";
	const char* fontend = "</font>";
	size_t len = strlen (fontstart);
	bool infont = false;
	while (*ptr) {
		if (!infont) {
			if (*ptr == '<') {
				if (strncmp(ptr, fontstart, len) == 0) {
					while (len-- && *ptr) ptr++;
					if ((*ptr == ' ') || (*ptr ==  '	')) {
						infont = true;
						str += fontstart;
						str += ' ';
						len = strlen(fontend);
					}
				}
			}
			ptr++;
		}
		else {
			if (*ptr == '<') {
				if (strncmp(ptr, fontend, len) == 0) {
					str += fontend;
					break;
				}
			}
			str += *ptr++;
		}
	}
}

void SVGDevice::printFont(std::ostream& out, const char* file) const
{
	ifstream is (file);
	if (is.is_open()) {
		is.seekg (0, ios::end);
		int length = int(is.tellg());
		is.seekg (0, ios::beg);
		char * buffer = new char [++length];
		// read data as a block:
		is.read (buffer,length);
		buffer[length-1] = 0;
		string str;
		getsvgfont (buffer, str);
		delete [] buffer;
		if (str.size())
			out << "<defs>\n" << str << "\n</defs>" << endl;
	}
	else cerr << "SVGDevice: can't open svg guido font " << file << endl;
}

//______________________________________________________________________________
// - Drawing services
//______________________________________________________________________________
bool SVGDevice::BeginDraw()
{
	fStream << "<?xml version=\"1.0\"?>" << fEndl;
	fStream << "<svg viewBox=\"0 0 " << fWidth << " " << fHeight << "\" xmlns=\"http://www.w3.org/2000/svg\"  version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">";
	fEndl++;
	fStream << fEndl << "<desc> SVG file generated using the GuidoEngine version " << GuidoGetVersionStr() << "</desc>";
	if (fGuidoFontFile) printFont (fStream, fGuidoFontFile);
	fBeginDone = true;
	if (fPendingStrokeColor) {
		SelectPenColor (*fPendingStrokeColor);
		delete fPendingStrokeColor;
		fPendingStrokeColor = 0;
	}
	return true;
}

void SVGDevice::EndDraw()
{
	if (fPushedPen) closegroup();
	if (fPushedPenColor) closegroup();
	if (fPushedPenWidth) closegroup();
	if (fPushedFill) closegroup();
    if (fScaled) closegroup();
	if (fOffset) closegroup();
	if (fCurrFont) closegroup();
	fPushedPen = false;
	fPushedFill = false;
    fScaled = false;
	fOffset = false;
	fEndl--; fStream << fEndl << "</svg>" << fEndl;
}

void SVGDevice::InvalidateRect( float left, float top, float right, float bottom ) {}
void SVGDevice::checkfont()	
{
	if (fCurrFont) {
		closegroup();
		fCurrFont = 0;
	}
}

//______________________________________________________________________________
// - Standard graphic primitives
//______________________________________________________________________________
void SVGDevice::MoveTo( float x, float y )	{ fXPos = x; fYPos = y; }
void SVGDevice::LineTo( float x, float y )	{ Line(fXPos, fYPos, x, y); }
void SVGDevice::Line( float x1, float y1, float x2, float y2 )
{
	fStream << fEndl << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 <<	"\" y2=\"" << y2 << "\" />";
}

void SVGDevice::Frame( float left, float top, float right, float bottom )
{
	fStream << fEndl << "<rect x=\"" << left << "\" y=\"" << top 
			<< "\" width=\"" << (right - left) <<	"\" height=\"" << (bottom - top) 
			<< "\" fill=\"none\" />";
}

void SVGDevice::Arc( float left, float top, float right, float bottom, float startX, float startY, float endX, float endY )
{
}

//______________________________________________________________________________
// - Filled surfaces
//______________________________________________________________________________
void SVGDevice::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 )
{
	float x[3], y[3];
	x[0] = x1;
	x[1] = x2;
	x[2] = x3;
	y[0] = y1;
	y[1] = y2;
	y[2] = y3;
	Polygon (x, y, 3);
}

void SVGDevice::Polygon( const float * xCoords, const float * yCoords, int count )
{
	fStream << fEndl << "<polygon points=\"";
	for (int i=0; i < count; i++) 
		fStream << xCoords[i] << " " << yCoords[i] << "  ";
	fStream << "\" />";
}

void SVGDevice::Rectangle( float left,  float top, float right, float bottom )
{
	fStream << fEndl << "<rect x=\"" << left << "\" y=\"" << top 
			<< "\" width=\"" << (right - left) <<	"\" height=\"" << (bottom - top) 
			<< "\" />";
}

//______________________________________________________________________________
void SVGDevice::print (std::ostream& out, const VGColor& color) const
{
	char buff[10];
	sprintf (buff, "#%02x%02x%02x",  int(color.mRed), int(color.mGreen), int(color.mBlue));
	out << buff;
}

float SVGDevice::alpha2float (const VGColor& color)	const	{ return (float)color.mAlpha / ALPHA_OPAQUE; }
void SVGDevice::closegroup()								{ fEndl--; fStream << fEndl << "</g>"; }

//______________________________________________________________________________
// - Font services
//______________________________________________________________________________
void			SVGDevice::SetMusicFont( const VGFont * font )	{ fMusicFont = font; }
const VGFont *	SVGDevice::GetMusicFont() const					{ return fMusicFont; }
void			SVGDevice::SetTextFont( const VGFont * font )	{ fTextFont = font; }
const VGFont *	SVGDevice::GetTextFont() const					{ return fTextFont; }

//______________________________________________________________________________
// - Pen & brush services
//______________________________________________________________________________
void SVGDevice::SelectPen( const VGColor & c, float w )
{
	if (fPushedPen) closegroup();
	PushPen (c, w);
	fPushedPen = true;
}
void SVGDevice::SelectFillColor( const VGColor & c )
{
	if (fPushedFill) closegroup();
	PushFillColor (c);
	fPushedFill = true;
}

void SVGDevice::PushPen( const VGColor & color, float width )
{
	fStream << fEndl << "<g style=\"stroke:";
	print(fStream, color);
	fStream << "; stroke-opacity:" << alpha2float(color) << "; stroke-width:" << width << "\">"; 
	fEndl++ ;

    fTagTypesVector->push_back(penTag);
}
void SVGDevice::PushFillColor( const VGColor & color )
{
	fStream << fEndl << "<g style=\"fill:";
	print(fStream, color);
	fStream << "; fill-opacity:" << alpha2float(color) << ";\">"; 
	fEndl++ ;

    fTagTypesVector->push_back(fillColorTag);
}

void SVGDevice::PopPen() {
    checkTagsOrder(penTag);

    closegroup();
}

void SVGDevice::SetRasterOpMode( VRasterOpMode mode)			{ fOpMode = mode; }
VGDevice::VRasterOpMode SVGDevice::GetRasterOpMode() const		{ return fOpMode; }


//______________________________________________________________________________
// - Bitmap services (bit-block copy methods)
//______________________________________________________________________________
void SVGDevice::putbase64 (VGDevice* dev) const
{
	int length = 0;
	const char * data;
	const char* mimetype = dev->GetImageData (data, length);
	if (mimetype && length) {
		fStream << fEndl
			<< "<image width=\"" << dev->GetWidth() << "\" height=\"" << dev->GetHeight() << "\" "
			<< "xlink:href=\"data:" << mimetype << ";base64," << base64_encode((const unsigned char*)data, length) << "\"/>";
		ReleaseImageData (mimetype);
	}
}

inline float alpha2svgalpha(float a) { return (a==-1.f) ? 1.f : a; }

bool SVGDevice::CopyPixels( VGDevice* pSrcDC, float alpha)
{
	fStream << fEndl << "<g opacity=\"" << alpha2svgalpha(alpha) << "\">";
	fEndl++ ;
	putbase64( pSrcDC);
	fEndl--;
	fStream << fEndl << "</g>";
	return true;
}

bool SVGDevice::CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha)
{
	cerr << "SVGDevice::CopyPixels: source subset unsupported" << endl;
	return true;
}

bool SVGDevice::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha)
{
	float scalex = float(dstWidth) / pSrcDC->GetWidth();
	float scaley = float(dstHeight) / pSrcDC->GetHeight();
	
	fStream << fEndl << "<g transform=\"translate(" << xDest << ", " << yDest << ")"
			<< " scale(" << scalex << ", " << scaley
			<< ")\" opacity=\"" << alpha2svgalpha(alpha) << "\">";
	fEndl++ ;
	putbase64( pSrcDC);
	fEndl--;
	fStream << fEndl << "</g>";
	return true;
}

bool SVGDevice::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha)
{
	cerr << "SVGDevice::CopyPixels: source subset unsupported" << endl;
	return true;
}

//______________________________________________________________________________
// - Coordinate services
//______________________________________________________________________________
void SVGDevice::SetScale( float x, float y )
{
    if ((x > (1 / fXScale) - 0.01) && (x < (1 / fXScale) + 0.01)       // Ugly but necessary to avoid floating approximations
        && (y > (1 / fYScale) - 0.01) && (y < (1 / fYScale) + 0.01))
    {
        checkTagsOrder(scaleTag);
        closegroup();
    }
    else
    {
        fScaled = true;
        fXScale = x; fYScale = y;
        fStream << fEndl << "<g transform=\"scale(" << x << ", " << y << ")\">";
        fEndl++;

        fTagTypesVector->push_back(scaleTag);
    }
}

void SVGDevice::SetOrigin( float x, float y )
{ 
	checkfont();
	if (fOffset) closegroup();
	fStream << fEndl << "<g transform=\"translate(" << x << ", " << y << ")\">";
	fXOrigin = x; fYOrigin = y;
	fEndl++ ;
	fOffset = true;

    fTagTypesVector->push_back(originTag);
}

void SVGDevice::OffsetOrigin( float x, float y )	
{ 
	SetOrigin( fXOrigin + x, fYOrigin += y); 
}

void SVGDevice::LogicalToDevice( float * x, float * y ) const {}
void SVGDevice::DeviceToLogical( float * x, float * y ) const {}

float SVGDevice::GetXScale() const		{ return fXScale; }
float SVGDevice::GetYScale() const		{ return fYScale; }
float SVGDevice::GetXOrigin() const		{ return fXOrigin; }
float SVGDevice::GetYOrigin() const		{ return fYOrigin; }

void SVGDevice::NotifySize( int w, int h )	{ fWidth=w; fHeight=h;  }
int	SVGDevice::GetWidth() const				{ return fWidth; }
int SVGDevice::GetHeight() const			{ return fHeight; }


//______________________________________________________________________________
// - Text and music symbols services
//______________________________________________________________________________
const char* SVGDevice::align2str (int align) const
{
	if (align & kAlignRight)	return "text-anchor=\"end\"";
	if (align & kAlignCenter)	return "text-anchor=\"middle\"";
//	return "start"; // default value is "start"
	return "";
}
const char* SVGDevice::baseline2str (int align) const
{
	if (align & kAlignTop)	return " alignment-baseline=\"hanging\"";
//	return "baseline"; // default value is "baseline"
	return "";
}

//______________________________________________________________________________
void SVGDevice::selectfont (int fonttype)
{
    if (fCurrFont == fonttype && fonttype == 1) // (fonttype == 1) condition added
        return;                                 // to avoid risk of writing several
                                                // texts in the same text font
	
    if (fCurrFont) closegroup();

	const VGFont * font = 0;
	switch (fonttype) {
		case kMusicFont:	font = fMusicFont; break;
		case kTextFont:		font = fTextFont; break;
	}
	if (font) {
		fStream << fEndl << "<g font-family=\"" << font->GetName()  << "\"";  // << "\" font-size=\"" << font->GetSize() << "\">"; 
        
        int fontProperties = font->GetProperties();

        switch(fontProperties)
        {
        case VGFont::kFontBold:
            fStream << " font-weight=\"bold\"";
            break;
        case VGFont::kFontItalic:
            fStream << " font-style=\"italic\"";
            break;
        case VGFont::kFontBold + VGFont::kFontItalic:
            fStream << " font-weight=\"bold\" font-style=\"italic\"";
            break;
        case VGFont::kFontUnderline:
            fStream << " text-decoration=\"underline\"";
            break;
        default:
            break;
        }

        fStream << ">";

        fEndl++ ;
		fCurrFont = fonttype;

        fTagTypesVector->push_back(fontTag);
	}
}

void SVGDevice::DrawMusicSymbol(float x, float y, unsigned int inSymbolID )
{
	selectfont (kMusicFont);
	fStream << fEndl << "<text x=\"" << x << "\" y=\"" << y  << "\" " << align2str(fFontAlign) 
	<< baseline2str(fFontAlign) << " font-size=\"" << fMusicFont->GetSize() 
	<< "\" stroke=\""; 
	print(fStream, fFontColor);
	fStream << "\" fill=\""; 
	print(fStream, fFontColor);
	fStream << "\">&#" << inSymbolID << ";</text>"; 
}

void SVGDevice::DrawString( float x, float y, const char * s, int inCharCount )
{
	selectfont (kTextFont);
	fStream << fEndl << "<text x=\"" << x << "\" y=\"" << y << "\" " << align2str(fFontAlign) 
	<< baseline2str(fFontAlign) << " font-size=\"" << fTextFont->GetSize() 
	<< "\" stroke=\""; 
	print(fStream, fFontColor);
	fStream << "\" fill=\""; 
	print(fStream, fFontColor);
	fStream << "\">";
	fStream << fEndl++; 
	for (int i=0; i<inCharCount; i++) {
		if (s[i] > 0)
			fStream << s[i];
		else 
			fStream << "&#" << int((unsigned char)s[i]) << ";"; 
	}
	fEndl--;
	fStream << fEndl << "</text>"; 
}

void	SVGDevice::SetFontColor( const VGColor & color )	{ fFontColor = color; }
VGColor SVGDevice::GetFontColor() const						{ return fFontColor; }
void	SVGDevice::SetFontBackgroundColor( const VGColor & inColor )	{fFontBackgroundColor = inColor; }
VGColor SVGDevice::GetFontBackgroundColor() const						{ return fFontBackgroundColor; }
void	SVGDevice::SetFontAlign( unsigned int inAlign )	{ fFontAlign = inAlign; }
unsigned int SVGDevice::GetFontAlign() const			{ return fFontAlign; }

//______________________________________________________________________________
// - Printer informations services - useless in svg context
//______________________________________________________________________________
void SVGDevice::SetDPITag( float inDPI )		{ fDPI = inDPI; }
float SVGDevice::GetDPITag() const				{ return fDPI; }

//______________________________________________________________________________
// - VGDevice extension
//______________________________________________________________________________
void SVGDevice::SelectPenColor( const VGColor & color)
{
	if (fBeginDone) {
		if (fPushedPenColor) closegroup();
		PushPenColor (color);
		fPushedPenColor = true;
	}
	else fPendingStrokeColor = new VGColor(color);
}

void SVGDevice::SelectPenWidth( float width)
{
	if (fPushedPenWidth) closegroup();
	PushPenWidth (width);
	fPushedPenWidth = true;
}

void SVGDevice::PushPenColor( const VGColor & color)
{
	fStream << fEndl << "<g style=\"stroke:";
	print(fStream, color);
	fStream << "; stroke-opacity:" << alpha2float(color) << "\">"; 
	fEndl++;

    fTagTypesVector->push_back(penColorTag);
}

void SVGDevice::PushPenWidth( float width)
{
	fStream << fEndl << "<g style=\"stroke-width:" << width << "\">"; 
	fEndl++ ;

    fTagTypesVector->push_back(penWidthTag);
}

void SVGDevice::PopPenColor() {
    checkTagsOrder(penColorTag);
    closegroup();
}

void SVGDevice::PopPenWidth() {
    checkTagsOrder(penWidthTag);
    closegroup();
}

void SVGDevice::PopFillColor() {
    checkTagsOrder(fillColorTag);
    closegroup();
}

void SVGDevice::checkTagsOrder(TagType tagToClose) {
    if (!fTagTypesVector->empty() && fTagTypesVector->back() == tagToClose) {
        if (tagToClose == scaleTag) {
            fXScale = 1;
            fYScale = 1;
        }

        fTagTypesVector->pop_back();
    }
    else if (!fTagTypesVector->empty() && fTagTypesVector->back() == fontTag) {
        checkfont();

        fTagTypesVector->pop_back();

        if (!fTagTypesVector->empty())
            fTagTypesVector->pop_back();
    }
    else {
        // error ?
    }
}