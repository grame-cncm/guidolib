/*
 * file : ExportImageDialog.h
 *
 *		Dialog window for image-export settings.
 *	
 *		- Current settings
 *			Width, Height: size of the image to create, in pixels
 *
 *
 *	The "Proportionnal" setting tells that the output image must have the
 *	same width / height ratio than the input Guido Score Page.
 *
 *	The text edit fields have the well-known bug (or missing feature) not to send
 * a carbon event whenever their content is modified. The solution I've choosen
 * is to install a timer that will spy width and height values, and adjust them
 * whenever required.
 *
 * 
 */

#ifndef ExportImageDialog_H
#define ExportImageDialog_H

#include <vector>

#include "MacDialogBox.h"
#include "MacCarbonTimer.h"
// - Carbon event handler
pascal OSStatus
MyExportImageDialogSheetHandler( EventHandlerCallRef inRef,
									EventRef inEvent,  void * userData );


class DialogCaller;
// ---------------------------------------------------------------------------
/*
class DialogCaller
{
	public:
	
		virtual void	DialogFinished( MacDialog * inDialog ) = 0;
};
*/

// const OSType kExportImageControl_Signature = 'Dims';
const SInt32 kExportImageControl_Cancel = 1; 
const SInt32 kExportImageControl_Ok = 2;

const SInt32 kExportImageControl_Width = 10;
const SInt32 kExportImageControl_Height = 11;
const SInt32 kExportImageControl_Proportional = 12;
const SInt32 kExportImageControl_Format = 13;

/*class ExportImageSettings
{
	public:
	
		int		mWidth;
		int		mHeight;
		bool	mProportionnal;
}*/

// ---------------------------------------------------------------------------
class ExportImageDialog : public MacDialogBox, public MacCarbonTimer
{
	public:

							ExportImageDialog(/*DialogCaller * inCaller */);
		virtual				~ExportImageDialog();
		
		virtual bool 		Setup( IBNibRef inNibRef, CFStringRef inNibSheetName,
									WindowRef inParentWindow, double inRatio = 1 );
		
		// - MacDialogBox heritage

		virtual UInt32		DoDialog();
		
		virtual OSStatus	EventCallback( EventRef inEvent );

		
		// - Specific

			ControlRef 		GetDialogControl( SInt32 inID ) const;
	
			WindowRef		GetDialogWindow() const { return mDialogWindow; }
			WindowRef		GetParentWindow() const { return mParentWindow; }

		
		// - Values
		
				int			GetPixelWidth() const;
				int			GetPixelHeight() const;
				OSType		GetExportFormat() const;

	protected:
	
		virtual void		Fire();			// Timer heritage				
		virtual int 		BuildExporterList();

		virtual	void		DialogFinished( UInt32 inCommand );
				int 		ApplyProportionnal( SInt32 srcControl, SInt32 destControl, double inRatio );
	
				bool		GetProportionnal();
	
	
			WindowRef mDialogWindow;
			WindowRef mParentWindow;

			UInt32			mResultCmd;

			
				bool		mProportionnal;
				double		mPrevWidth;
				double		mPrevHeight;
				double		mPageRatio;
				
		std::vector<OSType>	mFormatList;	// list of format codes in the menu
};

#endif
