/*
	GUIDO Library
	Copyright (C) 2005,2006  Grame

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

/////////////////////////////////////////////////////////////////
///
///	OpenGL implementation of VGDevice. (work in progress).
///		
///	Requirement:
///	- FTGL library:			http://homepages.paradise.net.nz/henryj/code/index.html#FTGL
///	- FreeType library:		http://www.freetype.org
///	- OpenGL:				http://www.opengl.org
///
/////////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

#include "GDeviceGL.h"
#include "GFontGL.h"
#include "FTGL/ftgl.h"

#include "GSystemGL.h" // guido hack - must be removed asap

// - Callback def
#ifdef WIN32
typedef GLvoid (__stdcall *CallBackFunc)(void);
#elif defined(__linux__) || defined(__FreeBSD__)
typedef void (*CallBackFunc)();
#else
//typedef GLvoid (*CallBackFunc)(...);
typedef GLvoid (*CallBackFunc)();
#endif

/////////////////////////////////////////////////////////////////
// - Static methods  --------------------------------------------
/////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
//static void tessCombine(GLdouble coords[3], VERTEX *d[4], GLfloat w[4], VERTEX ** dataout)
static void 
tessCombine(GLdouble coords[3], void *d[4], GLfloat w[4], void ** dataout)
{
//	cout << "tessCombine  w [" << w[0] << ", " << w[1] << ", " << w[2] << ", " << w[3] << "]" << endl;
//	GLdouble *c = new GLdouble[7];
	GLfloat *c = new GLfloat[7];
	c[0] = coords[0];
	c[1] = coords[1];
	c[2] = coords[2];
	c[3] = c[4] = c[5] = c[6] = 0;
	*dataout = c;
}
// ----------------------------------------------------------------------------
static void 
tessError(GLenum errno) 
{ 
	cout << "tessError " << gluErrorString(errno) << endl; 
}


// ----------------------------------------------------------------------------
GDeviceGL::GDeviceGL(VGSystem* sys)				// classic constructor				
//	: fTextColor(0,0,0,255), VGDevice (0, 0)
	: mCurrTextFont(NULL), mCurrMusicFont(NULL),
	  mTextColor(0,0,0,ALPHA_OPAQUE), mRasterMode(kOpCopy),  
	  mScaleX(1), mScaleY(1), mOriginX(0), mOriginY(0), mDPITag(72.0f), 
	  /*mBeginDrawCount(0),*/ mPhysicalWidth(0), mPhysicalHeight(0)

{
	mPenStack.reserve( 16 );
	mBrushStack.reserve( 16 );
	mStateStack.reserve( 16 );
	
	mTextBackColor.Set( 255, 255, 255, ALPHA_OPAQUE ); // transparent

	mTextAlign = 0;

	//----------------------------
	mDrawing = false;

	// init device's members
//	SelectPen(VGColor(0,0,0,255), 1);					// opaque black
//	SelectFillColor(VGColor(0,0,0,255));				// opaque black
//	SetFontColor(VGColor(0,0,0,255));					// opaque black
//	SetFontBackgroundColor( VGColor(255,255,255,0) );	// transparent white
//	SetFontAlign( kAlignLeft | kAlignBase );



	// guido hack - must be removed asap
	mSys = sys; 
}

// ----------------------------------------------------------------------------
GDeviceGL::~GDeviceGL()
{
}

// --------------------------------------------------------------
bool
GDeviceGL::IsValid() const
{ 
	return true; 
}

