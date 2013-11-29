/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRAccolade.h"
#include "VGDevice.h"
#include "GuidoDefs.h"
#include "GraphTools.h"

// --------------------------------------------------------------------------
GRAccolade::GRAccolade( int inAccoladeType, float inDx )
{
	mAccoladeType = inAccoladeType;
	mDx = inDx;
}

// --------------------------------------------------------------------------
GRAccolade::~GRAccolade()
{
}

// --------------------------------------------------------------------------
void 
GRAccolade::draw( VGDevice & hdc, const NVPoint & leftTop, 
								const NVPoint & leftBottom ) const
{
	// - Accolade, can be scaled 
	switch( mAccoladeType )
	{
		case kAccoladeThin: // mywidth *= float(0.67); // (JB) don't know if it's what we want.
		{
			const float thickness = float(0.45) * LSPACE/2;
			DrawEmulatedAccolade( hdc, leftTop.x + mDx, leftTop.y, leftBottom.y, thickness );
		
			break;
		}
		case kAccoladeCurly:
		{
			/*	
			if( hdc.AllowNonProportional())
			{
				const float totalHeight = leftBottom.y - leftTop.y;							
				// - Hard-coded x offset
				float mywidth = 0;
				float offsx = - 35;
				if( mysize > 1 )	// TODO: Check this
				{
					offsx = -60;
					mywidth = 56;	// (JB) was 100;	//	hardcoded
				}
				GRNotationElement::OnDrawSymbol( hdc, kAccoladeSymbol, offsx - mPosition.x, 
											(last.y - mPosition.y + staffHeight), totalHeight, mywidth );
			}
			else*/
			
				const float thickness = float(0.45) * LSPACE;
				DrawEmulatedAccolade( hdc, leftTop.x + mDx, leftTop.y, leftBottom.y, thickness );
		
			break;
		}

		case kAccoladeStraight:	// it could also exists in a version without end little curves.
		{
			bool drawEndCurves = true;
			const float edgeCorrection = float(0.05) *  LSPACE;
			DrawStraightAccolade( hdc, leftTop.x + mDx,
								leftTop.y - edgeCorrection, 
								leftBottom.y + edgeCorrection, 
								drawEndCurves );
			break;
		}
	}
}

// --------------------------------------------------------------------------
/** \brief Draws a vectorized straight accolade.
*/
void 
GRAccolade::DrawStraightAccolade( VGDevice & hdc, float x, float yTop, 
									float yBottom, bool drawEndCurves ) const
{
	const float thick = LSPACE * float(0.5);
	const float xOffset = LSPACE * float(0.5);
		
	x -= xOffset;

	// - Setup graphic context
//	hdc.PushPen( VGColor( 0, 0, 0, ALPHA_TRANSPARENT ), 0 );	// invisible
//	hdc.PushFillColor( VGColor( 0, 0, 0 ));						//opaque

	// - Draws the body

	hdc.Rectangle( x - thick, yTop, x, yBottom );


	// - Draws the end curves
	if( drawEndCurves )
	{
		int index;
		int currPt;

		const int segCount = 6;	// quality of the curve
		const int ptCount = (2 * ( segCount + 3 ));	
		NVPoint thePoints[ ptCount ];
		float xPoints [ ptCount ];
		float yPoints [ ptCount ];

		float startX = x - thick;
		float startYa = yTop;
		float startYb = yTop - thick;
		float endX = startX + LSPACE * float( 2 );
		float endY = startYa - LSPACE * float( 1.25 );
		float x2 = startX + ((endX - startX) * float(0.5));
		float y2a = startYa;
		float y2b = startYb;

		// - Draw the upper little curve
		index = 0;
		makeCurve( startX, startYa, x2, y2a, x2, y2a, endX, endY, segCount, thePoints, &index );
		makeCurve( endX, endY, x2, y2b, x2, y2b, startX, startYb, segCount, thePoints, &index );

		for( currPt = 0; currPt < index; ++ currPt )
		{
			xPoints [ currPt ] = thePoints[ currPt ].x;
			yPoints [ currPt ] = thePoints[ currPt ].y;
		}

		hdc.Polygon( xPoints, yPoints, index );

		// - Mirror some coordinates for the bottom little curve

		startYa = yBottom;
		startYb = yBottom + thick;
		endY = startYa + LSPACE * float( 1.25 );
		y2a = startYa;
		y2b = startYb;

		// - Draw the lower little curve
		index = 0;
		makeCurve( startX, startYa, x2, y2a, x2, y2a, endX, endY, segCount, thePoints, &index );
		makeCurve( endX, endY, x2, y2b, x2, y2b, startX, startYb, segCount, thePoints, &index );

		for( currPt = 0; currPt < index; ++ currPt )
		{
			xPoints [ currPt ] = thePoints[ currPt ].x;
			yPoints [ currPt ] = thePoints[ currPt ].y;
		}

		hdc.Polygon( xPoints, yPoints, index );


	}

	// - Restore graphic context
//	hdc.PopFillColor();
//	hdc.PopPen();
}

