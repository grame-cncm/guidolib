/*
 * file : ExportImageWizard.h
 * 
 */

#ifndef ExportImageWizard_H
#define ExportImageWizard_H

#include "ExportImageDialog.h" // To be replaced by a generic macos dialog class.
#include "GUIDOEngine.h"
// ---------------------------------------------------------------------------
class ExportImageWizard //: public DialogCaller
{
	public:
			

								ExportImageWizard( IBNibRef inNibRef, WindowRef inParentWindow );
		virtual					~ExportImageWizard();

		virtual void			Invoke( GRHandler inGuidoHandleGR, int inPageNum, 
												CFStringRef inDocName = 0 );
		
	protected:

		virtual void			ProcessDialogResult( ExportImageDialog * inDialog, CFStringRef inDocName );

				IBNibRef		GetNibRef() const { return mNibRef; }

			IBNibRef 			mNibRef;	
			WindowRef			mParentWindow;
			
			GRHandler			mGuidoHandleGR;
			int mPageNum;
};

#endif
