#ifndef __MidiShareLight__
#define __MidiShareLight__

/*****************************************************************************
 *                                MIDI SHARE LIGHT									
 *----------------------------------------------------------------------------
 * This is a light version of MidiShare. It provides high level events and 
 * memory management only. 
 * Note that the memory manager is lock-free, like the standard MidiSahre memory manager.
 * Note also that the interface is a C++ interface.
 *----------------------------------------------------------------------------
 * 		      c GRAME 1989, 1990, 1991, 1992, 1999, 2010
 *	     [Yann Orlarey, Herve Lequay, Dominique fober, Stephane Letz]						
******************************************************************************/

/*******************************************************************************
 *						MIDISHARE EVENTS									
 *------------------------------------------------------------------------------
 * 	The listing below presents the different types of MidiShare handled events. 
 * 	This typology contains the whole of the standard Midi messages, plus specific 
 * 	messages such as typeNote corresponding to a note with its duration;  
 * 	or typeStream corresponding to a series of arbitrary bytes, possibly including 
 * 	data and status codes, sent directly without any processing; or typePrivate 
 * 	that are application private messages.
 * 	
 * 	All these codes may be used in the MidiNewEv function to allocate an event
 * 	of the desirable type and are accessible in an event evType field.
 *******************************************************************************/


#ifdef WIN32
# include <windows.h>
# ifdef MSEXPORT
#	define	MIDISHAREAPI __declspec(dllexport)
# else
#	define	MIDISHAREAPI __declspec(dllimport)
# endif
#else
#	define	MIDISHAREAPI
#endif

typedef unsigned char Byte;
typedef char * Ptr;
typedef unsigned long ulong;
typedef unsigned char uchar;

/*******************************************************************************
 * MIDISHARE EVENTS
 *------------------------------------------------------------------------------
 * The listing below presents the different types of MidiShare handled events. 
 * This typology contains the whole of the standard Midi messages, plus specific 
 * messages such as typeNote corresponding to a note with its duration;  
 * or typeStream corresponding to a series of arbitrary bytes, possibly including 
 * data and status codes, sent directly without any processing; or typePrivate 
 * that are application private messages.
 * 
 * All these codes may be used in the MidiNewEv function to allocate an event
 * of the desirable type and are accessible in an event evType field.
 *******************************************************************************/

#define typeNote             0   /* note with pitch, velocity and duration        */

#define typeKeyOn            1   /* Note On with pitch, velocity                  */
#define typeKeyOff           2   /* Note Off with pitch, velocity                 */
#define typeKeyPress         3   /* Poly Key Pressure with pitch and pressure     */
#define typeCtrlChange       4   /* Control Change with controller ID and value   */
#define typeProgChange       5   /* Program Change with program ID number         */
#define typeChanPress        6   /* Channel Pressure with pressure value          */
#define typePitchWheel       7   /* Pitch Bend Change with LSB and MSB values     */

#define typeSongPos          8   /* Song Position Pointer with LSB and MSB values */
#define typeSongSel          9   /* Song Select with song ID number               */
#define typeClock           10   /* Timing Clock                                  */
#define typeStart           11   /* Start                                         */
#define typeContinue        12   /* Continue                                      */
#define typeStop            13   /* Stop                                          */

#define typeTune            14   /* Tune Request                                  */
#define typeActiveSens      15   /* Active Sensing                                */
#define typeReset           16   /* System Reset                                  */

#define typeSysEx           17   /* System Exclusive (only data bytes)            */
#define typeStream          18   /* arbitrary midi bytes (data and status codes)  */
#define typePrivate         19   /* 19..127 Private events for applications internal use */

#define typeProcess        128   /* used by MidiShare for MidiCall and MidiTask   */
#define typeDProcess       129   /* used by MidiShare for MidiDTask               */
#define typeQuarterFrame   130   /* Midi time code quarter frame                  */

#define typeCtrl14b        131
#define typeNonRegParam    132
#define typeRegParam       133

#define typeSeqNum         134   /* sequence number           */
#define typeTextual        135   /* text event                */
#define typeCopyright      136   /* message copyright message */
#define typeSeqName        137   /* sequence or track name    */
#define typeInstrName      138   /* instrument name           */
#define typeLyric          139   /* lyrics                    */
#define typeMarker         140   /* marker                    */
#define typeCuePoint       141   /* cue point                 */
#define typeChanPrefix     142   /* Midi Channel Prefix       */
#define typeEndTrack       143   /* end of a track            */
#define typeTempo          144   /* changement de tempo       */
#define typeSMPTEOffset    145   /* smpte offset              */

