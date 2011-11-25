#ifndef GuidoDocWindow_H
#define GuidoDocWindow_H

/*
 * 	File : GuidoDocWindow.h
 *
 * 	Copyright (C) 2003  Grame
 *
 *		JB	10-2003		Creation
 *	
 *			
 */

#include "GDeviceOSX.h"
// #include "ScoreSymbolMap.h"
//#include "GuidoDocInfos.h"
#include "CGuidoDoc.h"

class GuidoCarbonControl;
// ----------------------------------------------------------------------------
class GuidoDocWindow : public CGuidoDoc
{
	public:
								GuidoDocWindow();
		virtual					~GuidoDocWindow();

		virtual bool 			SetupGuidoWindow( WindowRef inWinRef );
		virtual void			SetPath( const FSRef * inPath );
		virtual FSRef *			GetPath();
	
		virtual	bool			NotifyNewGuidoHandle( GRHandler inGuidoHandle );
		virtual	void			BringToScreen();

		virtual void			SizeChanged();
		virtual	void			RefreshGuidoView();

		GuidoCarbonControl *	GetGuidoControl() { return mGuidoControl; }
		WindowRef				GetWindowRef() const { return mWindowRef; }
//		GDeviceOSX *			GetGDevice() { return &mGDevice; }

		virtual void			SetShowMapping( bool inState );
		virtual bool			GetShowMapping() const;
		virtual bool			XML() const  { return mXMLFile; }
		virtual void			XML(bool val)  { mXMLFile = val; }

		virtual OSStatus		HandleMouseEvent( EventRef inEvent );
		virtual	void			NotifyWindowScrolled();
		virtual float			GetZoom() const;
		virtual	void			HandleZoomCommand( FourCharCode inCommand );

				CFStringRef		GetFileNameCFString() { return mFileNameCF; }

	protected:

		// - Scrolling
		
		static 	ControlActionUPP sScrollerUPP; 

				int				GetScrollBarThickness() const;
		virtual void			InitScrollBars();
		virtual void			AdjustScrollBars();
		virtual void			PlaceScrollBars();

		// - Document handling
		virtual	bool			BuildPath( FSRef * inPath );
		virtual	void			MakeFileName();
		virtual bool			PathIsValid();
		virtual bool			SyncParentPath();	
		virtual void			RefreshWindowTitle();
		
		CFStringRef				mFileNameCF;
		HFSUniStr255			mFileNameUni;
		FSRef					mFilePath;
		FSRef					mParentFilePath;
		bool					mPathSet;
		bool					mXMLFile;
		
		// - Guido
	//	GDeviceOSX				mGDevice;		// For score rendering
		GuidoCarbonControl * 	mGuidoControl;
	//	ScoreSymbolMap			mSymbolMap;

		WindowRef				mWindowRef;		// Our Carbon window
};


#endif