/////////////////////////////////////////////////////////////////
// - Drawing services -------------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
bool
GDeviceGL::BeginDraw()
{
	
	// general device's state save - was SaveDC()
	GState			s;
	s.pen			= mPen;
	s.fillColor		= mFillColor;
	s.textColor		= mTextColor;
	s.textBackColor = mTextBackColor;
	s.scaleX		= mScaleX;
	s.scaleY		= mScaleY;
	s.originX		= mOriginX;
	s.originY		= mOriginY;
	s.textAlign		= mTextAlign;
	s.currTextFont	= mCurrTextFont;
	s.currMusicFont = mCurrMusicFont;
	s.rasterMode	= mRasterMode;

	mStateStack.push_back( s );

	// GL specific - was DoBeginDraw
	if( mDrawing ) return true;
	mDrawing = true;
	AAliasing();

	glPushMatrix();
	MoveTo( 0, 0 );
#ifndef useglOrtho
	glTranslatef(0, GetHeight(), 0 );
#endif
	TraceRect ();
	glColor3f(0.0, 0.0, 0.0);

	return true;
	
}

// --------------------------------------------------------------
void
GDeviceGL::EndDraw()
{
/*	if( -- mBeginDrawCount == 0 )
	{}
*/
	// general device's state restore - was RestoreDC()
	GState & s = mStateStack.back();
	
	mPen			= s.pen;
	mFillColor		= s.fillColor;
	mTextColor		= s.textColor;	
	mTextBackColor	= s.textBackColor;	
	mScaleX			= s.scaleX;
	mScaleY			= s.scaleY;
	mOriginX		= s.originX;
	mOriginY		= s.originY;
	mTextAlign		= s.textAlign;
	mCurrTextFont	= s.currTextFont;
	mCurrMusicFont	= s.currMusicFont;
	mRasterMode		= s.rasterMode;
	
	mStateStack.pop_back();

	// GL specific - was DoEndDraw
	if( !mDrawing ) return;
	glPopMatrix();
	mDrawing = false;
}
// ----------------------------------------------------------------------------
void			
GDeviceGL::InvalidateRect( float left, float top, float right, float bottom )
{
}

// ----------------------------------------------------------------------------
void 
GDeviceGL::MoveTo( float x, float y )
{
	mCurrPenPosX = getX(x);
	mCurrPenPosY = getY(y);
}

// ----------------------------------------------------------------------------
void 
GDeviceGL::LineTo( float x, float y )
{
	x=getX(x);
	y=getY(y);

	float w;
	glGetFloatv(GL_LINE_WIDTH, &w);
//cout << "GDeviceGL::LineTo (" << x << ", " << y << ")" << endl;
//cout << "GDeviceGL::LineTo line width " << w << endl;
    glBegin(GL_LINES);
		glVertex3f( mCurrPenPosX, mCurrPenPosY, 0 );
		glVertex3f( x, y, 0 );
    glEnd();
	mCurrPenPosX = x;
	mCurrPenPosY = y;
}

// --------------------------------------------------------------
void 
GDeviceGL::Line( float x1, float y1, float x2, float y2 )
{
	MoveTo( x1, y1 );
	LineTo( x2, y2 );
}

// --------------------------------------------------------------
void			
GDeviceGL::Frame( float left, float top, float right, float bottom )
{
    MoveTo (left, top);
    LineTo (right, top);
    LineTo (right, bottom);
    LineTo (left, bottom);
    LineTo (left, top);
}

// --------------------------------------------------------------
void		
GDeviceGL::Arc( float left, float top, float right, float bottom, 
				float startX, float startY, float endX, float endY )
{
}