// --------------------------------------------------------------------------
/** \brief Draws a vectorized curly accolade  (brace), usefull for graphic devices
	 that don't allow non-proportional scaling.
 
	version 2
*/
void 
GRAccolade::DrawEmulatedAccolade( VGDevice & hdc, float x, float yTop, float yBottom, float thickness ) const
{
	const float xOffset = LSPACE * 0.33f;			// offset between brace and system
	const float yMid = (yTop + yBottom) * 0.5f;		// middle of the system

	x -= xOffset;

	// - Defines the shape of the curve.
	float x1, x2, x3, x4, y1, y2, y3, y4; // the four bezier points.

	x1 = x;			// the start anchor point
	y1 = yTop;

	x2 = x1 - float(1.5) * LSPACE;		// the first control point
	y2 = yTop + float(1.25) * LSPACE;

	x3 = x + float(1.25) * LSPACE;		// the second control point
	y3 = yMid - float(0.66) * LSPACE;

	x4 = x - float(1) * LSPACE;		// the end anchor point
	y4 = yMid;

	float x2b = x2 - thickness;
	float x3b = x3 - thickness; // we could also modify y's

	// - Prepare our table of curve points
	const int segCount = 25;	// quality of the curve
	const int ptCount = (2 * ( segCount + 3 ));	
	NVPoint thePoints[ ptCount ];
	float xPoints [ ptCount ];
	float yPoints [ ptCount ];
	int currPt;
	int index;

//	hdc.PushPen( VGColor( 0, 0, 0 ), 1 );// We may use an invisible pen.
//	hdc.PushFillColor( VGColor( 0, 0, 0 ));

	// - Draws the top part of the accolade
	index = 0;
	makeCurve( x1, y1, x2, y2, x3, y3, x4, y4, segCount, thePoints, &index );
	makeCurve( x4, y4, x3b, y3, x2b, y2, x1, y1, segCount, thePoints, &index );

	for( currPt = 0; currPt < index; ++ currPt )
	{
		xPoints [ currPt ] = thePoints[ currPt ].x;
		yPoints [ currPt ] = thePoints[ currPt ].y;
	}

	hdc.Polygon( xPoints, yPoints, index );

	// - Mirror some coordinates for the bottom of the accolade
	y1 = yBottom;
	y2 = yBottom - (y2 - yTop);
	y3 = yBottom - (y3 - yTop);

	// - Draws the bottom part of the accolade
	index = 0;
	makeCurve( x1, y1, x2, y2, x3, y3, x4, y4, segCount, thePoints, &index );
	makeCurve( x4, y4, x3b, y3, x2b, y2, x1, y1, segCount, thePoints, &index );
	for( currPt = 0; currPt < index; ++ currPt )
	{
		xPoints [ currPt ] = thePoints[ currPt ].x;
		yPoints [ currPt ] = thePoints[ currPt ].y;
	}

	hdc.Polygon( xPoints, yPoints, index );

//	hdc.PopFillColor();
//	hdc.PopPen();
}

// --------------------------------------------------------------------------
/* \brief Draws a vectorized accolade (brace), usefull for graphic devices
	 that don't allow non-proportional scaling.

	version 1 

void 
GRAccolade::DrawEmulatedAccolade( VGDevice & hdc, float x, float yTop, float yBottom ) const
{
	// - Arbitraty constants that defines the shape of the accolade

	const float xOffset = LSPACE * 0.5f;		// offset between brace and system
	const float midRad = LSPACE * 0.65f;		// radius of the edge curve.
	const float borderRad = LSPACE * 0.5f;		// radius of the middle curve.
	const float	middlePush = 0; //LSPACE * 0.5f;	// x-offset of the middle part. 0 = strictly vertical.

	const float lineThick = LSPACE * 0.25f;

	// - Preprocess
	x -= xOffset;
	float x1 = x - borderRad;
	float x2 = x1 - midRad;
	float x3 = x + midRad;

	const float y1 = yTop + borderRad;
	const float y3 = (yTop + yBottom) * 0.5f;
	const float y2 = y3 - midRad;
	const float y4 = y3 + midRad;
	const float y5 = yBottom - borderRad;

	// - Drawing

	hdc.PushPen( GColor( 0, 0, 0 ), lineThick );	// TODO: use correct color

	// - Top curve
	hdc.Arc( x1, yTop, x3, y1 + borderRad,
				x, yTop, x1, y1 );

	// - Bottom curve
	hdc.Arc( x1, y5 - borderRad, x3, yBottom,
				x1, y5, x, yBottom );

	// - Push the middle to the right.
	x1 += middlePush;
	x2 += middlePush;
	x3 += middlePush;

	// - Middle curve (top)
	hdc.Arc( x2 - midRad, y2 - midRad, x1, y3,
				x2, y3, x1, y2 );


	// - Middle curve (bottom)
	hdc.Arc( x2 - midRad, y3, x1, y4 + midRad,
				x1, y4, x2, y3 );


	// - Vertical lines
	hdc.Line( x1 - middlePush, y1, x1, y2 );
	hdc.Line( x1, y4, x1 - middlePush, y5 );

	hdc.PopPen();
}
*/


