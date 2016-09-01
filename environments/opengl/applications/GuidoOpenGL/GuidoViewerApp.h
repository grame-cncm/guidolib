#ifndef __GuidoViewerApp__
#define __GuidoViewerApp__

#include <string>
#include "GUIDOEngine.h"

class GuidoDoc;
class VGSystem;

// ----------------------------------------------------------------------------
class GuidoViewerApp
{
	public:
							GuidoViewerApp();
		virtual				~GuidoViewerApp();

				bool 		Initialize(const char* guidofont, const char* textfont, bool perspective);
		virtual void 		Reload();
		virtual void 		OpenFile(const char * file);
		virtual void		DrawScore();
		virtual void		SetSize(int w, int h);
		virtual void		SetDevice(VGDevice * inDevice);
		virtual void		FullScreen(bool on);
		const char *		FontType() const;

				bool        firstPage();
				bool        nextPage();
				bool        previousPage();
				bool        lastPage();
				int         get_fWidth() 	{ return fWidth; }
				int         get_fHeight() 	{return fHeight; }
				void        zoomOut() 		{ zoomFactor += 0.1; }
				void        zoomIn() 		{ if(zoomFactor>0) zoomFactor -= 0.1;}
				void        setPerspective(bool state) 		{  fPerspective = state;}
	
	protected:
				GuidoDoc *	OpenGuidoFile( const char * file );
				GuidoDoc *	OpenXMLFile( const char * file);
				void		SetCamera (int w, int h, bool perspective=false);

				void		ReportGuidoError( GuidoErrCode inGuidoErr );
				void		ReportError( const char * inMessage, const char * inDetail = 0 );

				VGDevice *	fDevice;
				VGSystem *	fSystem;
				GuidoDoc *	fDocument;
				int			fWidth, fHeight;
				int			fWinX, fWinY, fWinW, fWinH;
				bool		fPerspective;

				int         pageNum;
				float       zoomFactor;
};


#endif

