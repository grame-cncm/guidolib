/*
	GUIDO Library
	Copyright (C) 2004  Torben Hohn

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
/// GTK implementation of VGDevice.
///
/////////////////////////////////////////////////////////////////

#include "GDeviceGtk.h"
#include "GFontGtk.h"

#include "GSystemGtk.h"		// GUIDO HACK - REMOVE ASAP

// --------------------------------------------------------------
GDeviceGtk::GDeviceGtk( GdkDrawable * inDrawable, int inWidth, int inHeight ) 
		: mCurrTextFont(NULL), mCurrMusicFont(NULL),
		  mRasterMode(kOpCopy),  
		  mDPITag(72.0f), mBeginDrawCount(0), mDrawable( inDrawable )
{
	// Default device init ---------------------------
	// Default pen and brush colors must be pure black.
	mPhysicalWidth  = inWidth;
	mPhysicalHeight = inHeight;

	mPenStack.reserve( 16 );
	mBrushStack.reserve( 16 );
	
	SetFontAlign( kAlignLeft | kAlignBase );

	// GTK -------------------------------------------
	Init();

	mPangoContext = gdk_pango_context_get();
	gdk_pango_context_set_colormap( mPangoContext, gdk_rgb_get_colormap() );

	if( mDrawable )
	{
	    printf( "dc is OK\n" );
		mGC = gdk_gc_new( mDrawable );

		// TODO: get GdkGC...
					
		// - Initial state of GDevice
		SelectPen( VGColor( 0, 0, 0), 1 );
		SelectFillColor( VGColor( 0, 0, 0) );
		SetFontColor( VGColor(0,0,0) );
		SetFontBackgroundColor( VGColor(255,255,255,ALPHA_TRANSPARENT) );	
		SetScale( 1, 1 );
		SetOrigin( 0, 0 );
	}
	else
	    printf( "dc is NULL\n" );

	// guigo hack - remove asap
	mSys = new GSystemGtk(inDrawable, NULL);	        
}

// --------------------------------------------------------------
GDeviceGtk::~GDeviceGtk()
{
	delete [] mPtArray;
}

// --------------------------------------------------------------
bool
GDeviceGtk::IsValid() const
{
	if( mDrawable == NULL ) return false;
	
	return true;
}

/////////////////////////////////////////////////////////////////
// - Drawing services -------------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
bool
GDeviceGtk::BeginDraw()
{
	
	// general device's state save - was VGDevice::SaveDC()
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

	s.gtkUserScaleX = mGtkScaleX;
	s.gtkUserScaleY = mGtkScaleY;
	s.gtkOriginX	= mGtkOriginX;
	s.gtkOriginY	= mGtkOriginY;
	s.penPos		= mCurrPenPos;

	mStateStack.push_back( s );

	// TODO : complete this code with correct 
	// platform-dependant implementation

	return true;
	
}

// --------------------------------------------------------------
void
GDeviceGtk::EndDraw()
{
	// Default device code ---------------------------
	if( -- mBeginDrawCount == 0 )
	{}

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

	mGtkOriginX		= s.gtkOriginX;
	mGtkOriginY		= s.gtkOriginY;
	mGtkScaleX		= s.gtkUserScaleX;
	mGtkScaleY		= s.gtkUserScaleY;
	mCurrPenPos		= s.penPos;
	
	mStateStack.pop_back();

	SetTextFont( s.currTextFont );
	SetMusicFont( s.currMusicFont );

}

// --------------------------------------------------------------
void 
GDeviceGtk::MoveTo( float x, float y )
{
    printf( "moveto: %f, %f\n", x, y );
	mCurrPenPos.x = lrintf (x * mGtkScaleX + mGtkOriginX);
	mCurrPenPos.y = lrintf (y * mGtkScaleY + mGtkOriginY);
}

// --------------------------------------------------------------
void 
GDeviceGtk::LineTo( float x, float y )
{
	int gx = lrintf (x * mGtkScaleX + mGtkOriginX); 
	int gy = lrintf (y  * mGtkScaleY + mGtkOriginY); 
    printf( "lineto: %f, %f -> (%d, %d)\n", x, y, gx, gy );

	gdk_draw_line( mDrawable, mGC, mCurrPenPos.x, mCurrPenPos.y, gx, gy );

	mCurrPenPos.x = gx;
	mCurrPenPos.y = gy;
}

// --------------------------------------------------------------
void 
GDeviceGtk::Line( float x1, float y1, float x2, float y2 )
{
	MoveTo( x1, y1 );
	LineTo( x2, y2 );
}

// --------------------------------------------------------------
void			
GDeviceGtk::Frame( float left, float top, float right, float bottom )
{
    MoveTo (left, top);
    LineTo (right, top);
    LineTo (right, bottom);
    LineTo (left, bottom);
    LineTo (left, top);
}

// --------------------------------------------------------------
void		
GDeviceGtk::Arc( float left, float top, float right, float bottom, 
					float startX, float startY, float endX, float endY )
{
	int x = (int) (left);
	int y = (int) (top);
	int width = (int) (right - left);
	int height = (int) (bottom - top);
	
	float midX = (left + right) * 0.5f;
	float midY = (top + bottom) * 0.5f;

	int angle1 = (int) (atan( (startX - midX)/(startY - midY) ) * 360 * 64 * 2 * M_PI);
	int angle2 = (int) (atan( (endX - midX)  /(endY - midY)   ) * 360 * 64 * 2 * M_PI - angle1);

    gdk_draw_arc( mDrawable, mGC, 0, x, y, width, height, angle1, angle2 );
}
// --------------------------------------------------------------
void			
GDeviceGtk::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 )
{
	const float xCoords [] = { x1, x2, x3 };
	const float yCoords [] = { y1, y2, y3 };

	Polygon( xCoords, yCoords, 3 );
}

// --------------------------------------------------------------
void 
GDeviceGtk::Polygon( const float * xCoords, const float * yCoords, int count )
{
	if( count > mPtCount )
	{
		if( mPtArray )
			delete [] mPtArray;
		
		mPtArray = new GdkPoint [ count ];
		if( mPtArray )
			mPtCount = count;
	}

	for( int index = 0; index < count; index ++ )
	{
		mPtArray[ index ].x = (int) (xCoords[ index ] * mGtkScaleX + mGtkOriginX);
		mPtArray[ index ].y = (int) (yCoords[ index ] * mGtkScaleY + mGtkOriginY);
	}

	gdk_draw_polygon( mDrawable, mGC, 0, mPtArray, count );
	
}

// --------------------------------------------------------------
void 
GDeviceGtk::Rectangle( float left, float top, float right, float bottom )		
{ 
	int x = (int) (left * mGtkScaleX + mGtkOriginX); 
	int y = (int) (top  * mGtkScaleY + mGtkOriginY); 
	int w = (int) ((left - right) * mGtkScaleX); 
	int h = (int) ((top - bottom ) * mGtkScaleY); 

	gdk_draw_rectangle( mDrawable, mGC, 0, x, y, w, h );
}

/////////////////////////////////////////////////////////////////
// - Font services ----------------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void			
GDeviceGtk::SetMusicFont( const VGFont * font ) 
{ 
	mCurrMusicFont = font; 

	GFontGtk*  gtkFont = (GFontGtk*)mCurrMusicFont;
	
//	printf( "select music font = %d\n", (PangoFontDescription *)font->GetNativeFont() );

	pango_context_set_font_description( mPangoContext, 
					    (PangoFontDescription*) gtkFont->GetNativeFont() );
} 

// --------------------------------------------------------------
const VGFont *	
GDeviceGtk::GetMusicFont() const				  
{ 
	return mCurrMusicFont; 
}

// --------------------------------------------------------------
void			
GDeviceGtk::SetTextFont( const VGFont * font ) 			
{ 
	mCurrTextFont = font; 

	GFontGtk*  gtkFont = (GFontGtk*)mCurrTextFont;

//	printf( "select text font = %d\n", font->GetNativeFont() );

	pango_context_set_font_description( mPangoContext, 
					    (PangoFontDescription*) gtkFont->GetNativeFont() );
} 

// --------------------------------------------------------------
const VGFont *	
GDeviceGtk::GetTextFont() const				  
{ 
	return mCurrTextFont; 
}

/////////////////////////////////////////////////////////////////
// - Pen & brush services ---------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void			
GDeviceGtk::SelectPen( const VGColor & inColor, float width ) 
{
	mPen.Set( inColor, width ); 

	GdkColor color = { 0, inColor.mRed, inColor.mGreen, inColor.mBlue };

	GdkLineStyle style = (inColor.mAlpha >= ALPHA_TRANSPARENT) ? GDK_LINE_ON_OFF_DASH : GDK_LINE_SOLID ;
	
	gdk_gc_set_rgb_fg_color( mGC, &color );
	gdk_gc_set_line_attributes( mGC, lrintf(width * mGtkScaleX), style, GDK_CAP_ROUND, GDK_JOIN_MITER );
} 
// --------------------------------------------------------------
void			
GDeviceGtk::SelectFillColor( const VGColor & c ) 	
{ 
	mFillColor.Set( c ); 

	GdkColor color = { 0, c.mRed, c.mGreen, c.mBlue };

	gdk_gc_set_rgb_fg_color( mGC, &color );
} 

// --------------------------------------------------------------
// Save the current pen, select the new one
void
GDeviceGtk::PushPen( const VGColor & inColor, float inWidth )
{
	mPenStack.push_back( mPen );
	SelectPen( inColor, inWidth );

	// TODO : complete this code with correct 
	// platform-dependant implementation
}

// --------------------------------------------------------------
// Restore the previous pen from the stack
void
GDeviceGtk::PopPen()
{
	VGPen & pen = mPenStack.back();
	SelectPen( pen.mColor, pen.mWidth );
	mPenStack.pop_back();

	// TODO : complete this code with correct 
	// platform-dependant implementation
}

// --------------------------------------------------------------
// Save the current brush, select the new one
void
GDeviceGtk::PushFillColor( const VGColor & inColor )
{
	mBrushStack.push_back( mFillColor );
	if( inColor != mFillColor )
		SelectFillColor( inColor );

	// TODO : complete this code with correct 
	// platform-dependant implementation
}

// --------------------------------------------------------------
// Restore the previous brush from the stack
void
GDeviceGtk::PopFillColor()
{
	VGColor & brush = mBrushStack.back();
	SelectFillColor( brush );
	mBrushStack.pop_back();

	// TODO : complete this code with correct 
	// platform-dependant implementation
}

// --------------------------------------------------------------
void			
GDeviceGtk::SetRasterOpMode( VRasterOpMode ROpMode )  
{ 
	mRasterMode = ROpMode; 
} 

/////////////////////////////////////////////////////////////////
// - Bitmap services (bit-block copy methods) -------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
bool			
GDeviceGtk::CopyPixels( VGDevice* pSrcDC, float alpha ) 
{
	return false; 
	// TODO : replace this code by correct 
	// platform-dependant implementation
}

// --------------------------------------------------------------
bool			
GDeviceGtk::CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, 
						 int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha )	
{ 
	return false; 
	// TODO : replace this code by correct 
	// platform-dependant implementation
}

// --------------------------------------------------------------
bool			
GDeviceGtk::CopyPixels( int xDest, int yDest, 
						 int dstWidth, int dstHeight, VGDevice* pSrcDC, float alphas ) 
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
GDeviceGtk::SetScale( float x, float y )
{
	const float prevX = mScaleX;
	const float prevY = mScaleY;
	mScaleX = mGtkScaleX = x;
	mScaleY = mGtkScaleY = y;	

	// TODO : complete this code with correct 
	// platform-dependant implementation
	// was DoSetScale( prevX, prevY, x, y );

	printf( "set Scale %f, %f\n", x, y );
//	mGtkScaleX = x;
//	mGtkScaleY = y;
}

// --------------------------------------------------------------
// Called only by GRPage
void				
GDeviceGtk::SetOrigin( float x, float y )
{
	const float prevX = mOriginX;
	const float prevY = mOriginY;
	mOriginX = x;
	mOriginY = y;

	// TODO : complete this code with correct 
	// platform-dependant implementation
	// was DoSetOrigin( prevX, prevY, x, y );

	printf( "set Origin %f, %f\n", x, y );
	mGtkOriginX += (int) ((x - prevX) * GetXScale());
	mGtkOriginY += (int) ((y - prevY) * GetYScale());

}

// --------------------------------------------------------------
// Called by: GRPage, GRStaff, GRSystem
void				
GDeviceGtk::OffsetOrigin( float x, float y )
{
	const float prevX = mOriginX;
	const float prevY = mOriginY;
	mOriginX += x;
	mOriginY += y;

	// TODO : complete this code with correct 
	// platform-dependant implementation
	// was DoSetOrigin( prevX, prevY, x, y );

	printf( "set Origin %f, %f\n", x, y );
	mGtkOriginX += (int) ((x - prevX) * GetXScale());
	mGtkOriginY += (int) ((y - prevY) * GetYScale());
}

// --------------------------------------------------------------
// Called by:
// GRMusic::OnDraw
// GRPage::getPixelSize
void			
GDeviceGtk::LogicalToDevice( float * x, float * y ) const
{
	*x = (*x * mScaleX - mOriginX);
	*y = (*y * mScaleY - mOriginY);

	// TODO : complete this code with correct 
	// platform-dependant implementation
}

// --------------------------------------------------------------
// Called by:
// GRPage::DPtoLPRect
// GRPage::getVirtualSize
void			
GDeviceGtk::DeviceToLogical( float * x, float * y ) const	
{
	*x = ( *x + mOriginX ) / mScaleX;
	*y = ( *y + mOriginY ) / mScaleY;

	// TODO : complete this code with correct 
	// platform-dependant implementation
}

// --------------------------------------------------------------
float			
GDeviceGtk::GetXScale() const
{ 
	return mScaleX; 
}
// --------------------------------------------------------------
float			
GDeviceGtk::GetYScale() const
{ 
	return mScaleY; 
}
// --------------------------------------------------------------
float			
GDeviceGtk::GetXOrigin() const
{ 
	return mOriginX; 
}
// --------------------------------------------------------------
float		
GDeviceGtk::GetYOrigin() const
{ 
	return mOriginY; 
}

// --------------------------------------------------------------
void			
GDeviceGtk::NotifySize( int inWidth, int inHeight )
{ 
	mPhysicalWidth = inWidth; mPhysicalHeight = inHeight; 
}

// --------------------------------------------------------------
int			
GDeviceGtk::GetWidth() const
{ 
	return mPhysicalWidth; 
}

// --------------------------------------------------------------
int	
GDeviceGtk::GetHeight() const
{ 
	return mPhysicalHeight; 
}

/////////////////////////////////////////////////////////////////
// - Text and music symbols services ----------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void			
GDeviceGtk::DrawMusicSymbol(float x, float y, unsigned int inSymbolID ) 
{
    int gx = (int) (x * mGtkScaleX + mGtkOriginX); 
    int gy = (int) (y  * mGtkScaleY + mGtkOriginY); 
    
    GFontGtk*  gtkFont = (GFontGtk*)mCurrMusicFont;

//  PangoFontDescription *mydescr = pango_font_description_copy( (PangoFontDescription *) mCurrFont->mNativeFont );
//  pango_font_description_set_size( mydescr, mCurrFont->mHeight * PANGO_SCALE * mGtkScaleY );
    
    PangoFontDescription *mydescr = pango_font_description_copy( (PangoFontDescription *) gtkFont->GetNativeFont() );
    pango_font_description_set_size( mydescr, gtkFont->GetSize() * PANGO_SCALE * mGtkScaleY );

    printf( "Draw Symbol %f, %f -> (%d,%d) size = %d \n", x,y, gx,gy, (int) (gtkFont->GetSize() * PANGO_SCALE * mGtkScaleY) );

 //   char theChar = mSymbolToChar[ inSymbolID ];
    char theChar = gtkFont->Symbol( inSymbolID );

    printf( "char = %c, %d\n", theChar, (int) theChar );

    gsize newstringlen;
    char *utf8string = g_convert( &theChar, 1, "LATIN1", "UTF8", NULL, &newstringlen, NULL );
    PangoLayout *layout = pango_layout_new( mPangoContext );

    PangoLanguage* pangoLanguage = pango_language_from_string( "en-us" );
    PangoFontMetrics *metrics = pango_context_get_metrics( mPangoContext, mydescr, pangoLanguage );

    pango_layout_set_font_description( layout, mydescr );
    pango_layout_set_text( layout, utf8string, newstringlen );


    const unsigned int textAlign = GetFontAlign();
    if( textAlign != ( kAlignLeft | kAlignTop ))
    {
		int w = 0;
		int h = 0;
		int descent = (pango_font_metrics_get_descent( metrics ) / PANGO_SCALE) + 1; 
		//int descent = 0;
		pango_layout_get_pixel_size( layout, &w, &h );

		h--;
		if( textAlign & kAlignBase )
			gy -= ( h - descent ); 
		else if( textAlign & kAlignBottom )
			gy -= h;		// depends on axis orientation ?	

		// - Perform horizontal text alignement
		if( textAlign & kAlignRight )
			gx -= w;
		else if( textAlign & kAlignCenter )
			gx -= (w * 0.5);
    }

    gdk_draw_layout( mDrawable, mGC, gx, gy, layout );

    pango_font_description_free( mydescr );	
    g_free( utf8string );
    g_object_unref( G_OBJECT( layout ) );
}
// --------------------------------------------------------------
void			
GDeviceGtk::DrawString( float x, float y, const char * s, int inCharCount ) 
{
    printf( "printAt( %f, %f ) scale = %f, origin = %d \n", x, y, mGtkScaleX, mGtkOriginX );

	int gx = (int)( x * mGtkScaleX + mGtkOriginX ); 
	int gy = (int)( y  * mGtkScaleY + mGtkOriginY ); 

	PangoLayout *layout = pango_layout_new( mPangoContext );

	pango_layout_set_text( layout, s, inCharCount );
	gdk_draw_layout( mDrawable, mGC, gx, gy, layout );

	g_object_unref( G_OBJECT( layout ) );
}

// --------------------------------------------------------------
void			
GDeviceGtk::SetFontColor( const VGColor & inColor ) 
{ 
	mTextColor = inColor; 

	GdkColor color  = { 0, inColor.mRed, inColor.mGreen, inColor.mBlue };
	gdk_gc_set_rgb_bg_color( mGC, &color );
}

// --------------------------------------------------------------
VGColor			
GDeviceGtk::GetFontColor() const 
{ 
	return mTextColor; 
}

// --------------------------------------------------------------
void			
GDeviceGtk::SetFontBackgroundColor( const VGColor & inColor ) 
{ 
	mTextBackColor = inColor; 

	GdkColor color  = { 0, inColor.mRed, inColor.mGreen, inColor.mBlue };
	gdk_gc_set_rgb_bg_color( mGC, &color );

}

// --------------------------------------------------------------
VGColor			
GDeviceGtk::GetFontBackgroundColor() const
{ 
	return mTextBackColor; 
}

// --------------------------------------------------------------
void			
GDeviceGtk::SetFontAlign( unsigned int inAlign ) 
{ 
	mTextAlign = inAlign; 
}

// --------------------------------------------------------------
unsigned int	
GDeviceGtk::GetFontAlign() const
{ 
	return mTextAlign; 
}

/////////////////////////////////////////////////////////////////
// - Printer informations services ------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void			
GDeviceGtk::SetDPITag( float inDPI ) 
{ 
	mDPITag = inDPI; 
}

// --------------------------------------------------------------
float			
GDeviceGtk::GetDPITag() const 
{ 
	return mDPITag; 
}

// --------------------------------------------------------------
VGSystem *		
GDeviceGtk::getVGSystem() const 
{ 
	return mSys; 

	// TODO : replace this code by correct 
	// platform-dependant implementation
}

// --------------------------------------------------------------
void *			
GDeviceGtk::GetNativeContext() const
{
	return mGC; 
}

// --------------------------------------------------------------
void
GDeviceGtk::Init()
{
	mStateStack.reserve( 16 );
	mGtkOriginX		= 0;
	mGtkOriginY		= 0;

	mCurrPenPos.x	= 0;
	mCurrPenPos.y	= 0;

	mPtArray		= 0;	// For polygon
	mPtCount		= 0;	// For polygon
}

// --------------------------------------------------------------
#if 0
void 
GDeviceGtk::AddWxAlignOffset( const wxString & inString, wxCoord * ioX, wxCoord * ioY ) const		
{ 
	const unsigned int textAlign = GetTextAlign();
	if( textAlign != ( kAlignLeft | kAlignTop ))
	{
		wxCoord w = 0;
		wxCoord h = 0;
		wxCoord descent = 0; 
		mDC->GetTextExtent( inString, &w, &h, &descent );// <- crashes wxWindows if font is wrong.

		if( textAlign & kAlignBase )
			*ioY -= ( h - descent ); 
		else if( textAlign & kAlignBottom )
			*ioY -= h;		// depends on axis orientation ?	

		// - Perform horizontal text alignement
		if( textAlign & kAlignRight )
			*ioX -= w;
		else if( textAlign & kAlignCenter )
			*ioX -= (w * 0.5);
	}
}
#endif

// --------------------------------------------------------------
int 
GDeviceGtk::MaxWidth() const
{	
	int width;
	int height;
	gdk_drawable_get_size( mDrawable, &width, &height );
	return width;
}

// --------------------------------------------------------------
int 
GDeviceGtk::MaxHeight() const
{
	int width;
	int height;
	gdk_drawable_get_size( mDrawable, &width, &height );
	return height;
}






