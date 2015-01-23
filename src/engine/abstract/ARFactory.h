#ifndef ARFactory_H
#define ARFactory_H

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

class ARMusic;
class ARMusicalVoice;
class ARNote;

class ARMusicalEvent;
class ARRepeatBegin;
class ARMusicalObject;
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
class ARCluster;
class ARGlissando;
class ARCrescendo;
class ARDiminuendo;
class ARTremolo;
class ARChordTag;
class ARTuplet;


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

		virtual void 		createMusic();
		virtual ARMusic * 	getMusic();

		virtual void 		createVoice();
		virtual void 		addVoice();

		virtual void 		createChord();
		virtual void 		addChord();

	  	virtual void 		initChordNote();

		virtual void 		createEvent( const char * name );
		virtual void 		addEvent();

		virtual void 		addSharp();
		virtual void 		addFlat();

		virtual void		setPoints( int pointCount );
		virtual void 		setAccidentals( TYPE_ACCIDENTALS accidentals );

		virtual void 		setRegister(TYPE_REGISTER newRegister);

		virtual void 		setNumerator(int newNumerator);
		virtual void 		setDenominator(int newDenominator);

		virtual void 		createTag( TYPE_TAG_NAME_CONST name, int no = -1) ;

					// manages the tag-range. will be called if the tag has a range
		virtual void 		tagRange();

					// for the closing bracket ")"
					// the last tag will be popped from the stack and added
					// to the current voice
		virtual void 		endTag();
		virtual void 		addTag();



		virtual void 		addTagParameter( const char * parameter );
		virtual void 		addTagParameter( TYPE_TAGPARAMETER_INT parameter );
		virtual void 		addTagParameter( TYPE_TAGPARAMETER_REAL parameter );
		virtual void 		setTagParameterList( TagParameterList theTagParameterList );

	  	virtual float 		UndoTransform( const float val );
	  	virtual void 		setParameterName( const char * name );
	  	virtual void 		setUnit( const char * s );

        virtual void        setFilePath(const char* inFilePath) { mFilePath = inFilePath; }
  	
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
	ARCluster *         mCurrentCluster;
    ARTremolo *         mCurrentTremolo;
    ARChordTag *        mCurrentChordTag;
    ARTuplet *          mCurrentTuplet;
	int					mVoiceNum;
	int					mCurrentTags;
	bool				mTagRanged;
	bool				mVoiceAdded;

    NVstring mFilePath;
};

#endif