// --------------------------------------------------------------
void			
GDeviceGL::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 )
{
	const float xCoords [] = { x1, x2, x3 };
	const float yCoords [] = { y1, y2, y3 };

	Polygon( xCoords, yCoords, 3 );
}
// ----------------------------------------------------------------------------
void GDeviceGL::Polygon( const float * xCoords, const float * yCoords, int count )
{
	if( xCoords == 0 ) return;
	if( yCoords == 0 ) return;
	if( count <= 1 ) return;

//cout << "GDeviceGL::Polygon " << count << endl;

	if (count <= 4) {
		glBegin( GL_POLYGON );
		for( int i = 0; i < count; i++ ) {
			glVertex2f(xCoords[i], getY(yCoords[i]) );
		}
		glEnd();
	}
	else {
		GLUtesselator* tess = gluNewTess();
		float * coords = new float[count*3];
		if (!tess) {
			cout << "GDeviceGL::Polygon tesselation new failed" << endl;
			return;
		}
		gluTessCallback(tess, GLU_TESS_BEGIN, (CallBackFunc)glBegin);
		gluTessCallback(tess, GLU_TESS_VERTEX, (CallBackFunc)glVertex2fv);
		gluTessCallback(tess, GLU_TESS_END, (CallBackFunc)glEnd);
#ifndef WIN32
		// combine callback makes gluTessEndPolygon crash on WIN32
		// tesselation method is thus to be checked across the different platforms
		gluTessCallback(tess, GLU_TESS_COMBINE, (CallBackFunc)tessCombine);
#endif
//		gluTessCallback(tess, GLU_TESS_ERROR, (CallBackFunc)tessError);
		// pb here

//		glColor3f(0.0, 0.0, 0.0);
		GLdouble data[3];

		gluTessBeginPolygon (tess, 0);
		gluTessBeginContour (tess);
		for( int i = 0, j=0; i < count; i++, j+=3 ) {
			coords[j]   = data[0] = xCoords[i];
			coords[j+1] = data[1] = getY(yCoords[i]);
			coords[j+2] = data[2] = 0;
			
//			cout << "=> tess vertex [" << data[0] << "," << data[1] << "]" << endl;
			gluTessVertex (tess, data, &coords[j]);
		}
		gluTessEndContour (tess);
		gluTessEndPolygon (tess);

		gluDeleteTess (tess);
	}
}
// ----------------------------------------------------------------------------
void GDeviceGL::Rectangle( float left, float top, float right, float bottom )
{
	glRectf(left, getY(top), right, getY(bottom));
}

// ----------------------------------------------------------------------------
// Return the previous font.
//GFontRef GDeviceGL::SelectFont( GFontRef inObj )
void GDeviceGL::SetMusicFont( const VGFont * inObj )
{
	mCurrMusicFont = inObj;
}
// --------------------------------------------------------------
const VGFont *	
GDeviceGL::GetMusicFont() const				  
{ 
	return mCurrMusicFont; 
}

// ----------------------------------------------------------------------------
//GFontRef GDeviceGL::SelectFont( GFontRef inObj )
void GDeviceGL::SetTextFont( const VGFont * inObj )
{
	mCurrTextFont = inObj;
}
// --------------------------------------------------------------
const VGFont *	
GDeviceGL::GetTextFont() const
{
	return mCurrTextFont; 
}

/////////////////////////////////////////////////////////////////
// - Pen & brush services ---------------------------------------
/////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
//void GDeviceGL::DoSelectPen( const GColor & c, float width )
void GDeviceGL::SelectPen( const VGColor & c, float width )
{
    mPen.Set( c, width );
	
	// GL specific - was DoEndDraw
#ifndef useglOrtho
	width *= mScaleY;
#endif
//cout << "GDeviceGL::DoSelectPen [" << (int)c.red << ", " << (int)c.green << ", " << (int)c.blue << ", " << (int)c.alpha << "] width " << width << endl;
//   	glColor4ub (c.mRed, c.mGreen, c.mBlue, 255-c.mAlpha);
   	glColor4ub (c.mRed, c.mGreen, c.mBlue, c.mAlpha);
	glLineWidth (width);
}

// ----------------------------------------------------------------------------
void GDeviceGL::SelectFillColor( const VGColor & c )
{
	mFillColor.Set( c ); 

	// GL specific - was DoEndDraw
//cout << "GDeviceGL::DoSelectBrush [" << (int)c.red << ", " << (int)c.green << ", " << (int)c.blue << ", " << (int)c.alpha << "]" << endl;
   	glColor4ub (c.mRed, c.mGreen, c.mBlue, c.mAlpha);
}
// --------------------------------------------------------------
// Save the current pen, select the new one
void
GDeviceGL::PushPen( const VGColor & inColor, float inWidth )
{
	mPenStack.push_back( mPen );
	SelectPen( inColor, inWidth );
}

