/*
  GUIDO Library
  Copyright (C) 2006 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string.h>
#include "GDevicePostScript.h"
#include "VGFont.h"
#include "secureio.h"

// --------------------------------------------------------------
GDevicePostScript::GDevicePostScript( int inWidth, int inHeight,
									  const char * inPSFilePath, 
									  const char * inCreatorStr, 
									  const char * inHeaderFilePath )	
		: mRasterMode(kOpCopy), 
		  mScaleX(1), mScaleY(1), mOriginX(0), mOriginY(0), 
		  mDPITag(72), mBeginDrawCount(0), mPSFilePath ( inPSFilePath ),
		  mCreatorStr( inCreatorStr ),	// creator string
		  mHeaderFilePath( inHeaderFilePath ), mDrawing( false )		  
{

	mPhysicalWidth  = inWidth;
	mPhysicalHeight = inHeight;

	mPenStack.reserve( 16 );
	mBrushStack.reserve( 16 );
	mStateStack.reserve( 16 );
	
	mTextBackColor.Set( 255, 255, 255, ALPHA_OPAQUE ); // transparent

	mTextAlign = 0;
	
	PSReset();
}

// --------------------------------------------------------------
GDevicePostScript::~GDevicePostScript()
{
	if( mFile )
	{
		fclose( mFile );
		mFile = 0;
	}
}

// --------------------------------------------------------------
// Append a font file..
void
GDevicePostScript::PSIncludeHeaderFile()
{
	if( mHeaderFilePath.length() <= 0 )
		return;

	FILE * headerFile = fopen( mHeaderFilePath.c_str(), "r" );	// "r"
	if( headerFile )
	{
		char buf[BUFSIZ];
 
		size_t n;
		while (( n = fread(buf, sizeof(char), BUFSIZ, headerFile)) > 0)
			fwrite(buf, sizeof(char), n, GetFile());
 
		fclose( headerFile );
	}
}

// --------------------------------------------------------------
void
GDevicePostScript::PSReset()
{
	mFile			= 0;
	mMoveToSet		= false;

	mCurrPenPos.x	= -1;
	mCurrPenPos.y	= -1;

	SetFontAlign( kAlignLeft | kAlignBase );

//	mCurrFont		= 0;
	mCurrTextFont	= 0;
	mCurrMusicFont	= 0;
	mCurrGrayColor = -1;
}

// --------------------------------------------------------------
// Return the previous font.
void
GDevicePostScript::SetTextFont( const VGFont * inObj )
{
//	if( inObj == 0 ) return mCurrFont;
//	if( inObj == mCurrFont ) return mCurrFont;
	
//	GFontRef old = mCurrFont;
//	GFontInfos * fontDesc = (GFontInfos *)(inObj);

//	fprintf( GetFile(), "/%s %d selectfont\n", fontDesc->GetName(), fontDesc->mHeight );
	fprintf( GetFile(), "/%s %d selectfont\n", inObj->GetName(), inObj->GetSize() );

	mCurrTextFont = inObj;
//	return old;
}

// --------------------------------------------------------------
// Return the previous font.
void
GDevicePostScript::SetMusicFont( const VGFont * inObj )
{
//	if( inObj == 0 ) return mCurrFont;
//	if( inObj == mCurrFont ) return mCurrFont;
	
//	GFontRef old = mCurrFont;
//	GFontInfos * fontDesc = (GFontInfos *)(inObj);

//	fprintf( GetFile(), "/%s %d selectfont\n", fontDesc->GetName(), fontDesc->mHeight );
	fprintf( GetFile(), "/%s %d selectfont\n", inObj->GetName(), inObj->GetSize() );

	mCurrMusicFont = inObj;
//	return old;
}

// --------------------------------------------------------------
void			
//GDevicePostScript::DrawSymbol( float x, float y, unsigned int inSymbolID )
GDevicePostScript::DrawMusicSymbol( float x, float y, unsigned int inSymbolID )
{
	char theChar = (char)(inSymbolID);	// TODO: correct convertion
	
	if( theChar == (char)151 )	// catch the incorrect stem-up 2 character
		theChar = (char)219;
	
//	PrintAt( x, y, &theChar, 1 );
	DrawString( x, y, &theChar, 1 );
}

// --------------------------------------------------------------
// With PostScript, the current position is the base-left point
// of displayed text.
void			
//GDevicePostScript::PrintAt( float x, float y, const char * s, int inCharCount )
GDevicePostScript::DrawString( float x, float y, const char * s, int inCharCount )
{
	if( s == 0 ) return;
	if( s[ 0 ] == '\0' ) return;
	if( inCharCount == 0 ) return;

	if( inCharCount == -1 ) 
		inCharCount = (int)strlen( s );


	/*/ DEBUG -> 
	if( s[ 0 ] != '=' )
	{
		GStackPen( *this, GColor( 0.4 ));
		Line( x - 10, y - 10, x + 10, y + 10 );
		Line( x + 10, y - 10, x - 10, y + 10 );
	}
	//*/

	// - Calculate string dimensions
	float w = 0;
	float h = 0;
	float baseline = 0; 