#define typeTimeSign       146   /* time signature                 		*/
#define typeKeySign        147   /* signature tonale                    */
#define typeSpecific       148   /* sequencer specific meta event       */

#define typePortPrefix     149   /* Midi Port  Prefix       */
#define typeRcvAlarm       150   /* RcvAlam         		*/
#define typeApplAlarm      151   /* ApplAlam        		*/


#define typeReserved       152   /*152..254 reserved for future extensions */

#define typeDead           255   /* dead Task or DTask                     */



/******************************************************************************
* MIDI STATUS CODE
*******************************************************************************/

#define NoteOff        0x80
#define NoteOn         0x90
#define PolyTouch      0xa0
#define ControlChg     0xb0
#define ProgramChg     0xc0
#define AfterTouch     0xd0
#define PitchBend      0xe0
#define SysRealTime    0xf0
#define SysEx          0xf0
#define QFrame         0xf1
#define SongPos        0xf2
#define SongSel        0xf3
#define UnDef2         0xf4
#define UnDef3         0xf5
#define Tune           0xf6
#define EndSysX        0xf7
#define MClock         0xf8
#define UnDef4         0xf9
#define MStart         0xfa
#define MCont          0xfb
#define MStop          0xfc
#define UnDef5         0xfd
#define ActSense       0xfe
#define MReset         0xff



/******************************************************************************
* ERROR CODES
*------------------------------------------------------------------------------
* List of the error codes returned by some MidiShare functions.
*******************************************************************************/

#define MIDIerrSpace    -1   /* no space left in the freeList */
#define MIDIerrRefNum   -2   /* bad reference number          */
#define MIDIerrBadType  -3   /* bad event type                */
#define MIDIerrIndex    -4   /* bad access index (events)     */
#define MIDIerrEv       -5   /* event argument is nil         */
#define MIDIerrUndef    -6   /* event argument is undef       */

		
/******************************************************************************
* 							    EVENTS STRUCTURES							
*------------------------------------------------------------------------------
* All events are built using one or several fixed size cells (16 bytes)
* Most events use  one cell. Some other like SysEx events use several linked cells.
*******************************************************************************/
#define CELLSIZE		4*sizeof(void*)

/*------------------------ System Exclusive extension cell ----------------------*/

    typedef struct TMidiSEX *MidiSEXPtr;
    typedef struct TMidiSEX
    {
        MidiSEXPtr link;								/* link to next cell */
        Byte data[CELLSIZE - sizeof(MidiSEXPtr)];       /* data bytes     */
    }   TMidiSEX;

/*------------------------------ Private extension cell -------------------------*/

    typedef struct TMidiST *MidiSTPtr;
    typedef struct TMidiST
    {
		void* val[4];
    }   TMidiST;

/*------------------------- Common Event Structure ----------------------*/
#define HEADERSIZE	(sizeof(void*) + sizeof(unsigned long) + sizeof(void*))
#define DATASIZE	(CELLSIZE - HEADERSIZE)
    typedef struct TMidiEv *MidiEvPtr;
    typedef struct TMidiEv
    {
        MidiEvPtr   link;           /* link to next event   */
        unsigned long date;         /* event date (in ms)   */
        Byte        evType;         /* event type           */
        Byte        refNum;         /* sender reference number */
        Byte        port;           /* Midi port            */
        Byte        chan;           /* Midi channel         */
		
#ifdef __x86_64__
		/* padding for 64 bits and to ensure natural alignment */
		Byte		data[12 - sizeof(long)];
#endif
        
		union {                     /* info depending of event type : */
            struct {                /* for notes            */
                Byte pitch;         /* pitch                */
                Byte vel;           /* velocity             */
                int dur;			/* duration             */
            } note;

            struct {              /* for MidiFile time signature */
                Byte numerator;   /* numerator                   */
                Byte denominator; /* denominator as neg power    */
                                  /* of 2. (2= quarter note)     */
                Byte nClocks;     /* number of Midi clocks in    */
                                  /* a metronome click           */
                Byte n32nd;       /* number of 32nd notes in     */
                                  /* a Midi quarter note         */
            } timeSign;

            struct {            /* for MidiFile key signature  */
                char ton;       /* 0: key of C, 1: 1 sharp     */
                                /* -1: 1 flat etc...           */
                Byte mode;      /* 0: major 1: minor           */
            } keySign;
            
			struct {            /* for paramchg & 14-bits ctrl  */
            	short num;      /* param or ctrl num            */
            	short val;      /* 14-bits value                */
            } param;

            struct {            /* for MidiFile sequence number */
                int	 number;
            } seqNum;

			short shortFields[DATASIZE / sizeof(short)]; /* for 14-bits controlers		*/
            int longField[DATASIZE / sizeof(int)];

            long tempo;          /* MidiFile tempo in            */
								 /* microsec/Midi quarter note   */
            Byte data[DATASIZE]; /* for other small events       */
			MidiSEXPtr linkSE;   /* link to last sysex extension */
			MidiSTPtr linkST;    /* link to private extension    */
        } info;
    } TMidiEv;

