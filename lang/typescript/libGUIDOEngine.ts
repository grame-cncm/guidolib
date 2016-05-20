
enum GuidoMapping {
    kNoMapping     =  0,
    kVoiceMapping  =  1,
    kStaffMapping  =  1<<1,
    kSystemMapping =  1<<2
}

enum GuidoErrCode {
	//! null is used to denote no error
    guidoNoErr					= 0,
	//! error while parsing the Guido format
	guidoErrParse				= -1,
	//! memory allocation error
	guidoErrMemory				= -2,
	//! error while reading or writing a file
	guidoErrFileAccess			= -3,
	//! the user cancelled the action
	guidoErrUserCancel			= -4,
	//! the music font is not available
	guidoErrNoMusicFont			= -5,
	//! the text font is not available
	guidoErrNoTextFont			= -6,
	//! bad parameter used as argument
	guidoErrBadParameter		= -7,
	//! invalid handler used
	guidoErrInvalidHandle		= -8,
	//! required initialisation has not been performed
	guidoErrNotInitialized		= -9,
	//! the action failed
	guidoErrActionFailed		= -10
}

enum GuidoElementSelector {
	kGuidoPage, 
	kGuidoSystem, 
	kGuidoSystemSlice, 
	kGuidoStaff, 
	/*kGuidoMeasure,*/ 
	kGuidoBar, 
	kGuidoEvent, 
	kGuidoScoreElementEnd
}

enum GuidoElementType { kNote = 1, kRest, kEmpty, kBar, kRepeatBegin, kRepeatEnd, kStaff, kSystemSlice, kSystem, kPage } 

enum PianoRollType { kSimplePianoRoll, kTrajectoryPianoRoll }
