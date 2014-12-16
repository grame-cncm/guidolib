#ifndef __GuidoDoc__
#define __GuidoDoc__

#include "GUIDOEngine.h"
#include <string>

// --------------------------------------------------------------
class GuidoDoc
{
	public:
		
								GuidoDoc();
		virtual					~GuidoDoc();

		// - AR and GR management
		virtual	GuidoErrCode	Read( const char * inFilePath );
				int				ParseErrorLine() const { return GuidoGetParseErrorLine(); }

		
		GRHandler				GetGR() const	{ return fGR; }
		GuidoLayoutSettings *	GetSettings()	{ return &fGuidoSettings; }
		void					SetSettings(GuidoLayoutSettings *);
		
				bool			IncPageNum( int offset ) 	{ return SetPageNum( GetPageNum() + offset ); }
				bool			SetPageNum( int num );
				int				GetPageNum() const			{ return fPageNum; }
				int 			PageCount() const 			{ return GuidoGetPageCount( GetGR()); }

				void			ZoomIn();
				void			ZoomOut();
				void			SetZoom( float in );
				float			GetZoom() const				{ return fZoom; }
				void			SetZoomFactor( float in )	{ fZoomFactor = in; }
				float			GetZoomFactor() const		{ return fZoomFactor; }

				std::string		Path() const			{ return fFile; }
				std::string		File() const;

	protected:	
		virtual	void			ClearSettings();
		
		GuidoLayoutSettings		fGuidoSettings;
		GRHandler				fGR;
		float					fZoom;
		float					fZoomFactor;
		int						fPageNum;
		std::string				fFile;
};

#endif