// --------------------------------------------------------------
// Restore the previous pen from the stack
void
GDeviceGL::PopPen()
{
	VGPen & pen = mPenStack.back();
	SelectPen( pen.mColor, pen.mWidth );
	mPenStack.pop_back();
}

// --------------------------------------------------------------
// Save the current brush, select the new one
void
GDeviceGL::PushFillColor( const VGColor & inColor )
{
	mBrushStack.push_back( mFillColor );
	if( inColor != mFillColor )
		SelectFillColor( inColor );
}

// --------------------------------------------------------------
// Restore the previous brush from the stack
void
GDeviceGL::PopFillColor()
{
	VGColor & brush = mBrushStack.back();
	SelectFillColor( brush );
	mBrushStack.pop_back();
}

// --------------------------------------------------------------
void			
GDeviceGL::SetRasterOpMode( VRasterOpMode ROpMode )
{
	mRasterMode = ROpMode; 
} 

/////////////////////////////////////////////////////////////////
// - Bitmap services (bit-block copy methods) -------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
bool			
GDeviceGL::CopyPixels( VGDevice* pSrcDC, float alpha ) 
{
	return false; 
	// TODO : replace this code by correct 
	// platform-dependant implementation
}

// --------------------------------------------------------------
bool			
GDeviceGL::CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, 
					   int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha )	
{ 
	return false; 
	// TODO : replace this code by correct 
	// platform-dependant implementation
}

// --------------------------------------------------------------
bool			
GDeviceGL::CopyPixels( int xDest, int yDest, 
					   int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha ) 
{ 
	return false; 
	// TODO : replace this code by correct 
	// platform-dependant implementation
}

// --------------------------------------------------------------
bool			
GDeviceGL::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha ) 
{ 
	return false; 
	// TODO : replace this code by correct 
	// platform-dependant implementation
}

/////////////////////////////////////////////////////////////////
// - Coordinate services ----------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void			
GDeviceGL::SetScale( float x, float y )
{
	const float prevX = mScaleX;
	const float prevY = mScaleY;
	mScaleX = x;
	mScaleY = y;
	
	// GL specific - was DoSetScale
//cout << "GDeviceGL::DoSetScale [" << mScaleX << "," << mScaleY << "]" << endl;
#ifndef useglOrtho
	glScalef( x/prevX, y/prevY, 1.0f );
#endif
//	glPixelZoom( mScaleX, mScaleY );
}

// --------------------------------------------------------------
// Called only by GRPage
void				
GDeviceGL::SetOrigin( float x, float y )
{
	const float prevX = mOriginX;
	const float prevY = mOriginY;
	mOriginX = x;
	mOriginY = y;

	// GL specific - was DoSetOrigin
	float posx = (x-prevX); //*mScaleX;
	float posy = (y-prevY); //*mScaleY;
//cout << "GDeviceGL::DoSetOrigin translate " << posx << "," << -posy << endl;
	glTranslatef(posx, -posy, 0 );

}

// --------------------------------------------------------------
// Called by: GRPage, GRStaff, GRSystem
void				
GDeviceGL::OffsetOrigin( float x, float y )
{
	const float prevX = mOriginX;
	const float prevY = mOriginY;
	mOriginX += x;
	mOriginY += y;

	// GL specific - was DoSetOrigin
	float posx = (mOriginX-prevX); //*mScaleX;
	float posy = (mOriginY-prevY); //*mScaleY;
	glTranslatef(posx, -posy, 0 );
}

// --------------------------------------------------------------
// Called by:
// GRMusic::OnDraw
// GRPage::getPixelSize
void			
GDeviceGL::LogicalToDevice( float * x, float * y ) const
{
	*x = (*x * mScaleX - mOriginX);
	*y = (*y * mScaleY - mOriginY);
}

// --------------------------------------------------------------
// Called by:
// GRPage::DPtoLPRect
// GRPage::getVirtualSize
void			
GDeviceGL::DeviceToLogical( float * x, float * y ) const	
{
	*x = ( *x + mOriginX ) / mScaleX;
	*y = ( *y + mOriginY ) / mScaleY;
}

