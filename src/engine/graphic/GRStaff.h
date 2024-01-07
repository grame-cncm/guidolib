#ifndef GRStaff_H
#define GRStaff_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "GRCompositeNotationElement.h"
#include "GuidoDefs.h"

class ARBar;
class ARBarFormat;
class ARClef;
class ARDoubleBar;
class ARFinishBar;
class ARInstrument;
class ARInstrument;
class ARIntens;
class ARKey;
class ARMeter;
class ARMusicalVoiceState;
class AROctava;
class ARRepeatBegin;
class ARRepeatEnd;
class ARRepeatEndRangeEnd;
class ARStaffFormat;
class ARText;

class GRBar;
class GRClef;
class GRDoubleBar;
class GRFinishBar;
class GRGlue;
class GRInstrument;
class GRIntens;
class GRKey;
class GRMeter;
class GRNote;
class GROctava;
class GRPossibleBreakState;
class GRRepeatBegin;
class GRRepeatEnd;
class GRRod;
class GRSpring;
class GRStaffManager;
class GRSystem;
class GRSystemSlice;
class GRText;
class GRVoice;
class GRVoiceManager;

class TCollisions;

class TagParameterFloat;

template <class T> class KF_List;

typedef KF_List<float> AccList;
// typedef KF_IPointerList<GRTag> GRTagPointerList;
// typedef KF_Vector<float> UnitVect;

/** \brief Keeps information about accidentals at measure level.
*/
// DF - class introduced on nov. 4 2011 to cover octave changes
// and to comply to the practice described in "Behind Bars" E. Gould p. 79
class MeasureAccidentals
{
	enum	{ kPitchClasses = 12, kOctRange=10 };	// 12 pitch classes to differentiate 'is' notes 
	float	fAccidentals[kPitchClasses][kOctRange];	// keeps current accidentals at measure level
	float	fPClass[kPitchClasses];					// keeps current accidentals by pîtch class
	public:
				 MeasureAccidentals()	{ reset(); }
		virtual ~MeasureAccidentals() {}
		
		void	reset();
		void	setPitchClassAccidental(int pclass, float accidental);
		void	setAccidental(int pitch, int oct, float accidental);
		float	accidental (int pitch, int oct) const;
		float	accidental (int pitch) const;

		MeasureAccidentals & operator=(const MeasureAccidentals &ma);
};

/** \brief Keeps information about a staff.
*/
class GRStaffState
{
	friend class GRStaff;
	friend class GRStaffManager;
	friend class GRVoiceManager;
	friend class GRSystem;
	friend class GRSystemSlice;

	public:
		enum clefstate { CLEFAUTO, CLEFEXPLICIT, CLEFINTERN, CLEFOFF };

						 GRStaffState();
		virtual 		~GRStaffState();

		GRStaffState & operator=(const GRStaffState &tmp);

		GRStaffState &	getState()								{ return *this; }
		int				getInstrKeyNumber() const				{ return instrNumKeys; }
		void			reset2key();

        float getYOffset() const                                { return fYOffset; }
        const TagParameterFloat* getStaffDistance() const       { return staffDistance; }
    
        void setStaffLSPACE(float value)                        { staffLSPACE = value * 2; } // Factor 2 to be consistent with GRStaff::setStaffFormat(ARStaffFormat * staffrmt)
        void setMultiVoiceCollisions(bool state)                { fMultiVoiceCollisions = state; }
        bool multiVoiceCollisions() const                       { return fMultiVoiceCollisions; }
        const MeasureAccidentals& getMeasureAccidentals() const { return fMeasureAccidentals; }
        const GRInstrument* getRepeatInstrument() const 		{ return fInstrument; }

	private:
		// Meter-Parameters
		bool	meterset;		// flag if meter-sig is given . TRUE, FALSE

		const ARMeter * curmeter;
		const GRInstrument *  fInstrument = nullptr;	// instrument to be repeated on every system
		// Noteparameter
		bool	keyset;							// flag for if key-sig is given
		int		numkeys;						// number of accidentals, neede for GRKey.
		float	KeyArray[NUMNOTES];
		MeasureAccidentals	fMeasureAccidentals;

		const ARKey * curkey;
		// clef-Parameter
		clefstate	clefset;			// CLEFINTERN, CLEFEXPLICIT, CLEFAUTO, [ CLEFOFF ]
		std::string	clefname;			// clef name (same as for ARClef ...)
		int			basepit;
		int			basepitoffs;		// these two parameters are for
		int			instrNumKeys;		// instrument-tag:
				// basepitoffs: the offset for the given instrument, example: clarinet in A
				// has an offset of -2, it is two pitchclasses away from c-major
				// instrnumkeys: the number of keys for the transposed instrument ,,,
		float instrKeyArray[NUMNOTES];
				// the key-array for the current instrument (if transposed).

