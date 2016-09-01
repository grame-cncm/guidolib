/*
 * file : MacDialogBox.cpp
 * 
 *
 */

#include "MacDialogBox.h"

// ---------------------------------------------------------------------------------
// Numeric key filter

static ControlKeyFilterUPP gMyNumericKeyFilterUPP = 0;

pascal ControlKeyFilterResult MyControlFilterNumericsOnly( ControlRef theControl, SInt16 * keyCode, 
														SInt16 * charCode, EventModifiers * modifiers )
{
	ControlKeyFilterResult result = kControlKeyFilterBlockKey;
	
	if(( *charCode >= '0' && *charCode <= '9' ) 
		|| (*charCode == kBackspaceCharCode)
		|| (*charCode == kDeleteCharCode)
		|| (*charCode == kClearCharCode)
		|| (*charCode == kRightArrowCharCode)
		|| (*charCode == kLeftArrowCharCode)
		|| (*charCode == kUpArrowCharCode)
		|| (*charCode == kDownArrowCharCode))
		
		
		result = kControlKeyFilterPassKey;
		
	// cout << "Key filter activated !" << endl;

	return result;
}

// ---------------------------------------------------------------------------------
//		* MacDialogBox
// ---------------------------------------------------------------------------------
MacDialogBox::MacDialogBox()
{
}

// ---------------------------------------------------------------------------------
//		* ~MacDialogBox
// ---------------------------------------------------------------------------------
MacDialogBox::~MacDialogBox()
{
}


// ---------------------------------------------------------------------------------
//		* InstallNumericKeyFilter
// ---------------------------------------------------------------------------------
void		
MacDialogBox::InstallNumericKeyFilter( ControlRef inEditText )
{
    // - Create Key filters
   	if( gMyNumericKeyFilterUPP == 0 )
   		gMyNumericKeyFilterUPP = ::NewControlKeyFilterUPP( MyControlFilterNumericsOnly );

	::SetControlData( inEditText, kControlEntireControl, kControlKeyFilterTag, 
 									sizeof(gMyNumericKeyFilterUPP), &gMyNumericKeyFilterUPP );

}