// --------------------------------------------------------------
float			
GDeviceGL::GetXScale() const
{ 
	return mScaleX; 
}
// --------------------------------------------------------------
float			
GDeviceGL::GetYScale() const
{ 
	return mScaleY; 
}
// --------------------------------------------------------------
float			
GDeviceGL::GetXOrigin() const
{ 
	return mOriginX; 
}
// --------------------------------------------------------------
float		
GDeviceGL::GetYOrigin() const
{ 
	return mOriginY; 
}

// --------------------------------------------------------------
void			
GDeviceGL::NotifySize( int inWidth, int inHeight )
{ 
	mPhysicalWidth = inWidth; mPhysicalHeight = inHeight; 
}

// --------------------------------------------------------------
int			
GDeviceGL::GetWidth() const
{ 
	return mPhysicalWidth; 
}

// --------------------------------------------------------------
int	
GDeviceGL::GetHeight() const
{ 
	return mPhysicalHeight; 
}

/////////////////////////////////////////////////////////////////
// - Text and music symbols services ----------------------------
/////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// - Musical symbols only. No text
//void GDeviceGL::DrawSymbol( float x, float y, unsigned int symbol )
void GDeviceGL::DrawMusicSymbol( float x, float y, unsigned int symbol )
{
    GFontGL * glFont = (GFontGL *)mCurrMusicFont;
    
    //	FTFont * font = (FTFont *)mCurrFont->mNativeFont;
    FTFont * font = glFont->GetNativeFont();
    
    if (!font) return;
    /*unsigned int symbol_a = glFont->Symbol(symbol);
    switch (symbol_a) {
        case 96:
        case 92:
        case 88:
        case 38:
        case 46:
        case 104:
        case 120:
            break;
        default:
            ;
    }*/
    glPushMatrix();
    glEnable(GL_MULTISAMPLE);
    glPushAttrib (GL_COLOR_BUFFER_BIT);
    glColor4ub (mTextColor.mRed, mTextColor.mGreen, mTextColor.mBlue, mTextColor.mAlpha);
    
    GSystemGL * glSys = (GSystemGL *)mSys;
    //		if ((fFontType == kPixmapFont) || (fFontType == kBitmapFont)) {
    if ((glSys->GetFontType() == GSystemGL::kPixmapFont) || (glSys->GetFontType() == GSystemGL::kBitmapFont))
    {
#ifndef useglOrtho
//			font->FaceSize( (unsigned int)(mCurrFont->mHeight*mScaleY));
			font->FaceSize( (unsigned int)(mCurrMusicFont->GetSize()*mScaleY));
#endif
			glRasterPos2f(x, - y);
		}

		else glTranslatef(x, getY(y), 0.0);
		
		char s = char(symbol);
		font->Render( &s, 1 );
//		font->Render( glFont->Symbol(symbol) );
	glPopAttrib ();
	glPopMatrix();
}

// ----------------------------------------------------------------------------
// - Text only. No musical symbols
//void GDeviceGL::PrintAt( float x, float y, const char * s, int inCharCount )
void GDeviceGL::DrawString( float x, float y, const char * s, int inCharCount )
{
    if (!s) return;
    GFontGL * glFont = (GFontGL *)mCurrTextFont;
    
    //	FTFont * font = (FTFont *)mCurrFont->mNativeFont;
    FTFont * font = glFont->GetNativeFont();
    if (!font) { cout << "===> GDeviceGL::PrintAt exit" << endl;  return; }
    
    glPushMatrix();
    glPushAttrib (GL_COLOR_BUFFER_BIT);
    glColor4ub (mTextColor.mRed, mTextColor.mGreen, mTextColor.mBlue, mTextColor.mAlpha);
    
    GSystemGL * glSys = (GSystemGL *)mSys;
    //		if ((fFontType == kPixmapFont) || (fFontType == kBitmapFont)) {
    if ((glSys->GetFontType() == GSystemGL::kPixmapFont) || (glSys->GetFontType() == GSystemGL::kBitmapFont))
    {
#ifndef useglOrtho
//			font->FaceSize( (unsigned int)(mCurrFont->mHeight*mScaleY));
			font->FaceSize( (unsigned int)(mCurrTextFont->GetSize()*mScaleY));
#endif
        glRasterPos2f(x, -y);
    }
    else glTranslatef(x, getY(y), 0.0);
    font->Render( s);
    glPopAttrib ();
    glPopMatrix();
}

