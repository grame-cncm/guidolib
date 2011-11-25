/*
**	File Name:	NOTEFLAG.H
**
**	Modification History:
**
**  02/10/97     noteflag.h     TNW      moved note bit flags to here.
**  04/24/97     noteflag.h     TNW      comments.
**  09/03/97     NOTEFLAG.H     RES      added EF_ENTRYDETAILS, XEF_ENTRYDETAILS
**  10/07/97 ===== shipped Finale Win 3.8.0 =====
**  10/10/97     NOTEFLAG.H     RES       made NOTEID() macros into inlines for C++
**  11/13/97 ===== shipped Finale Mac 3.8.0 =====
**  12/29/97     NOTEFLAG.H     RES       comments
**  12/31/97     NOTEFLAG.H     RES       dt_CN --> dt_CUEN
**  12/31/97     NOTEFLAG.H     RES       dt_AS --> dt_ALTS
**  12/31/97     NOTEFLAG.H     RES       dt_HH --> dt_HHOCKSPEC
**  12/31/97     NOTEFLAG.H     RES       dt_CD --> dt_CROSSSTAF
**  12/31/97     NOTEFLAG.H     RES       dt_BL --> dt_BMALT_PrimDownStem
**  12/31/97     NOTEFLAG.H     RES       dt_bL --> dt_BMALT_SecDownStem
**  12/31/97     NOTEFLAG.H     RES       dt_BH --> dt_BMALT_PrimUpStem
**  12/31/97     NOTEFLAG.H     RES       dt_bH --> dt_BMALT_SecUpStem
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  01/12/98     NOTEFLAG.H     RES       dt_Ts --> dt_TIEALTER_Start
**  01/12/98     NOTEFLAG.H     RES       dt_Te --> dt_TIEALTER_End
**  01/21/98     NOTEFLAG.H     CFC       TEXTDTAIL should really be called
**					LYRICDTAIL, or something like that. Add this one to the
**					to do list.
**  03/14/98     NOTEFLAG.H     RES       added XEF_ to extended entry flags
**  03/14/98     NOTEFLAG.H     RES       added XEF_SLASHGRACE
**  03/20/98     NOTEFLAG.H     RES       added XEF_FLATBEAM
**  04/04/98     NOTEFLAG.H     RES       BEATBIT --> BEAMBIT; it was a synonym
**						for BEAMBIT, but never accurate because beats are not
**						necessarily synonymous with beam groups, and yet it is
**						only really used for the latter
**  04/30/98     NOTEFLAG.H     RES       moved TCDBITS et al. from harmonic.h
**						to noteflag.h
**  04/30/98     NOTEFLAG.H     RES       removed #ifdef __cplusplus (always C++ now)
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  06/25/98 ===== shipped Finale Mac 4.0.0r1 =====
**  10/07/98     NOTEFLAG.H     RES       prefixed note flags with NF_
**  10/07/98     NOTEFLAG.H     RES       prefixed THR note flags with THR_
**  10/07/98     NOTEFLAG.H     RES       added NF_LEFTTREMOLO, NF_TREMOLO_BITS
**  10/07/98     NOTEFLAG.H     RES       SETBIT --> EF_SETBIT
**  10/07/98     NOTEFLAG.H     RES       prefixed note flags with NF_, THR
**						noteflags with THR_, entry flags with EF_
**  10/07/98     NOTEFLAG.H     RES       EF_TEXTDTAIL --> EF_LYRICDETAIL
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
**  12/22/98 ===== shipped Finale Mac 98c.r1 =====
**  01/06/99     NOTEFLAG.H     RES       EF_ICLEFBIT is no longer needed or used.
**						Basically, it was set as a temporary flag in TENTRY to
**						signal the drawing routines that a clef change occurred
**						there and that various positioning globals needed to
**						be recomputed (ylineadj, hmline, that sort of thing). But
**						over time, we removed the dependence of those globals on
**						the clef, so the bit is no longer needed.
*//*~*/
/*
$Log: NOTEFLAG.H,v $
Revision 1.1.1.1  2002/12/19 19:13:12  dfober
first import

** 
** 7     8/16/99 12:02p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 6     7/20/99 4:34p Rstokes
** ===== shipped Finale Mac 2000 =====
** 
** 5     6/09/99 12:31p Rstokes
** added RESTID and comments lost from last checkin
** 
** 4     6/07/99 4:29p Rstokes
** RESTID
 * 
 * 3     1/28/99 2:42p Rstokes
 * $Log keyword, plus possibly MSPEC/INSTUSED work although I'm trying to
 * keep that stuff in a separate check-in
*/

#ifndef _NOTEFLAG_H_
#define _NOTEFLAG_H_

/*################################################################################*/
/*
The Entry Flag
--------------

Definition pertaining to the flag register of an entry in the grafic file.
( gf.ent[e].ef)
*/

