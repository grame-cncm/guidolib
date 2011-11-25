/*
 * file : MacDialogBox.h
 * 
 */

#ifndef MacDialogBox_H
#define MacDialogBox_H

//#include "VAlertBox.h"
//#include "CString.h"
//#include "BaseString.h"

// #include <string>
#include <Carbon/Carbon.h>

// ---------------------------------------------------------------------------
class MacDialogBox
{
	public:
							MacDialogBox();

		virtual				~MacDialogBox();
	
		virtual UInt32		DoDialog() = 0;


//		virtual OSStatus	EventCallback( EventRef inEvent ) = 0;

		virtual	 void		InstallNumericKeyFilter( ControlRef inEditText );

	protected:
		
};


#endif
