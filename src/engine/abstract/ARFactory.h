#ifndef ARFactory_H
#define ARFactory_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003-2017  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <stack>

#include "kf_ilist.h"	// required

#include "ARDefine.h"
#include "ARMusicalTag.h"
#include "ARMusicalVoice.h"
#include "ARAuto.h"

class ARAlter;
class ARChordTag;
class ARCluster;
class ARCrescendo;
class ARCue;
class ARDiminuendo;
class ARDotFormat;
class ARGlissando;
class ARGrace;
class ARKey;
class ARMeter;
class ARMusic;
class ARMusicalEvent;
class ARMusicalObject;
class ARMusicalVoice;
class ARNote;
class ARNoteFormat;
class AROctava;
class ARRepeatBegin;
class ARRestFormat;
class ARStaff;
class ARTHead;
class ARTremolo;
class ARTrill;
class ARTStem;
class ARTuplet;

typedef KF_IPointerList<ARMusicalTag> TagPointerList; // todo: replace by stl container (easy)


/** \brief Class used in order to build the abstract representation (AR) structure
	of music.
*/
class ARFactory
{
 	public:
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
		virtual void 		createTab( int string, const char * disp );
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

		virtual void 		addTagParameter( TagParameter * parameter );
		virtual void 		addTagParameter( const char * parameter );
		virtual void 		addTagParameter( TYPE_TAGPARAMETER_INT parameter );
		virtual void 		addTagParameter( TYPE_TAGPARAMETER_REAL parameter );

	  	virtual float 		UndoTransform( const float val );
	  	virtual void 		setParameterName( const char * name );
	  	virtual void 		setUnit( const char * s );

        virtual void        setFilePath(const char* inFilePath) { mFilePath = inFilePath; }
  	
  private:

		enum { kNoBeam = -99999999 };

		ARMusicalVoice * 	mCurrentVoice = NULL;
		ARMusic * 			mCurrentMusic = NULL;
		ARMusicalEvent * 	mCurrentEvent = NULL;
		ARRepeatBegin *		mCurrentRepeatBegin = NULL;

		ARMusicalEvent * 	mLastEvent = NULL;
		TagPointerList 		mTags; // todo: replace by stl container
		TagParametersList	mTagParameters;
		TagParameter*		mLastTagParameter = 0;

		int		 			mCurrentRegister;	// (octave)
		int 				mCurrentNumerator;
		int 				mCurrentDenominator;
		int				 	mCurrentIntensity;
		int					mCurrentBeamID;

		void	checkTagEnd	( ARMusicalTag* tag);
		void	checkRange	( const ARMusicalTag* tag, const std::string& ) const;
		void	endTremolo	( ARMusicalTag * tag );
		void	endCue		( ARMusicalTag * tag );
		template <typename T> T* endStateTag (T * tag ) {
			T * tmp = dynamic_cast<T *>(tag->getEndTag());
			mCurrentVoice->AddTail(tmp);
			return tmp;
		}
	
		static long sMaxTagId;

protected:
	ARMusicalVoice *	mSaveCurrentVoice = NULL;
	ARCluster *			mCurrentCluster = NULL;
    ARTremolo *			mCurrentTremolo = NULL;
	ARTrill *			mCurrentTrill = NULL;

	const AROctava *	mCurrentOctava = NULL;
	const ARGrace *		mCurrentGrace = NULL;
	const ARCue *		mCurrentCue = NULL;
	const ARTStem *		mCurrentStem = NULL;
	const ARTHead *		mCurrentHead = NULL;
	const ARNoteFormat*	mCurrentNoteFormat = NULL;
	std::stack<const ARNoteFormat*>	fNoteFormats;
	const ARRestFormat*	mCurrentRestFormat = NULL;
	const ARDotFormat *	mCurrentDotFormat = NULL;
	const ARAlter *		mCurrentAlter = NULL;
	const ARStaff *		mCurrentStaff = NULL;
    const ARChordTag *  mCurrentChordTag = NULL;
    const ARTuplet *	mCurrentTuplet = NULL;
    const ARKey *		mCurrentKey = NULL;
    const ARMeter *		mCurrentMeter = NULL;
	int					mVoiceNum;
	int					mCurrentTags;
	bool				mTagRanged;
	bool				mVoiceAdded;
	bool				mAutoLyricsPos;
	bool				mAutoInstrPos;
	bool				mAutoIntensPos;
	bool				mAutoHideTiedAccidentals;
	ARAuto::position 	mFingeringPos;
	ARAuto::position 	mHarmonyPos;
	float				mFingeringSize;

private:
	int					mChordNotesCount;


    NVstring mFilePath;
};

#endif