/*
note details (3/19/96 RES): Here are the records currently associated with
individual notes.

	record		tag				entry flag
	------		---				----------
	CROSSSTAF 	dt_CROSSSTAF	CROSUPBIT
	CUEN		dt_CUEN			NOTEDTAIL
	DOTOFFSET	dt_DOTOFFSET	SPECIALALTS
	TIEALTER	dt_TIEALTER_End, dt_TIEALTER_Start	SPECIALALTS
	HHOCKSPEC	dt_HHOCKSPEC	(controlled by hhockID in FRAMEATTS)
	PERFDATA	dt_PERFDATA		PRFMDATA

(NOTE: ALTS (dt_ALTS) is also controlled by the NOTEDTAIL entry flag, but it is
an entry detail, not a note detail.)
*/

// these should be prefixed with EF_

//#ifdef SETBIT			// defined by Ctree
//#undef SETBIT
//#endif
#define EF_SETBIT		0x80000000L // always set for validity
#define EF_NOTEBIT		0x40000000L // mask to check for note vs. rest
#define EF_CNTLRBIT		0x20000000L // mask if entry is a controller
#define EF_CNTLBIT		0x10000000L // mask if entry is a controlled entry

/* NOTE: LINKBIT is an anachronism. We will reserve this bit for use as an
expansion bit. */
/*#define LINKBIT			0x08000000L *//* mask to check to link */

#define EF_CHORDBIT		0x04000000L /* mask to check if chord symbol exists */
// 1/6/99 RES: EF_ICLEFBIT is no longer needed or used (but I cannot guarantee
// its state in existing files)
//#define EF_ICLEFBIT		0x02000000L /* mask for inter clef change occurances */
#define EF_FLOATREST	0x01000000L /* mask for rest which floats to midline */

#define EF_GRACENOTE	0x00800000L	/* mask for grace notes */
#define EF_NOTEDTAIL	0x00400000L /* mask for note detail */
#define EF_IMRKDTAIL	0x00200000L /* mask for note expressions */
#define EF_LYRICDETAIL	0x00100000L	/* mask for lyrics */
#define EF_TUPLSTART	0x00080000L	/* tuplet start */
#define EF_CTUPPARA		0x00040000L	/* "controlled tuplet parasite bit" --
									currently only passively supported (see
									REELDUR.C) */
#define EF_PRFMDATA		0x00020000L	/* performance data */
#define	EF_TREMOLO		0x00010000L	/* 7/16/91 -- reserved for future use.
									The idea is that setting this bit on an
									entry marks it as a tremolo. Details on
									how it is notated (number of beams,
									measured/unmeasured, position of right
									half of entry, etc.) would be
									contained in a new detail record. See
									also NF_RIGHTTREMOLO, NF_LEFTTREMOLO in the
									note flags. */
//#define OCTVCONT		0x00010000L	-- this bit is now available for re-use
//									(archaic)

#define EF_IGNOREBIT	0x00008000L
#define EF_BMEXTDTAIL	0x00004000L
#define EF_FLIPTIE		0x00002000L
#define EF_SPECIALALTS	0x00001000L
//#define BEATBIT		0x00000800L /* same as BEAMBIT */
#define EF_BEAMBIT		0x00000800L	/* mask for beatbit, used to determine beaming */
#define EF_SECBEAMBIT	0x00000400L /* mask for secondary beam breaks */
#define EF_CNTLCONTBIT	0x00000200L /* mask to continue controlled beaming.
									This bit is set on the first controlled
									entry after a controller to indicate it
									should be grouped with the previous
									controlled group. (This is currently
									only passively supported by
									Enigma/Finale). */
#define EF_FREEZSTEM	0x00000100L

#define EF_STEMDTAIL	0x00000080L /* mask for detail stemming: controls
									BMALT (primary beam angle upstem and
									downstem: dt_BMALT_PrimUpStem, dt_BMALT_PrimDownStem;
									secondary beam angle upstem and downstem:
									dt_BMALT_SecUpStem, dt_BMALT_SecDownStem),
									STEMDEF (custom stems: dt_STEMDEF_down, dt_STEMDEF_up),
									STHG (stem length and offset: dt_ST),
									BEAMSTUB (broken beam direction: dt_BEAMSTUB) */

#define EF_CROSUPBIT	0x00000040L /* mask for cross-stave upward placement */
#define EF_CROSDWBIT	0x00000020L /* mask for cross-stave downward placement (it is
									unclear whether this flag is used (3/19/96 RES)) */
