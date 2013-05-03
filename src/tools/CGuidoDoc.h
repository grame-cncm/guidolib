#ifndef CGuidoDoc_H
#define CGuidoDoc_H

/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