		int baseoct;
		int octava;
		int baseline;

		const ARClef *		curclef;
		const ARBarFormat *	curbarfrmt;
		const ARStaffFormat * curstaffrmt;

		float	staffLSPACE;	// distance between two lines
		int		numlines;		// Zahl der Notenlinien.
        float   lineThickness;
        float   fYOffset;
        const TagParameterFloat* staffDistance;
        unsigned char *colRef;
		float	distance;
		bool	distanceset;
		bool	fMultiVoiceCollisions = false;
		// this is VOICE-Stuff!
		// Stemdirection
		// stemstate stemset;  // STEMUP, STEMDOWN, STEMAUTO

		// This is voice-stuff!
		// Tuplets
		// int tupletset;
		// GRTupletState tplstate;

 };

/** \brief A GRStaff represents a single line of music.
*/

class GRStaff : public GRCompositeNotationElement
{
	friend class GRPossibleBreakState;
	friend class GRStaffState;
	friend class GRStaffManager;
	friend class GRVoiceManager;

	public:
						 GRStaff(GRSystemSlice * systemslice, float propRender);
		virtual 		~GRStaff();

		float           getDistance() const;
		GRSystem *      getGRSystem() const;
		GRSystemSlice * getGRSystemSlice() const;
		GRStaff *       getPreviousStaff() const;
		GRStaff *		getNextStaff() const;
		int             getStaffNumber() const;
		GRGlue *        getEndGlue() const;
		GRGlue *        getStartGlue() const;
		GRGlue *        getSecondGlue() const;
		int             getFontSize() const;
		float           getStaffLSPACE() const			{ return mStaffState.staffLSPACE; }
		float           getSizeRatio() const			{ return mStaffState.staffLSPACE / LSPACE; }
		const GRStaffState *    getStaffState() const	{ return &mStaffState; }
		GRStaffState &          getGRStaffState()		{ return mStaffState; }
		const GRStaffState &    getGRStaffState() const { return mStaffState; }
		float					getXEndPosition(TYPE_TIMEPOSITION pos, TYPE_DURATION dur) const;
        const ARMeter *			getCurMeter() const		{ return mStaffState.curmeter; }

		virtual float       getNotePosition(TYPE_PITCH pit, TYPE_REGISTER oct) const;
		virtual float 		getNotePosition(TYPE_PITCH pit, TYPE_REGISTER oct, int basePitch, int baseLine, int baseOct) const;
		virtual GDirection  getDefaultThroatDirection(TYPE_PITCH pit, TYPE_REGISTER oct) const;
		virtual int         getNumHelplines(TYPE_PITCH pit, TYPE_REGISTER oct) const;
		virtual VGColor     getNoteColor(TYPE_PITCH pit) const;
		virtual int         getNumlines() const			{ return mStaffState.numlines; }
        virtual unsigned char* getStffrmtColRef() const { return mStaffState.colRef; }
        virtual float       getLineThickness() const	{ return mStaffState.lineThickness; }
		virtual	float       getDredgeSize() const;
        virtual float       getMappingDredgeSize() const;
		virtual float       getKeyPosition(TYPE_PITCH pitch, int numkeys = 1) const;
		virtual GRNote *    getLastNote() const;
		const GRNote *		getFirstNote() const;
		virtual GRBar *     getLastBar() const;
		virtual void		print(std::ostream& os) const;
		virtual void		accept (GRVisitor& visitor);

		virtual void setStaffFormat (const ARStaffFormat * staffrmt);
		void    setStaffState		(GRStaffState * state);
		void    setInstrumentFormat	(const GRStaffState & state);
		void    setBarFormat		(const ARBarFormat * barfrmt);
		void    setNoteParameters	(const GRNote * inNote );
		void    setKeyParameters	(GRKey * inKey);
		void    setDistance			(float distance)	{ mStaffState.distanceset = true; mStaffState.distance = distance; }
		void    setEndPosition		(float newendpos)	{ mLength = newendpos - mPosition.x; }
		void    setLength			(float newlength)	{ mLength = newlength; }