//	PSGetTextDims( s, inCharCount, &w, &h, &baseline );
	mCurrTextFont->GetExtent( s, inCharCount, &w, &h, 0 );
	baseline = h * 0.2f;

	// - Perform text alignement
	if( mTextAlign != ( kAlignLeft | kAlignBase ))
	{	
		if( mTextAlign & kAlignBottom )	// Vertical align
			y -= baseline; 
		else if( mTextAlign & kAlignTop )
			y += h - baseline;

		if( mTextAlign & kAlignRight )	// Horizontal align
			x -= w;
		else if( mTextAlign & kAlignCenter )
			x -= (w / 2);
	}

	// - Postcript
	MoveTo( x, y );
	//PSMoveTo( x, y ); // DEBUG

	if( inCharCount == 1 )	// Draw a char
	{
		fprintf(GetFile(), "(\\%o) show\n", (unsigned char)(*s));
	}
	else					// Draw a string
	{
 		char * ps_tmp = new char [inCharCount + 1]; // TODO: optimize
 		if (inCharCount) strncpy( ps_tmp, s, (size_t)inCharCount ); 
 		ps_tmp[ inCharCount ] = '\0';

	//	fprintf( GetFile(), "newpath\n" ); // Requiered ?
		fprintf(GetFile(),"(%s) show\n", ps_tmp ); 
		delete [] ps_tmp;
	}
}

// --------------------------------------------------------------
void 
GDevicePostScript::MoveTo( float x, float y )
{
	if(( x == mCurrPenPos.x ) && ( y == mCurrPenPos.y ))
		return;
	
	PSMoveTo( x, y );
}

// --------------------------------------------------------------
void 
GDevicePostScript::LineTo( float x, float y )
{	
 	if( mMoveToSet == false )
 		fprintf( GetFile(), "%f %f moveto\n", mCurrPenPos.x, -mCurrPenPos.y ); // To check
 	
  	fprintf( GetFile(), "%f %f lineto\nstroke\n", x, -y );	
	
	mCurrPenPos.x = x;
	mCurrPenPos.y = y;
	mMoveToSet = false;
}

// --------------------------------------------------------------
void 
GDevicePostScript::Line( float x1, float y1, float x2, float y2 )
{
	MoveTo( x1, y1 );
	LineTo( x2, y2 );
}

// --------------------------------------------------------------
void 
GDevicePostScript::Rectangle( float left, float top, float right, float bottom )
{
	float oldGray = mCurrGrayColor;

	if( mFillColor.mAlpha < ALPHA_OPAQUE )		// Paint with brush
	{
		SetGrayColor( mFillColor );
		fprintf( GetFile(),"newpath\n" );
		PSAddRectangleToPath( left, top, right, bottom );	
		fprintf( GetFile(),"closepath\n" );
		fprintf( GetFile(),"fill\n" );
	}

	if( mPen.mColor.mAlpha < ALPHA_OPAQUE )	// Draws the outline with pen
	{
		SetGrayColor( mPen.mColor );
		fprintf( GetFile(),"newpath\n" );
		PSAddRectangleToPath( left, top, right, bottom );	
		fprintf( GetFile(),"closepath\n" );
		fprintf( GetFile(),"stroke\n" );
	}

	SetGrayColor( oldGray );
//	fprintf( GetFile(),"fill\n" );
	mMoveToSet = false;
}

// --------------------------------------------------------------
void 
GDevicePostScript::PSAddRectangleToPath( float left, float top, float right, float bottom )
{
	fprintf( GetFile(),"%f %f moveto\n", left, - top );
	fprintf( GetFile(),"%f %f lineto\n", right, - top );
	fprintf( GetFile(),"%f %f lineto\n", right, - bottom );
	fprintf( GetFile(),"%f %f lineto\n", left, - bottom );
	mMoveToSet = true;
}
	
// --------------------------------------------------------------
void 
GDevicePostScript::Polygon( const float * xCoords, const float * yCoords, int count )
{
	if( xCoords == 0 ) return;
	if( yCoords == 0 ) return;
	if( count <= 1 ) return;

	fprintf( GetFile(),"newpath\n" );
	
	PSMoveTo( xCoords[ count - 1 ], yCoords[ count - 1 ] );

	for( int index = 0; index < count; index ++ )
		fprintf( GetFile(),"%f %f lineto\n", xCoords[ index ], - yCoords[ index ] );

	fprintf( GetFile(),"closepath\n" ); 
	fprintf( GetFile(),"fill\n" );
	mMoveToSet = false;
}

