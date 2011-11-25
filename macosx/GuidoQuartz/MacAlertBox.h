/*
 * file : MacAlertBox.h
 * 
 */

#ifndef MacAlertBox_H
#define MacAlertBox_H

//#include "VAlertBox.h"
//#include "CString.h"
//#include "BaseString.h"

#include <string>

// ---------------------------------------------------------------------------
// Comportement par defaut: un seul bouton, "OK".
//
class MacAlertBox // : public VAlertBox
{
	public:
			
			enum { kButtonOK = 1, kButtonCancel, kButtonOther };
			enum { kResultError, kResultOK, kResultCancel, kResultOther };

								MacAlertBox();
		virtual					~MacAlertBox();

		virtual void			SetupButtons(	bool showCancel,
												bool showOther,	
												int inDefaultButtonID );

		virtual	void			SetButtonText( int inButtonID, const char * inString );

		virtual int				Alert( const char * inError, const char * inInfo );

		
	protected:

			std::string		mOKText;
			std::string 	mCancelText;
			std::string 	mOtherText;

				bool		mShowOK;
				bool		mShowCancel;
				bool		mShowOther;
				int			mDefaultButton;
};

#endif