		GROctava *			AddOctava		(const AROctava * aroct);
		void                AddSecondGlue	(GRGlue * myglue);
		void                AddElementAt	(GuidoPos pos,GRNotationElement * el);
		GRRepeatBegin *     AddRepeatBegin	(ARRepeatBegin *);
		GRRepeatEnd *       AddRepeatEnd	(ARRepeatEnd *);
		GRDoubleBar *       AddDoubleBar	(ARDoubleBar * ardbar,const TYPE_TIMEPOSITION & tp);
		GRFinishBar *       AddFinishBar	(ARFinishBar * arfbar,const TYPE_TIMEPOSITION & tp);
		GRIntens *          AddIntens		(const ARIntens * aintens);
		GRText *            AddText			(const ARText * atext);
		GRKey *             AddKey			(const ARKey * arkey);
		GRBar *             AddBar			(ARBar * abar, const TYPE_TIMEPOSITION & von);
		GRMeter *           AddMeter		(const ARMeter * armeter);
		void                AddTag			(GRNotationElement * grtag);
		GRClef *            AddClef			(const ARClef *);
		GRInstrument *      AddInstrument	(const ARInstrument *);

		virtual AccList * askAccidentals(TYPE_PITCH p_pit, TYPE_REGISTER p_oct, int p_acc, float detune=0.f);

		virtual void	boundingBoxPreview();
		virtual void	updateBoundingBox();
		virtual void	FinishStaff();
		virtual float	FirstNoteORRestXPos() const;

		void        UpdateStaffState(GuidoPos pos);
		void        TellNewStaff(GuidoPos pos);
		void        createNewRods(GRStaffManager * staffmgr, int & startspr, int & endspr, float optForce);
		void        CreateBeginElements(GRStaffManager * staffmgr,GRStaffState & state, int staffnum);
		GuidoPos    EndStaff(const TYPE_TIMEPOSITION & tp, GRStaffManager * staffmgr,
							GuidoPos endpos = 0, int lastline = 0);

		void        EndStaff(const TYPE_TIMEPOSITION & tp,  GRSpring * spr);
		void        BeginStaff(GRStaffManager * staffmgr);
		void        addElementsToSprings();

		virtual void OnDraw( VGDevice & hdc ) const;
		virtual void GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const;
				void		setOnOff(bool onoff, TYPE_TIMEPOSITION tp);
		void		setOnOff(bool onoff);
		bool		isStaffBeginOn();
		bool		isStaffEndOn();
		void		setNextOnOff(bool onoff){isNextOn = onoff;}
		bool		isNextStaffOn(){return isNextOn;}
		
		// this function was defined as private previously. Because GRNoteFactory needs to add elements 
		// to the staff (Tuplets), it must be defined public.

		// Dies function will be very powerfull  ...
		// It must do the part of createStaffElements and call createCutStaffElement aufrufen ...
		// the old functionality will be lost
		void addNotationElement(GRNotationElement * notationElement);

		void	checkSystemBar(const TYPE_TIMEPOSITION & tp);

		void	generatePositions();
		float	currentLineThikness() const;
        
		float	getProportionnalRender() const { return this->fProportionnalRendering; }
		float	getStaffBottom () const;
		void	checkCollisions (TCollisions& state) const;
		void	checkMultiVoiceNotesCollision ();
		float	getNotesDensity () const;
		size_t	getLyrics (std::vector<const GRNotationElement*>& list) const;
		void	inhibitNextReset2Key()		{ fInhibitNextReset2key = true; }

  protected:
		void	DebugPrintState(const char * info) const;

		void	DrawStaffUsingSymbolScale( VGDevice & hdc ) const;
		void	DrawStaffUsingSymbolRepeat( VGDevice & hdc ) const;
		void	DrawStaffUsingLines( VGDevice & hdc ) const;
		void	DrawNotationElements( VGDevice & hdc ) const;

		void	setClefParameters(	GRClef * grclef, GRStaffState::clefstate cstate = GRStaffState::CLEFAUTO );
		void	setMeterParameters( GRMeter * grmeter );
    
        void    setStaffSize( float size )  { mStaffState.setStaffLSPACE(size); }

		GuidoPos 		lastrodpos;
		GRRod * 		lastrod;
	  	GRRod * 		firstrod;
	  	GRGlue * 		endglue;
	  	GRGlue * 		startglue;
	  	GRGlue * 		secglue;
		GRSystem * 		mGrSystem;
		GRSystemSlice * mGrSystemSlice;

	  	float     		mLength;
		GRStaffState 	mStaffState;

		// for the STEM-Auto calculation
		float 			avg_position;
		int 			mNoteCount;
		
 	 private:
		TYPE_TIMEPOSITION	fLastSystemBarChecked;
		void newMeasure(const TYPE_TIMEPOSITION & tp, bool reset=true);
		
		std::map<TYPE_TIMEPOSITION, bool> fOnOffList;
		std::map<float, float> fPositions;
		bool			isNextOn;
		bool			firstOnOffSetting;
		
		bool			fInhibitNextReset2key = false;
		float			fProportionnalRendering;
};

std::ostream& operator<< (std::ostream& os, const GRStaff& staff);
std::ostream& operator<< (std::ostream& os, const GRStaff* staff);

#endif