// --------------------------------------------------------------
void		
GDevicePostScript::Arc( float left, float top, float right, float bottom, 
					float startX, float startY, float endX, float endY )
{
}

// --------------------------------------------------------------
/*void
GDevicePostScript::PSGetTextDims( const char * inString, int inCharCount, 
				float * outWidth, float * outHeight, float * outBaseline ) const
{
//	GetTextExtent( inString, inCharCount, outWidth, outHeight );
	mCurrTextFont->GetExtent( inString, inCharCount, outWidth, outHeight, 0 );
	*outBaseline = *outHeight * 0.2f;
}*/

// --------------------------------------------------------------
// TODO: multiple pages support.
bool
GDevicePostScript::BeginDraw()
{
	// generic VGDevice
/*	if( ++ mBeginDrawCount <= 1 ) 
	{
		if( mSymbolMap ) 
			mSymbolMap->Reset(); 
	}
*/
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

	// was DoSaveDC()
	fprintf( GetFile(),"gsave\n" );

		
	// ps specific
	if( mDrawing ) return true;

	PSReset();

	if( mFile == 0 )	
	{
		mFile = fopen( mPSFilePath.c_str(), "w" );
		if( mFile == 0 ) 
			return false;
	}

//	float tmpRight = (float)mSizeX;
//	float tmpBottom = (float)mSizeY;
//	DeviceToLogical( &tmpRight, &tmpBottom );	// TEST

	int boundRight = GetWidth();
	int boundBottom = GetHeight();

	mDrawing = true;
	fprintf( GetFile(), "%%!PS-Adobe-2.0 EPSF-2.0\n"
			"%%%%Title: outfile.eps\n"	// TODO
			"%%%%Creator: %s\n"
			"%%%%Pages: 0 1\n"
			"%%%%BoundingBox: 0 0 %d %d\n"		// *must* be integers
	//		"%%%%DocumentFonts:\n"
			"%%%%EndComments\n\n", mCreatorStr.c_str(), (int)boundRight, (int)boundBottom );

	PSIncludeHeaderFile();

	// - Invert Y axix orientation (top to bottom)
 	fprintf( GetFile(), "%f %f translate\n", 0.0f, (float)boundBottom );

	MoveTo( 0, 0 );								// Force the printing of the initial current pos.
//	SelectPen( VGColor( 0, 0, 0 ), 1);			// This will write the pen and brushe initial
//	SelectFillColor( VGColor( 0, 0, 0 ));	// states into the file.

	return true;
}

// --------------------------------------------------------------
void
GDevicePostScript::EndDraw()
{
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
	
	mStateStack.pop_back();

	//was DoRestoreDC
	fprintf( GetFile(),"grestore\n" );

	if( !mDrawing ) return;
	mDrawing = false;

	if( mFile ) return;	
	{
		// fprintf( GetFile(),"showpage\n" );	// Not required in eps files.
		fprintf( GetFile(), "%%%%Trailer\n");
		fclose( mFile );
		mFile = 0;
	}
}

// --------------------------------------------------------------
/*
	Pushes a copy of the current graphics state onto the graphics state stack. 
	The graphics state consists of (among other things):

    - Current Transformation Matrix
    - Current Path
    - Clip Path
    - Current Color
    - Current Font
    - Current Gray Value
*/
/*void				
GDevicePostScript::DoSaveDC() 
{ 
	fprintf( GetFile(),"gsave\n" );
}

// --------------------------------------------------------------
void				
GDevicePostScript::DoRestoreDC() 
{ 
	fprintf( GetFile(),"grestore\n" );
}*/

// --------------------------------------------------------------
// Save the current pen, select the new one
void
GDevicePostScript::PushPen( const VGColor & inColor, float inWidth )
{
	mPenStack.push_back( mPen );
	SelectPen( inColor, inWidth );
}

// --------------------------------------------------------------
// Restore the previous pen from the stack
void
GDevicePostScript::PopPen()
{
	VGPen & pen = mPenStack.back();
	SelectPen( pen.mColor, pen.mWidth );
	mPenStack.pop_back();
}

void GDevicePostScript::PushPenColor( const VGColor & inColor)
{
	mPenColorStack.push( mPen.mColor );
	SelectPenColor (inColor);
}
void GDevicePostScript::PopPenColor()
{
	SelectPenColor (mPenColorStack.top());
	mPenColorStack.pop();
}
		
void GDevicePostScript::PushPenWidth( float width)
{
	mPenWidthStack.push( mPen.mWidth );
	SelectPenWidth (width);
}
void GDevicePostScript::PopPenWidth()
{
	SelectPenWidth (mPenWidthStack.top());
	mPenWidthStack.pop();
}
// --------------------------------------------------------------
// Save the current brush, select the new one
void
GDevicePostScript::PushFillColor( const VGColor & inColor )
{
	mBrushStack.push_back( mFillColor );
	if( inColor != mFillColor )
		SelectFillColor( inColor );
}

