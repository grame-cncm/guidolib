#ifndef GuidoDocInfos_H
#define GuidoDocInfos_H

/*
 * File: GuidoDocInfos.h		
 * Copyright (C) 2003  Grame
 *
 *	Handle informations related to the score rendering.
 *	
 *
 */

#include "GUIDOEngine.h"
// --------------------------------------------------------------
class GuidoDocInfos
{
	public:
				GuidoDocInfos(): mGuidoRef( 0 ), mZoom( 1 ), mZoomFactor( 1.5f ), 
									mPageNum( 1 ) { }

		void	SetGuidoRef( int in )		{ mGuidoRef = in; SetPageNum( 1 ); }
		int		GetGuidoRef() const			{ return mGuidoRef; }

		bool	SetPageNum( int in );		// >= 1
		bool	IncPageNum( int offset ) 	{ return SetPageNum( GetPageNum() + offset ); }
		int		GetPageNum() const			{ return mPageNum; }
		int 	PageCount() const 			{ return GuidoPageCount( GetGuidoRef()); }

		void	ZoomIn();
		void	ZoomOut();
		void	SetZoom( float in );
		float	GetZoom() const				{ return mZoom; }
		void	SetZoomFactor( float in )	{ mZoomFactor = in; }
		float	GetZoomFactor() const		{ return mZoomFactor; }

	protected:

		int		mGuidoRef;
		float	mZoom;
		float	mZoomFactor;
		int		mPageNum;
};

// --------------------------------------------------------------
// Return true if the page is actually changed
inline bool GuidoDocInfos::SetPageNum( int in )
{
	int maxPage = PageCount();
	if( maxPage < 1 )
		maxPage = 1;

	if( in > maxPage )			in = maxPage;	
	else if( in < 1 )			in = 1;

	if( in == mPageNum )
		return false;
	
	mPageNum = in;
	return true;
}
// --------------------------------------------------------------
inline void GuidoDocInfos::ZoomIn()
{
	SetZoom( GetZoom() * mZoomFactor );
}

// --------------------------------------------------------------
inline void GuidoDocInfos::ZoomOut()
{
	SetZoom( GetZoom() / mZoomFactor );
}

// --------------------------------------------------------------
inline void GuidoDocInfos::SetZoom( float in )
{
	if( in <= 0.001f )	// min zoom: 1 / 1000
		in = 0.001f;
	else if( in > 1000 )	// max zoom: 1000
		in = 1000;
	else if( in >= 0.99f && in <= 1.01f )	// normalize
		in = 1;

	mZoom = in;
}



#endif