#define EF_REVUPSTEMBIT	0x00000010L /* mask to reverse all upstems in the entry */
#define EF_REVDWSTEMBIT	0x00000008L /* mask to reverse all downstems in the entry */
#define EF_DBLSTEMBIT	0x00000004L /* mask for double stemming */
#define EF_SPLITBIT		0x00000002L /* mask for split stemming */
#define EF_UPSTEMBIT	0x00000001L /* mask for up or down stem (1 == up) */


#define EF_CBITS			(EF_CNTLRBIT|EF_CNTLBIT)
#define EF_UDCROSBITS		(EF_CROSUPBIT|EF_CROSDWBIT)

// 9/3/97 RES: These are the flags that have detail records associated with them
#define	EF_ENTRYDETAILS		(EF_CHORDBIT|EF_NOTEDTAIL|EF_IMRKDTAIL|EF_LYRICDETAIL|\
	EF_BMEXTDTAIL|EF_SECBEAMBIT|EF_STEMDTAIL|EF_CROSUPBIT|EF_CROSDWBIT|EF_PRFMDATA|\
	EF_SPECIALALTS|EF_TUPLSTART)

/*################################################################################*/

/*
The Extended Entry Flag
-----------------------

definitions for the new "extended entry flag" field (xef) in the entry:
*/

#define	XEF_STAFFEXPRDETAIL		0x0001
#define	XEF_TEMPBIT				0x0002	// used but not reset during
										// conversion of pre-3.0 files (it
										// is not reset for speed reasons).
										// The flag is available for other uses,
										// just don't assume a particular
										// state.
#define	XEF_CHECKACCIS			0x0004	// 3/6/92: this bit forces frame_tgf()
										// to recompute the NF_ACCIBITs when the
										// frame is loaded. This is done by
										// calling harm_const().
#define	XEF_ISTUPLET			0x0008	// 1/14/93: this bit is set by
										// relDur() in any any entry that is
										// part of a tuplet
#define	XEF_DUMMY				0x0010	// 5/7/93: a dummy entry (usually
										// a rest) used for alignment but which
										// may be stripped out if necessary
#define XEF_SMARTSHAPEDETAIL	0x0020	// 04/18/94: for smart shape entry
										// attachment
#define	XEF_NOLEGER 			0x0040	// 2/2/95 RES: if set, don't draw leger lines
										// for this entry (used for Penderecki-like
										// "highest note possible" stuff)
#define	XEF_ENTRY_IS_SORTED		0x0080  // 07/15/95 CFC: if set (which it should be most
										// of the time) the notes in the entry are sorted
										// in the normal way, for standard notation. If
										// not set, the notes are in some other ordering,
										// which for now would mean percussion notation.
										// hlorder() takes care of setting this bit.
#define	XEF_SLASHGRACE			0x0100	// 3/14/98 RES: if set and the entry is a
										// single grace note, it is slashed regardless
										// of global slash setting (the only reason
										// we don't slash beamed grace groups is
										// we don't have a means of determining
										// or adjusting the slash angle to work
										// properly with the beam)
#define	XEF_FLATBEAM			0x0200	// 3/20/98 RES

// 9/3/97 RES: These are the flags that have detail records associated with them
#define	XEF_ENTRYDETAILS	(XEF_STAFFEXPRDETAIL|XEF_SMARTSHAPEDETAIL)

/*################################################################################*/

/*
The Note Flag
-------------

Definition for the mapping of the note[] flags in an entry --
Graphic Harmonic Representation (GHR). Some of these bits are also
used in Transcription Harmonic Representation (THR) -- I have not had the
time to track down which. (RES)
*/

#define NF_SETBIT		0x80000000L		// always set for validity
#define NF_TSBIT		0x40000000L		// tie start
#define NF_TEBIT		0x20000000L		// tie end

#define NF_CROSSBIT		0x10000000L		// cross staff

#define NF_UPSECBIT		0x08000000L		// upstem second
#define NF_DWSECBIT		0x04000000L		// downstem second
#define NF_UDSECBITS	(NF_DWSECBIT|NF_UPSECBIT)

#define NF_UPSPBIT		0x02000000L // for split stems, set if the note goes on the upper stem
#define NF_ACCIBIT		0x01000000L /* indicates whether to show an accidental
									(will be recomputed during editing; to
									freeze this bit in place, see NF_FREEZEACCI
									below) */

#define NF_PARENACCI	0x00800000L 	// parenthesize accidental

//#define NEEDACCIBIT	0x00400000L		/* scratch pad used by edraw() */

/*					0x00200000L 	-- available
**					0x00400000L 	-- available (was NEEDACCIBIT)
**					be sure to reserve one bit for expansion!
*/

/* Note IDs range from 1 to TGFNN (currently 12). Zero means no id. Note IDs
	were introduced in Finale 2.x, so older files would have no id (0). We are
	now trying to enforce the protocal that all entries should have note ids.
*/