// --------------------------------------------------------------
// Restore the previous brush from the stack
void
GDevicePostScript::PopFillColor()
{
	VGColor & brush = mBrushStack.back();
	SelectFillColor( brush );
	mBrushStack.pop_back();
}
// --------------------------------------------------------------
void			
GDevicePostScript::SetFontColor( const VGColor & inColor )
{
	mTextColor = inColor; 
	const float transp = 1.0f - (float(inColor.mAlpha) / 255.0f);

	SetGrayColor( ((inColor.mRed + inColor.mGreen + inColor.mBlue) / 3 / 255) * transp );
}

// --------------------------------------------------------------
void			
GDevicePostScript::SetScale( float x, float y )
{
	const float prevX = mScaleX;
	const float prevY = mScaleY;
	mScaleX = x;
	mScaleY = y;	
	
	if(( x != prevX ) || (y != prevY))
	{ 	
 		fprintf( GetFile(), "%f %f scale\n", (x / prevX), (y /prevY));
		
		// - TODO: Udpate origin?

		// -

		// - We must update the current position after each coordinate 
		// transformation
		PSMoveTo( mCurrPenPos.x, mCurrPenPos.y );
	}
}

// --------------------------------------------------------------
void				
GDevicePostScript::SetOrigin( float x, float y )
{
	const float prevX = mOriginX;
	const float prevY = mOriginY;
	mOriginX = x;
	mOriginY = y;
	
	if(( x != prevX ) || (y != prevY ))
	{
	//	const float newX = mOrigin.x + (x -  mOrigin.x) * mScale.x;
	//	const float newY = mOrigin.y + (y -  mOrigin.y) * mScale.y;

		// - PS convertion ->
		const float offX = x - (prevX /* * mScale.x*/);
		const float offY = y - (prevY /* * mScale.y*/);

		fprintf( GetFile(), "%f %f translate\n", offX, - offY );

		//mPSOrigin.x += offX;
		//mPSOrigin.y += offY;
		// <-

	//	mOrigin.x = newX;
	//	mOrigin.y = newY;

		// - We must update the current position after each coordinate 
		// transformation
		PSMoveTo( mCurrPenPos.x, mCurrPenPos.y );
	}
}

// --------------------------------------------------------------

void				
GDevicePostScript::OffsetOrigin( float x, float y )
{
	mOriginX += x;
	mOriginY += y;

/*	if( x == 0 && y == 0 )
		return;

	const float newX = mOrigin.x + x * mScale.x;
	const float newY = mOrigin.y + y * mScale.y;

	// - PS convertion ->
	const float offX = x;
	const float offY = y;

	fprintf( GetFile(), "%f %f translate\n", offX, - offY ); // OK

//	mPSOrigin.x += offX;
//	mPSOrigin.y += offY;
	// <-

	mOrigin.x = newX;
	mOrigin.y = newY;

	// - We must update the current position after each coordinate 
	// transformation
	PSMoveTo( mCurrPenPos.x, mCurrPenPos.y );*/
}
// --------------------------------------------------------------
void			
GDevicePostScript::LogicalToDevice( float * x, float * y ) const
{
	*x = (*x * mScaleX - mOriginX);
	*y = (*y * mScaleY - mOriginY);
}

// --------------------------------------------------------------
void			
GDevicePostScript::DeviceToLogical( float * x, float * y ) const	
{
	*x = ( *x + mOriginX ) / mScaleX;
	*y = ( *y + mOriginY ) / mScaleY;
}
// --------------------------------------------------------------
void			
GDevicePostScript::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 )
{
	const float xCoords [] = { x1, x2, x3 };
	const float yCoords [] = { y1, y2, y3 };

	Polygon( xCoords, yCoords, 3 );
}

// --------------------------------------------------------------
void *			
GDevicePostScript::GetNativeContext() const
{
	return NULL; 
}


// --------------------------------------------------------------
void			
GDevicePostScript::PSMoveTo( float x, float y )
{
	fprintf( GetFile(),"%f %f moveto\n", x, -y );
	
	mCurrPenPos.x = x;
	mCurrPenPos.y = y;
	mMoveToSet = true;
}

// --------------------------------------------------------------
void
GDevicePostScript::SetGrayColor( float inValue ) const
{
	if( inValue == mCurrGrayColor ) return;

	fprintf( GetFile(), "%f setgray\n", inValue );
}

// --------------------------------------------------------------
void
GDevicePostScript::SetGrayColor( const VGColor & col ) const
{
	const float transp = 1.0f - (float(col.mAlpha) / 255.0f);

	SetGrayColor( ((col.mRed + col.mGreen + col.mBlue) / 3 / 255) * transp );
}


