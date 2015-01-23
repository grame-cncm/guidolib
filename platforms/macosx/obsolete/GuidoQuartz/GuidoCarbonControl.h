#ifndef GuidoCarbonControl_H
#define GuidoCarbonControl_H

/*
 * 
 * File : GuidoCarbonControl.h
 *
 *	
 *	
 */

#include <vector>

#include "GDeviceOSX.h"
#include "GUIDOEngine.h"

// - Callback prototype
pascal OSStatus GuidoCarbonControlEventHandler( EventHandlerCallRef inHandlerCallRef, 
												EventRef inEvent, void * inUserData );
class CGuidoDoc;
class ScoreSymbolMap;
// ----------------------------------------------------------------------------
class GuidoCarbonControl
{
	public:
			enum { kDrawSymbolMap, kParamCount };
	
							GuidoCarbonControl();
		virtual				~GuidoCarbonControl();
	
		
				void		SetGuidoDocInfos( CGuidoDoc * in ) { mGuidoInfos = in; }
		//		void		SetGDevice( GDeviceOSX * in ) { mGDevice = in; }
		
				void		Draw() { ::Draw1Control( GetControlRef()); }


			//	void		SetGuidoRef( int inGuidoRef ) { mGuidoRef = inGuidoRef; }
				GRHandler	GetGuidoGR() const;
				int			GetPageNum() const;
				void		SetZoom( float in );
				float		GetZoom() const;
				void		ZoomIn();
				void		ZoomOut();
			
				void		SetScrollX( int x ) { mScrollX = x; }
				void		SetScrollY( int y ) { mScrollY = y; }
				int			GetScrollX() const { return mScrollX; }
				int			GetScrollY() const { return mScrollY; }
				
				
		virtual void		SetParam( int id, float inParam ) { mParams[ id ] = inParam; }
		virtual float		GetParam( int id ) const	{ return mParams [ id ]; }

				ControlRef	GetControlRef() const { return mControlRef; }
				void		SetControlRef( ControlRef in ) { mControlRef = in; }

				OSStatus	HandleControlEvent( EventRef inEvent );
		static 	GuidoCarbonControl * FindControlByRef( ControlRef inRef );

				void		GetSize( int * oX, int * oY ) const;
				void		GetFullScoreSize( float * outWidth, float * outHeight ) const; // pixels

	protected:

		virtual bool		Register();		// Carbon toolbox
		virtual void		Unregister();	// Carbon toolbox

		// - Mousing
		virtual void		HitTest( EventRef inEvent );
		virtual	void		TrackMouse( EventRef inEvent );
		
		// - Drawing
//		virtual void		PrecalcScoreSize( VGDevice * inDevice );
		
		virtual	void		DoDraw();
				void		DrawSymbolMap();

	//	ScoreSymbolMap	* 	GetSymbolMap(); // ok
				
		//		GDeviceOSX * GetGDevice() { return mGDevice; }

		static ToolboxObjectClassRef sControlClassRef;		// Carbon toolbox
		static EventHandlerUPP 		sEventHandlerProc;		// Carbon toolbox
		static std::vector<GuidoCarbonControl *> sControlList;	// the list we're in

		static 	int			sInstanceNumGen;
				int			mInstanceNum;



		ControlRef			mControlRef;			// Carbon toolbox
		CGuidoDoc *			mGuidoInfos;
	//	GDeviceOSX *		mGDevice;
		GRHandler			mGuidoGR;
		
		int					mScrollX;				// in pixels
		int					mScrollY;
	//	int					mScoreWidth;			// in pixels
	//	int					mScoreHeight;
		
		float				mParams [ kParamCount ];
};



#endif