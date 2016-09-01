/*
 * file : MacGfxExporter.h
 * 
 *
 */

#ifndef MacGfxExporter_H
#define MacGfxExporter_H

#include <QuickTime/ImageCompression.h>

// ---------------------------------------------------------------------------
/** Save a Guido graphic score page to a graphic file (jpeg, png...)

	It provides the graphic export functionnality to Guido applications.
	
 	Pages are drawn offscreen, then saved to a file using QuickTime graphic
 	export components.
 	
*/
class MacGfxExporter
{
	public:
			
							MacGfxExporter();
		virtual				~MacGfxExporter();

		virtual bool		SetExportFormat( OSType inFormatCode );

		virtual bool		Export( const FSSpec * inDestinationFile, GWorldPtr inImage );

		
				CFStringRef CreateDefaultFileExtensionString();
				void		EnableSettingsDialog( bool inState );
		

	protected:

		    GraphicsExportComponent mComponent;
			OSType			mFormatCode;
			bool			mShowSettingsDialog;
};

#endif
