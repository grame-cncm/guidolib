/*
	GUIDO Library
	Copyright (C) 2006  Grame

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

#include <string>
#include "GFontWin32GDIPlus.h"

#pragma warning (disable : 4996)	// for _CRT_SECURE_NO_DEPRECATE

// --------------------------------------------------------------
GFontWin32GDIPlus::GFontWin32GDIPlus( Font* nativeFont, const char * faceName, 
									  int size, int properties, 
									  const unsigned int * charSet ) 
						: mNativeFont(nativeFont), mName(faceName),
						  mSize(size), mFontProp(properties), fSymbolTable(charSet)
{
	std::cerr << "new GFontWin32GDIPlus::GFontWin32GDIPlus: " << mName << std::endl;

	DWORD count = mbstowcs(NULL, faceName, strlen(faceName));
	WCHAR * wstr = (WCHAR*) malloc ((count + 1) * sizeof(WCHAR));
	mbstowcs(wstr, faceName, strlen(faceName));
	FontCollection fc;
	FontFamily ff (wstr, &fc);
	int style;
	switch (properties) {
		case kFontBold:
			style = FontStyleBold;
			break;
		case kFontItalic:
			style = FontStyleItalic;
			break;
		case kFontBold+kFontItalic:
			style = FontStyleBoldItalic;
			break;
		case kFontUnderline:
			style = FontStyleUnderline;
			break;
		default:
			style = FontStyleRegular;
	}
	mEmHeight = ff.GetEmHeight(style);
	float ratio = mEmHeight ? (float)mEmHeight / size : 1.f;
	mAscent = ff.GetCellAscent(style) / ratio;
	mDescent = ff.GetCellDescent(style) / ratio;
	free (wstr);
}

// --------------------------------------------------------------
GFontWin32GDIPlus::~GFontWin32GDIPlus()
{ 
	Font* winFont = GetNativeFont();
	if( winFont )
		delete ( winFont );
}
// --------------------------------------------------------------
void
GFontWin32GDIPlus::GetExtent( const char * str, int inCharCount, float * outWidth, 
					   float * outHeight, VGDevice * context ) const 
{
	// convert input string
	DWORD count = mbstowcs(NULL, str, inCharCount);
	WCHAR * wstr = (WCHAR*) malloc ((count + 1) * sizeof(WCHAR));
	mbstowcs(wstr, str, inCharCount);

	//std::string s(str);
	//std::wstring wstr(s.length(),L' ');
	//std::copy(s.begin(), s.end(), wstr.begin());

	Graphics* gdiContext = (Graphics*) GetContext( context );

/*	-- known Gdi+ issue: 
	We must use the following way to determine correct font extent 
	because typical Graphics::MeasureString() method doesn't work
	properly (return values are incorrect due to antialiasing)
*/
	Font* dgiFont = GetNativeFont();

	// Layout rectangles used for drawing strings
	RectF   layoutRect_A(0.0f, 0.0f, 1300.0f, 1300.0f);
	// 1 range of character positions within the string
	CharacterRange charRange(0, inCharCount);
	// String format used to apply to string when drawing
	StringFormat strFormat;
	// Set three ranges of character positions.
	strFormat.SetMeasurableCharacterRanges(1, &charRange);
	Region *pCharRangeRegions = new Region();

	// Get the regions that correspond to the ranges within the string when
	// layout rectangle A is used. 
	gdiContext->MeasureCharacterRanges( wstr, inCharCount, dgiFont, layoutRect_A, &strFormat, 1, pCharRangeRegions);

	RectF boundRect;
	pCharRangeRegions->GetBounds( &boundRect, gdiContext);

	*outWidth = boundRect.Width;
	*outHeight = boundRect.Height;
	free (wstr);
}

// --------------------------------------------------------------
void	
GFontWin32GDIPlus::GetExtent( unsigned char c, float * outWidth, 
					          float * outHeight, VGDevice * context ) const
{
	WCHAR wstr [] = L"0";
	mbstowcs(wstr, (const char*)&c, 1);

	Graphics* gdiContext = (Graphics*) GetContext( context );

/*	-- known Gdi+ issue: 
	We must use the following way to determine correct font extent 
	because typical Graphics::MeasureString() method doesn't work
	properly (return values are incorrect due to antialiasing)
*/
	Font* dgiFont = GetNativeFont();

	// Layout rectangles used for drawing strings
	RectF   layoutRect_A(0.0f, 0.0f, 1300.0f, 1300.0f);
	// 1 range of character positions within the string
	CharacterRange charRange(0, 1);
	// String format used to apply to string when drawing
	StringFormat strFormat;
	// Set three ranges of character positions.
	strFormat.SetMeasurableCharacterRanges(1, &charRange);
	Region *pCharRangeRegions = new Region();

	// Get the regions that correspond to the ranges within the string when
	// layout rectangle A is used. 
	gdiContext->MeasureCharacterRanges( wstr, 1, dgiFont, layoutRect_A, &strFormat, 1, pCharRangeRegions);

	RectF boundRect;
	pCharRangeRegions->GetBounds( &boundRect, gdiContext);

	*outHeight = boundRect.Height;
	*outWidth = boundRect.Width;
	// in some cases (when some symbols must be forced into char map), extent needs to be fixed  
	if (c == 139) *outWidth = 6.0;

}
