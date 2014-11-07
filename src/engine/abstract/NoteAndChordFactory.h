#ifndef NoteAndChordFactory_H
#define NoteAndChordFactory_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "kf_ilist.h"	// required
// #include "defines.h"

#include "ARDefine.h"
// #include "GRDefine.h"
#include "TagParameterList.h"
#include "ARMusicalTag.h"
#include "ARFactory.h"


/** \brief Class used in order to build the abstract representation (AR) structure
	of music.
*/


class NoteAndChordFactory : public ARFactory
{
 	public:

		enum		bmstate { BEAMSAUTO, BEAMSOFF, BEAMSEXPLICIT };
		
					NoteAndChordFactory();
     virtual 		~NoteAndChordFactory();


		virtual void 		createChord();
		virtual void 		addChord();

	  	virtual void 		initChordNote();

		virtual void 		createEvent( const char * name );
		virtual void 		addEvent();

		virtual void 		createTag( TYPE_TAG_NAME_CONST name, int no = -1) {}

		virtual void 		tagRange(){}

		virtual void 		endTag(){}
		virtual void 		addTag(){}

        virtual void        setVoice(ARMusicalVoice * voice) {mCurrentVoice = voice;}

		virtual void 		addTagParameter( const char * parameter ){}
		virtual void 		addTagParameter( TYPE_TAGPARAMETER_INT parameter ){}
		virtual void 		addTagParameter( TYPE_TAGPARAMETER_REAL parameter ){}
		virtual void 		setTagParameterList( TagParameterList theTagParameterList ){}

	  	virtual void 		setParameterName( const char * name ){}
	  	virtual void 		setUnit( const char * s ){}

        virtual void        setRegister(TYPE_REGISTER newRegister);

		virtual void 		setNumerator(int newNumerator);
		virtual void 		setDenominator(int newDenominator);
    
        virtual bool        isDone() {return mDone;}
    
  private:

		ARMusicalVoice * 	mCurrentVoice;
		ARMusicalEvent * 	mCurrentEvent;

		int		 			mCurrentRegister;	// (octave)
		int 				mCurrentNumerator;
		int 				mCurrentDenominator;
		int				 	mCurrentIntensity;
        bool                mDone;
        bool                mInChord;
};

#endif


