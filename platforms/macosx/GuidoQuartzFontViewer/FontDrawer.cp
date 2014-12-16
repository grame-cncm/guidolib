
#include "FontDrawer.h"
#include "GUIDOEngine/GSystemOSX.h"
#include "GUIDOEngine/VGDevice.h"
#include "GUIDOEngine/VGFont.h"


// -----------------------------------------------------------------------------
FontDrawer::~FontDrawer()
{
}

// -----------------------------------------------------------------------------
void FontDrawer::init(CGContextRef context)
{
	::CGContextScaleCTM (context, 1.0f, -1.0f);

	GSystemOSX * sys = new GSystemOSX(context, 0);
	fDevice = sys->CreateDisplayDevice();
	if (fDevice) {
//		const VGFont* font = sys->CreateVGFont("Arial", 10, VGFont::kFontNone);
//		const VGFont* music = sys->CreateVGFont("Guido2", 24, VGFont::kFontNone);
//		fDevice->SetTextFont (font);
//		fDevice->SetMusicFont (music);
	}
}

// -----------------------------------------------------------------------------
void FontDrawer::Draw(CGContextRef context)
{
	if (!fDevice) init(context);
	if (!fDevice) return;

//	fDevice->BeginDraw();
	DrawFont(context);
//	fDevice->EndDraw();
}

// -----------------------------------------------------------------------------
void FontDrawer::Draw(CGContextRef context, const HIRect& r)
{
	if (!fDevice) init(context);
	if (!fDevice) return;

	fDevice->BeginDraw();
	fDevice->Frame (r.origin.x+1, r.origin.y+1, r.origin.x + r.size.width, r.origin.y + r.size.height);
	DrawFont(context);
	fDevice->EndDraw();
}

//#define PRINTEXTEND
#ifdef PRINTEXTEND
#include <math.h>
#include <fstream>
using namespace std;
static int getExtend (CGContextRef context, CGGlyph glyph)
{
	CGPoint oldTextPos = ::CGContextGetTextPosition( context );
	::CGContextSetTextDrawingMode( context, kCGTextInvisible );
	::CGContextShowGlyphs( context, &glyph, 1 );
	CGPoint newTextPos = ::CGContextGetTextPosition( context );
	return round (newTextPos.x - oldTextPos.x);
}

#endif

// -----------------------------------------------------------------------------
void FontDrawer::DrawFont(CGContextRef context)
{
#ifdef PRINTEXTEND
	static bool done = false;
	if (!done) {
		ofstream guido("Guido2.txt");
		ofstream times("Times.txt");
		const float LSPACE = 50;
		::CGContextSaveGState(context);
		::CGContextSelectFont(context, "Guido2", 4*LSPACE, kCGEncodingMacRoman  );
		guido << "MacOS X - Quartz - Guido2 font - size: " << 4*LSPACE << endl; 
		for (CGGlyph i=32; i<255; i++) {
			guido << int(i) << ": " << getExtend(context, i) << endl; 
		}
		::CGContextSelectFont(context, "Times", 1.5*LSPACE, kCGEncodingMacRoman  );
		times << "MacOS X - Quartz - Time font - size: " << 1.5*LSPACE << endl; 
		for (CGGlyph i=32; i<255; i++) {
			times << int(i) << ": " << getExtend(context, i) << endl; 
		}
		::CGContextRestoreGState( context );
		done = true;
	}
#endif
	int x=15, y=35;
	unsigned short n=0;
	for (int i=0; i<19; i++) {
		for (int j=0; j<18; j++) {
			char buff[5];
			sprintf (buff, "%d", n);
			::CGContextSelectFont(context, "Guido2", 24, kCGEncodingMacRoman  );
			::CGContextShowGlyphsAtPoint(context, x, y, &n, 1 );
			::CGContextSelectFont(context, "Arial", 10, kCGEncodingMacRoman  );
			::CGContextShowTextAtPoint(context, x, y+18, buff, strlen(buff) );
			x += 33;
			n++;
		}
		x  = 10;
		y += 45;
	}
}