const FLAG_32 NF_TGFNID	= 0x001F0000L;	/* since entries can now have only 12 notes
								(at one time they were designed to have
								24), we don't really need five bits here.
								The high bit could be used for something else.
								(But be aware of RESTID below.) */

// 6/3/99 RES: RESTID is a special value for the note ID for an "unfloating" rest 
// (a rest that uses the first ENOTE for positioning info so it can be moved 
// away from the center line). We've used the special ID of 31 (highest possible
// value for some time now, but never got around to actually defining a constant
// for it.
const twobyte RESTID = 31;		

//------------------------------------------------------------------------------
/* vars to control the mapping of the note registers */

const FLAG_16 TCDBITS 	= 0xFFF0;
const FLAG_16 TCDRLBIT  = 0x0008;
const FLAG_16 TCDAMOUNT = 0x0007;
const twobyte TCDSHIFT	= 4;
const twobyte MAXhLEVEL = 4096; 		/* with twelve TCD bits, we have 4096
										possible TCD values (-2048 ... 2047) */
const twobyte hLEVELbreak = 2047;		/* the value at which TCDs go negative */

// Since these inlines are available in the EDK as well as to Finale, and since
// the EDK does not use ENOTE but uses EXNOTE instead, I reworked both the
// macros and the inlines to deal with the flag directly rather than take a
// reference to ENOTE. (10/10/97 RES)

inline twobyte GETNOTEID( const FLAG_32 flag )
{
	return (twobyte)(( flag & NF_TGFNID ) >> 16 );
}

inline FLAG_32 MAKENOTEID( const twobyte id )
{
	return (((FLAG_32)id << 16 ) & NF_TGFNID );
}

inline FLAG_32 SETNOTEID( FLAG_32 flag, const twobyte id )
{
	flag &= ~NF_TGFNID;
	flag |= MAKENOTEID( id );
	return flag;
}

/* Definitions for the low word in the note flag. This word used to be
reserved for the TCD, but the TCD was moved to a new field so these flags
are now available. */

#define	NF_RIGHTTREMOLO	0x00000001L		/* 7/16/91: reserved for future use.
							The idea is if the TREMOLO bit is set in the
							entry, this bit can be set for individual notes.
							All notes with this bit set will be notated as a
							new entry of the same duration, offset to the
							right, for a "fingered tremolo" effect. We may
							need an additional bit for this, since you might
							want a note on both sides of the tremolo. In this
							case it is probably better to create a detail for
							this information. */
#define	NF_FREEZEACCI	0x00000002L		/* 3/10/92: if set, freeze the state
										of NF_ACCIBIT (which is set/reset in
										harm_const()) */
#define	NF_PARENNOTE	0x00000004L		/* 4/22/93: parenthesize the note
										(reserved for future use) */
#define	NF_LEFTTREMOLO	0x00000008L		// 10/7/98 RES: See NF_RIGHTTREMOLO above
#define	NF_TREMOLO_BITS	(NF_RIGHTTREMOLO | NF_LEFTTREMOLO)

/* Since the organization of the TCD is theoretically editable by the user
(the infamous 'TCD watershed bit' is really the bit position of TCDRLBIT),
the masks are stored in variables defined in harmonic.h.
(7/16/91: TCD used to be stored in the low word of the note flag, but is
now a separate field)
(3/19/96 RES: The variables in harmonic.h are now constants; we decided some time
ago to abandon the theoretical editability of these structures.
*/

/*################################################################################*/

/*
Definition for the mapping of the not[] registers in an entry --
Transcription Harmonic Representation (THR)
*/

#define THR_NONHBIT		0x00800000L	// key number is nonharmonic (THR)
#define THR_NHUPRBIT	0x00400000L	// mask to check whether nonharmonic note
									// pertains to the diatonic note above it or
									// below it. If set then the note pertains to
									// the 'upper' diatonic note. Used only if
									// THR_NONHBIT is set. (THR)
#define THR_RESTOBIT	0x00200000L	// restorative accidental. (THR)

//#define THR_NACCIBIT   (~(THR_ACCIBIT|THR_RESTOBIT)) 	/* mask to negate any
//														accidentals on the note */

#define THR_SETBIT		EF_SETBIT
#define THR_ACCIBIT		NF_ACCIBIT
#define THR_HARMBITS	(THR_NONHBIT|THR_ACCIBIT|THR_NHUPRBIT|THR_RESTOBIT)

#define THR_HUPRBITS	(THR_NONHBIT|THR_NHUPRBIT)
#define THR_HUPRWABITS	(THR_NONHBIT|THR_NHUPRBIT|THR_ACCIBIT)

#define THR_HLWRBITS	THR_NONHBIT
#define THR_HLWRWABITS	(THR_NONHBIT|THR_ACCIBIT)


#endif	// _NOTEFLAG_H_
