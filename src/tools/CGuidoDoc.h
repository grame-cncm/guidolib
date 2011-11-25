#ifndef CGuidoDoc_H
#define CGuidoDoc_H

/*
	GUIDO Library
	Copyright (C) 2004	Grame

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

	This is the main application class.

*/

#include "GUIDOEngine.h"

// --------------------------------------------------------------
class CGuidoDoc
{
	public:
		
						CGuidoDoc();
		virtual			~CGuidoDoc();

		// - AR and GR management
		virtual	GuidoErrCode ParseFileARGR( const char * inFilePath );
		virtual	GuidoErrCode ParseStringARGR( const char * str );
		virtual GuidoErrCode AR2GR();
		virtual GuidoErrCode UpdateGR();
		virtual void		FreeARGR();

		GuidoErrCode GetLastError() const { return mLastError; }
		int			GetParseErrorLine() const { return mParseErrorLine; }

		
			ARHandler	GetAR() const { return mAR; }
			GRHandler	GetGR() const { return mGR; }

			void		SetARGR( ARHandler inAR, GRHandler inGR ) 
							{ mAR = inAR; mGR = inGR; mPageNum = 1; }

		// - Settings, page, zoom
		GuidoLayoutSettings * GetSettings();
		void SetSettings(GuidoLayoutSettings *);
		
				bool	IncPageNum( int offset ) 	{ return SetPageNum( GetPageNum() + offset ); }
				bool	SetPageNum( int in );		// >= 1
				int		GetPageNum() const			{ return mPageNum; }
				int 	PageCount() const 			{ return GuidoGetPageCount( GetGR()); }

				void	ZoomIn();
				void	ZoomOut();
				void	SetZoom( float in );
				float	GetZoom() const				{ return mZoom; }
				void	SetZoomFactor( float in )	{ mZoomFactor = in; }
				float	GetZoomFactor() const		{ return mZoomFactor; }

		static bool FormatsAreEqual( const GuidoPageFormat & a, const GuidoPageFormat & b ); 

	protected:
	
		virtual	void		ClearSettings();
		virtual void		HandleError( GuidoErrCode inCode );
		
		GuidoLayoutSettings mGuidoSettings;

		ARHandler			mAR;
		GRHandler			mGR;

		GuidoErrCode		mLastError;
		int					mParseErrorLine;
		
		float				mZoom;
		float				mZoomFactor;
		int					mPageNum;
};

#endif
