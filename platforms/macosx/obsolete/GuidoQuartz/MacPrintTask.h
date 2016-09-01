/*
 * file : MacPrintTask.h
 *
 *		01/2004		JB	creation
 *
 *		! Work in progress !
 * 
 */

#ifndef MacPrintTask_H
#define MacPrintTask_H

#include <Carbon/Carbon.h>

// ---------------------------------------------------------------------------
class MacPrintTask
{
	public:
							MacPrintTask();
		virtual				~MacPrintTask();

		// - Setup
		virtual void		SetTaskName( CFStringRef inName );
		
		virtual void		SetPageCount( int in ) { mPageCount = in; }
		virtual int			GetPageCount() const { return mPageCount; }

		// - Runtime
		virtual	bool		Start();

		
	protected:

		virtual bool		IsQuartzMode() { return true; }

		virtual void		MakePrintSettings();
		virtual void		MakePageFormat();
		virtual void		ApplyQuartzMode();
		
		// - Print loop
		virtual void		PrintDialogDone( bool accepted );
		virtual bool		LaunchPrintLoop();
		virtual bool		DoPrintDocument();
		virtual bool		DoPrintPage( int inPageNum ); // >= 1
		virtual bool		DoDrawPageContent( CGContextRef inContext, 
													int inPageNum ); // >= 1

  		PMPrintSession		mPrintSession;
		PMPrintSettings		mPrintSettings;
		PMPageFormat		mPageFormat;

		int					mPageCount;
		CFStringRef			mPrintJobName;
		
};

#endif
