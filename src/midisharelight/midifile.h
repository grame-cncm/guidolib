/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/ 
 
#ifndef __MIDIFILE_H__
#define __MIDIFILE_H__

#include <stdio.h>
#include "MidiShareLight.h"

#define nil 0

#if defined(linux) || defined(__MACH__)
# define MFAPI
# define errno  /* a revoir */

#elif defined(WIN32)
# ifdef __BuildDLL__
#  ifdef __BuildLib__
#	define MFAPI	__declspec(dllexport)
#  else
#	define MFAPI	__declspec(dllimport)
#  endif
# else
#	define MFAPI
#endif

#else
#error "architecture undefined"
#endif

#ifdef __x86_64__
 typedef int	int32;
 typedef short	int16;
#else
 typedef long	int32;
 typedef short	int16;
#endif

/*--------------------------------------------------------------------------*/
extern int MidiFile_errno;		/* used similarly to errno				*/
								/* to read just in error case, if the	*/
								/* code is equal to MidiFileNoErr, look	*/
								/* at errno	*/

/*--------------------------------------------------------------------------*/
/* implemented file types			 										*/

enum { midifile0= 0, midifile1, midifile2};


/*--------------------------------------------------------------------------*/
/* opening modes for existing files											*/
#define MidiFileRead	0			/* reading the file				*/
#define MidiFileAppend	1			/* appening to the file			*/


/*--------------------------------------------------------------------------*/
/* time definition															*/

#define TicksPerQuarterNote	0
#define Smpte24				24
#define Smpte25				25
#define Smpte29				29
#define Smpte30				30


/*--------------------------------------------------------------------------*/
/* meta events types 														*/
#define	META			0xFF		/* meta event header code			*/

#define MDF_NumSeq		0			/* sequence number					*/
#define MDF_Texte		1			/* texte message					*/
#define MDF_Copyright	2			/* copyright message 				*/
#define MDF_SeqName		3			/* sequence or track name			*/
#define MDF_InstrName	4			/* instrument name					*/
#define MDF_Lyric		5			/* lyrics							*/
#define MDF_Mark		6			/* marker							*/
#define MDF_CuePt		7			/* cue point						*/
#define MDF_ChanPref	0x20		/* MIDI Channel Prefix				*/
#define MDF_PortPref	0x21		/* MIDI Port Prefix					*/
#define MDF_EndTrk		0x2F		/* end track						*/
#define MDF_Tempo		0x51		/* tempo change						*/
#define MDF_Offset		0x54		/* smpte offset						*/
#define MDF_Meas		0x58		/* time signature					*/
#define MDF_Ton			0x59		/* key signature					*/
#define MDF_Extra		0x7F		/* sequencer specific meta event	*/

/* length of the differents meta events */
#define MDF_NumSeqLen	2
#define MDF_ChanPrefLen 1
#define MDF_PortPrefLen 1
#define MDF_EndTrkLen	0
#define MDF_TempoLen	3
#define MDF_OffsetLen	5
#define MDF_MeasLen		4
#define MDF_TonLen		2

/*--------------------------------------------------------------------------*/
/* datas structures															*/
/*--------------------------------------------------------------------------*/

typedef struct MDF_Header{			/* the file header structure	*/
	char 	id[4];					/* header id					*/
	int32	len;					/* datas length ( = 6 )			*/
	int16	format;					/* file format					*/
	int16	ntrks;					/* number of tracks				*/
	int16	time;					/* time representation			*/
}MDF_Header;


typedef struct MDF_Trk{				/* track header structure		*/
	char	id[4];					/* header id					*/
	int32	len;					/* datas length 				*/
}MDF_Trk;