// ----------------------------------------------------------------------------
//void GDeviceGL::DoSetTextColor( unsigned char r, unsigned char g, unsigned char b, unsigned char a )
void GDeviceGL::SetFontColor( const VGColor & inColor )
{
	mTextColor = inColor;
	
	// GL specific - was DoSetOrigin
	mTextColor.Set (inColor.mRed, inColor.mGreen, inColor.mBlue, inColor.mAlpha);
}
// --------------------------------------------------------------
VGColor			
GDeviceGL::GetFontColor() const 
{ 
	return mTextColor; 
}
// --------------------------------------------------------------
void			
GDeviceGL::SetFontBackgroundColor( const VGColor & inColor ) 
{ 
	mTextBackColor = inColor; 

}

// --------------------------------------------------------------
VGColor			
GDeviceGL::GetFontBackgroundColor() const
{ 
	return mTextBackColor; 
}

// --------------------------------------------------------------
void			
GDeviceGL::SetFontAlign( unsigned int inAlign ) 
{ 
	mTextAlign = inAlign; 
}

// --------------------------------------------------------------
unsigned int	
GDeviceGL::GetFontAlign() const
{ 
	return mTextAlign; 
}

/////////////////////////////////////////////////////////////////
// - Printer informations services ------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void			
GDeviceGL::SetDPITag( float inDPI ) 
{ 
	mDPITag = inDPI; 
}

// --------------------------------------------------------------
float			
GDeviceGL::GetDPITag() const 
{ 
	return mDPITag; 
}

// --------------------------------------------------------------
VGSystem *		
GDeviceGL::getVGSystem() const 
{ 
	return mSys; 

	// TODO : replace this code by correct 
	// platform-dependant implementation
}

// --------------------------------------------------------------
void*		
GDeviceGL::GetBitMapPixels()
{	
	return NULL;	
}

// --------------------------------------------------------------
void		
GDeviceGL::ReleaseBitMapPixels()
{
}


// --------------------------------------------------------------
void *			
GDeviceGL::GetNativeContext() const
{
	return NULL; 
}

// ----------------------------------------------------------------------------
void GDeviceGL::TraceRect() 
{ 
	glColor3f (1.0, 0.0, 0.0);
	glRecti(1,-1,20,-20);
	glColor3f (0.0, 0.0, 0.0);
}

// ----------------------------------------------------------------------------
void GDeviceGL::TracePos(const char * msg) const
{
	GLint pos[4];
	glGetIntegerv (GL_CURRENT_RASTER_POSITION, pos);
	cout << "=> " << msg << " [" << pos[0] << ", " << pos[1] << ", " << pos[2] << ", " << pos[3] << "]" << endl;
}

// ----------------------------------------------------------------------------
void GDeviceGL::AAliasing() 
{
	glDisable(GL_DEPTH_TEST);
	glEnable (GL_POINT_SMOOTH);
	glEnable (GL_LINE_SMOOTH);
	glEnable (GL_POLYGON_SMOOTH);
	glEnable (GL_BLEND);
//	glBlendFunc (GL_SRC_ALPHA, GL_ONE);
//	glBlendFunc (GL_SRC_ALPHA, GL_ZERO);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glBlendFunc (GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
//	glBlendFunc (GL_SRC_ALPHA_SATURATE, GL_ONE);
	glHint (GL_POINT_SMOOTH_HINT, GL_FASTEST);
	glHint (GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glHint (GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
}





