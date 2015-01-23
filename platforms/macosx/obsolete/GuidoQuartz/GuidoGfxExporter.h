/*
 * file : GuidoGfxExporter.h
 * 
 *
 */

#ifndef GuidoGfxExporter_H
#define GuidoGfxExporter_H

#include "MacGfxExporter.h"
#include "GUIDOEngine.h"

// ---------------------------------------------------------------------------
/** Save a Guido graphic score page to a graphic file (jpeg, png...)

	It provides the graphic export functionnality to Guido applications.
	
 	Pages are drawn offscreen, then saved to a file using QuickTime graphic
 	export components.
 	
*/
class GuidoGfxExporter : public MacGfxExporter
{
	public:
			
							GuidoGfxExporter();
		virtual				~GuidoGfxExporter();
		
		/** \brief Draws a score page offscreen and save it in a file. 
		
			\param inGuidoHandleGR a Guido opaque handle to a GR structure.
			\param inPageNum the page index to export, starting from 1.
			\param inWidth the width of the new picture.
			\param inHeight the height of the new picture.
			\param inFileSpecs the destination file.
			\return true on success.
			
			\note While the FSSpec type is deprecated, it's required for
				  Quicktime version prior to 6.4. Futur versions of this
				  class may implement an Export() method using CFURL instead.
					
		*/
		virtual bool 		ExportGuidoPage( GRHandler inGuidoHandleGR, int inPageNum, 
									int inWidth, int inHeight, 
									FSSpec * inFileSpecs );
			

		/** \brief Defines the background color of the page. 
		
			default color is white (255, 255, 255)
		*/
				void		SetBackgroundColor( int inRed, int inGreen, int inBlue );
						

	protected:

		virtual bool		Initialize( int inWidth, int inHeight );
		virtual bool 		ChangeSize( int inWidth, int inHeight );
		virtual bool 		Draw( GRHandler inGuidoHandleGR, int inPage );
		virtual bool 		Save( const FSSpec * inDestinationFile );

		virtual void		DisposeOffscreen();

			GWorldPtr		mGWorld;

			char *			mOffscreenMemory;
			int 			mHeight;
			int 			mWidth;	
			
			int				mRed, mGreen, mBlue;	
};

#endif