typedef struct midiFILE{			/* MIDI file descriptor		*/
	MidiLight* fMidi;
	short 	format;					/* file format					*/
	unsigned short ntrks;			/* number of tracks					*/
	short	time;					/* time representation			*/
					/* for MIDI time: tick count per quarter note           */
					/* for smpte time: b. 15  = 1                           */
					/*                 b.8-14 = frame count per sec         */
					/*			       b.0-7  = tick count per frame        */
	FILE 	*fd;					/* standard file descriptor             */	
	long  	trkHeadOffset;			/* track header offset                  */
									/* nil if the track is closed           */
	long	_cnt;					/* count for end track detection		*/
	MidiSeqPtr keyOff;				/* keyOff coming from typeNote events   */
	long 	curDate;				/* current date 						*/
	bool mode;					/* 0/1 : reading/writing                */
	bool opened;					/* flag for opened track 				*/
} midiFILE;

typedef midiFILE * MIDIFilePtr;

typedef struct MDF_versions{
	short	src;					/* source code version 					*/
	short 	MidiFile;				/* MIDI file format version				*/
}MDF_versions;


/*--------------------------------------------------------------------------*/
/* error codes								 								*/

#define MidiFileNoErr		0		/* everything is ok						*/
#define MidiFileErrFrmt		(-1)	/* internal datas format error			*/
#define MidiFileErrEvs		(-2)	/* MidiShare memory exhausted			*/
#define MidiFileErrNoTrack	(-3)	/* the track doesn't exists				*/
#define MidiFileErrAdd0		(-4)	/* try to add to a format 0	MIDI file 	*/
#define MidiFileErrMode		(-5)	/* uncorrect file opening mode			*/
#define MidiFileErrUnknow	(-6)	/* unknown event type					*/
#define MidiFileErrReset	(-7)	/* try to write a reset event			*/

#define MidiFileErrTrackClose	(-8)	/* the track is closed				*/

/*--------------------------------------------------------------------------*/
/* some macros															*/

#ifdef __cplusplus
inline short smpte(MDF_Header *f)					{ return ((f)->time & 0x8000); }
inline short frame_par_sec(MDF_Header *f)			{ return (((f)->time & 0x8000) >> 8); }
inline short ticks_par_frame(MDF_Header *f)			{ return ((f)->time & 0xFF); }
inline short ticks_par_quarterNote(MDF_Header *f)	{ return (f)->time; }
#endif

#define MDF_Header_SIZE 14
#define MDF_Trk_SIZE 	8


/*--------------------------------------------------------------------------*/
/* functions declarations													*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32

MFAPI const MDF_versions * MidiFileGetVersion(void);
#else

const MDF_versions * MFAPI MidiFileGetVersion(void);

#endif

class MIDIFile {
	midiFILE fMidiFile;

	bool Init();
	bool ErrOpen();
	bool isTrackOpen() const			{ return fMidiFile.opened; }
	bool isMidiFileError() const		{ return MidiFile_errno!=MidiFileNoErr || ferror(fMidiFile.fd) || feof(fMidiFile.fd); }
	bool WriteEv( MIDIFilePtr fd, MidiEvPtr ev);
	bool FlushKeyOff( MIDIFilePtr fd);
	MidiEvPtr ReadEv (MIDIFilePtr fd);
	
	public:
					 MIDIFile();
		virtual		~MIDIFile();
		
		bool		Open  (const char *filename, short mode);
		bool		Create(const char *filename, short format, short timeDef, short ticks);
		bool		Close ();

		/*-------------------------------- tracks management -----------------------*/
		bool		OpenTrack  ();
		bool		NewTrack   ();
		bool		CloseTrack ();
		bool		ChooseTrack(short trackIndex);

		/*-------------------------------- functions to read -----------------------*/
		MidiEvPtr	ReadEv    ();
		MidiSeqPtr	ReadTrack ();

		/*-------------------------------- functions to write ----------------------*/
		bool		WriteEv   (MidiEvPtr ev);
		bool		WriteTrack(MidiSeqPtr seq);

		/*-------------------------------- error codes access ----------------------*/
		int			GetMFErrno (void);
		int			GetErrno   (void);
		
		const midiFILE&	infos() const	{ return fMidiFile; }
		MidiLight*		midi()			{ return fMidiFile.fMidi; }
};


#ifdef __cplusplus
}
#endif

#endif