/*------------------------------ sequence header ---------------------------*/

    typedef struct TMidiSeq *MidiSeqPtr;
    typedef struct TMidiSeq
    {
        MidiEvPtr first;        /* first event pointer  */
        MidiEvPtr last;         /* last event pointer   */
        Ptr undef1;
        Ptr undef2;
    }   TMidiSeq;


/*-------------------------------- input Filter -------------------------------*/

    typedef struct TFilter *MidiFilterPtr;
    typedef struct TFilter
    {
        char port[32];         /* 256 bits */
        char evType[32];       /* 256 bits */
        char channel[2];       /*  16 bits */
        char unused[2];        /*  16 bits */
    } TFilter;


/******************************************************************************
*                              FIELD MACROS
*------------------------------------------------------------------------------
* Somes macros to read and write event's fields 
*******************************************************************************/

#define Link(e)       ( (e)->link )
#define Date(e)       ( (e)->date )
#define EvType(e)     ( (e)->evType )
#define RefNum(e)     ( (e)->refNum )
#define Port(e)       ( (e)->port )
#define Canal(e)      ( (e)->chan )
#define Chan(e)       ( (e)->chan )
#define Pitch(e)      ( (e)->info.note.pitch )
#define Vel(e)        ( (e)->info.note.vel )
#define Dur(e)        ( (e)->info.note.dur )
#define Data(e)       ( (e)->info.data )
#define LinkSE(e)     ( (e)->info.linkSE )
#define LinkST(e)     ( (e)->info.linkST )

#define TSNum(e)      ( (e)->info.timeSign.numerator )
#define TSDenom(e)    ( (e)->info.timeSign.denominator )
#define TSClocks(e)   ( (e)->info.timeSign.nClocks )
#define TS32nd(e)     ( (e)->info.timeSign.n32nd )

#define KSTon(e)      ( (e)->info.keySign.ton )
#define KSMode(e)     ( (e)->info.keySign.mode )

#define Tempo(e)      ( (e)->info.tempo )
#define SeqNum(e)     ( (e)->info.seqNum.number )
#define ChanPrefix(e) ( (e)->info.data[0] )
#define PortPrefix(e) ( (e)->info.data[0] )

#define First(e)      ( (e)->first )
#define Last(e)       ( (e)->last )
#define FirstEv(e)    ( (e)->first )
#define LastEv(e)     ( (e)->last )


typedef void (* ApplyProcPtr)    ( MidiEvPtr e );

/*******************************************************************************
* 						MidiShare Light API									
*******************************************************************************/
class MSMemory;
class MidiLight {
	MSMemory* fMemory;
	void checkSpace ();

	public:
				 MidiLight(unsigned long memsize=5000);
		virtual ~MidiLight();

		float			GetVersion		(void);

		/*-------------------------- Events and memory managing -----------------------*/
		unsigned long 	FreeSpace	(void);						

		MidiEvPtr 		NewCell 		(void);			
		void 	  		FreeCell 		(MidiEvPtr e);					
		unsigned long 	TotalSpace 		(void);
		unsigned long 	GrowSpace 		(long n);

		MidiEvPtr 	NewEv 			(short typeNum);			
		MidiEvPtr 	CopyEv 			(MidiEvPtr e);			
		void 	  	FreeEv 			(MidiEvPtr e);					

		void 		SetField 		(MidiEvPtr e, long f, long v);
		long		GetField 		(MidiEvPtr e, long f);
		void 		AddField 		(MidiEvPtr e, long v);
		long 		CountFields 	(MidiEvPtr e);


		/*------------------------------- Sequence managing ---------------------------*/
		MidiSeqPtr	NewSeq			(void);				 			
		void 		AddSeq 			(MidiSeqPtr s, MidiEvPtr e);
		void 		FreeSeq 		(MidiSeqPtr s);		
		void 		ClearSeq 		(MidiSeqPtr s);			
		void 		ApplySeq 		(MidiSeqPtr s, ApplyProcPtr proc); 
	
};

#endif
