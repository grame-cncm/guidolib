#ifndef ARFactory_H
#define ARFactory_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003  Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "kf_ilist.h"	// required
// #include "defines.h"

#include "ARDefine.h"
// #include "GRDefine.h"
#include "TagParameterList.h"

class ARMusic;
class ARMusicalVoice;
class ARNote;

class ARMusicalEvent;
class ARRepeatBegin;
class ARChord;
class ARMusicalObject;
class ARMusicalTag;
class AROctava;
class ARTStem;
class ARNoteFormat;
class ARRestFormat;
class ARDotFormat;
class ARAlter;
class ARGrace;
class ARCue;
class ARTrill;
class ARStaff;
class ARTHead;


// typedef KF_IPointerList<ARMusicalObject> ARMusicalObjectList;
typedef KF_IPointerList<ARMusicalTag> TagPointerList; // todo: replace by stl container (easy)


/** \brief Class used in order to build the abstract representation (AR) structure
	of music.
*/


class ARFactory
{
 	public:

		enum		bmstate { BEAMSAUTO, BEAMSOFF, BEAMSEXPLICIT };
		
					ARFactory();
	virtual 		~ARFactory();

		void 		createMusic();
		ARMusic * 	getMusic();

		void 		createVoice();
		void 		addVoice();

		void 		createChord();
		void 		addChord();

	  	void 		initChordNote();

		void 		createEvent( const char * name );
		void 		addEvent();

		void 		addSharp();
		void 		addFlat();

		void		setPoints( int pointCount );
		void 		setAccidentals( TYPE_ACCIDENTALS accidentals );

		void 		setRegister(TYPE_REGISTER newRegister);

		void 		setNumerator(int newNumerator);
		void 		setDenominator(int newDenominator);

		void 		createTag( TYPE_TAG_NAME_CONST name, int no = -1) ;

					// manages the tag-range. will be called if the tag has a range
		void 		tagRange();

					// for the closing bracket ")"
					// the last tag will be popped from the stack and added
					// to the current voice
		void 		endTag();
		void 		addTag();



		void 		addTagParameter( const char * parameter );
		void 		addTagParameter( TYPE_TAGPARAMETER_INT parameter );
		void 		addTagParameter( TYPE_TAGPARAMETER_REAL parameter );
		void 		setTagParameterList( TagParameterList theTagParameterList );

	  	float 		UndoTransform( const float val );
	  	void 		setParameterName( const char * name );
	  	void 		setUnit( const char * s );
	  	
  private:

		ARMusicalVoice * 	mCurrentVoice;
		ARMusic * 			mCurrentMusic;
		ARMusicalEvent * 	mCurrentEvent;
		ARRepeatBegin *		mCurrentRepeatBegin;

		ARMusicalEvent * 	mLastEvent;
		TagPointerList 		mTags; // todo: replace by stl container
		TagParameterList 	mTagParameterList;

		int		 			mCurrentRegister;	// (octave)
		int 				mCurrentNumerator;
		int 				mCurrentDenominator;
		int				 	mCurrentIntensity;

		static long sMaxTagId;

protected:

	bmstate				mBeamState;
	AROctava *			mCurrentOctava;
	ARGrace *			mCurrentGrace;
	ARCue *				mCurrentCue;
	ARTrill *			mCurrentTrill;
	ARTStem *			mCurrentStem;
	ARTHead *			mCurrentHead;
	ARNoteFormat *		mCurrentNoteFormat;
	ARRestFormat *		mCurrentRestFormat;
	ARDotFormat *		mCurrentDotFormat;
	ARAlter *			mCurrentAlter;
	ARMusicalVoice *	mSaveCurrentVoice;
	ARStaff *			mCurrentStaff;
	int					mVoiceNum;
	int					mCurrentTags;
	bool				mTagRanged;
	bool				mVoiceAdded;
};

extern ARFactory * gGlobalFactory;

#endif


