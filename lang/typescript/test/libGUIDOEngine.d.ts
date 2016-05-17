declare var Module: any; 

// INTERFACE REQUIERE BY GUIDOENGINE
//--------------------------------------------------------------
interface GuidoInitDesc {
    graphicDevice:  VGDevice;
    reserved:       void;
    musicFont:      string;
    textFont:       string;
}

interface GuidoDate {
    num:    number;
    denom:  number;
}

interface GuidoPageFormat {
    width:          number; 
    height:         number; 
    marginleft:     number; 
    margintop:      number; 
    marginright:    number; 
    marginbottom:   number;
}

interface GuidoLayoutSettings {
	systemsDistance:        number;
	systemsDistribution:    number;
	systemsDistribLimit:    number;
	force:                  number;
    spring:                 number;
	neighborhoodSpacing:    number;
	optimalPageFill:        number;
	resizePage2Music:       number;
	proportionalRenderingForceMultiplicator: number;
}

interface GuidoOnDrawDesc {
    handle:         GRHandler;
    hdc:            VGDevice;
    page:           number;
	updateRegion:   GPaintStruct ;
    scrollx:        number; 
    scrolly:        number;
    reserved:       number;
    sizex:          number;
    sizey:          number;
    isprint:        number;
}

interface GuidoVersion {
    major:  number;
    minor:  number;
    sub:    number;
    str:    string;
}

interface ParserError {
    line:   number;
    col:    number;
    msg:    string;
}

interface GPaintStruct {
    erase:  boolean;
    left:   number;
	top:    number;
	right:  number;
	bottom: number;
}

// Handlers
//------------------------------
interface ARHandler     {}
interface GRHandler     {}

// Classes
//------------------------------
interface GuidoParser   {}
interface GuidoStream   {}
interface VGDevice      {}


// ENUMS
//--------------------------------------------------------------
declare enum GuidoMapping {
    kNoMapping     =  0,
    kVoiceMapping  =  1,
    kStaffMapping  =  1<<1,
    kSystemMapping =  1<<2
}

declare enum GuidoErrCode {
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
	guidoErrActionFailed		= -10,
}

// GUIDOENGINE
//--------------------------------------------------------------
interface GuidoEngineAdapter {
    constructor: GuidoEngineAdapter;
        
    init        (desc?: GuidoInitDesc): GuidoErrCode;
    shutdown    (): void;
    
	ar2gr       (ar: ARHandler, settings?: GuidoLayoutSettings): GRHandler;	
    updateGR    (gr: GRHandler, settings?: GuidoLayoutSettings): GuidoErrCode;

	freeAR      (ar: ARHandler): void;
	freeGR      (gr: GRHandler): void;
    
    getErrorString              (errCode: GuidoErrCode): string;	
	getDefaultLayoutSettings    (): GuidoLayoutSettings; 
    	
    countVoices     (inHandleAR: ARHandler): number;
	
    getPageCount    (inHandleGR: GRHandler): number;	
    getSystemCount  (inHandleGR: GRHandler, page: number): number;

    duration        (inHandleGR: GRHandler): GuidoDate;

	findEventPage   (inHandleGR: GRHandler , date: GuidoDate): number;
	findPageAt      (inHandleGR: GRHandler, date: GuidoDate): number;

    getPageDate     (inHandleGR: GRHandler, pageNum: number): GuidoDate;
		
	onDraw          (desc: GuidoOnDrawDesc): GuidoErrCode;

    gr2SVG  (handle: GRHandler, page: number, embedFont: boolean, mappingMode: number): string;

    abstractExport  (handle: GRHandler, page: number): string;

    binaryExport    (handle: GRHandler, page: number): string;

	javascriptExport(handle: GRHandler, page: number): GuidoErrCode ;

	setDrawBoundingBoxes    (bbMap: number): void;
	getDrawBoundingBoxes    (): number;

    getPageFormat           (inHandleGR: GRHandler, pageNum: number): GuidoPageFormat;
    setDefaultPageFormat    (format: GuidoPageFormat): void;
	getDefaultPageFormat    (): GuidoPageFormat; 

    unit2CM     (val: number): number;
	cm2Unit     (val: number): number;
	unit2Inches (val: number): number;
    inches2Unit (val: number): number;

    resizePageToMusic   (inHandleGR: GRHandler): GuidoErrCode;
		
	getVersion          (): GuidoVersion;
    checkVersionNums    (major: number, minor: number, sub: number): GuidoErrCode;

	getLineSpace        (): number;

    markVoice   (inHandleAR: ARHandler, voicenum: number,
                date: GuidoDate, duration: GuidoDate,
                red: string, green: string, blue: string ): GuidoErrCode;

    setSymbolPath   (inHandleAR: ARHandler, inPaths: Array<string>): GuidoErrCode;
    getSymbolPath   (inHandleAR: ARHandler): Array<string>;

    getParsingTime  (ar: ARHandler): number;
	getAR2GRTime    (gr: GRHandler): number;
    getOnDrawTime   (gr: GRHandler): number;

    openParser      (): GuidoParser;
    closeParser     (p: GuidoParser): GuidoErrCode;

    file2AR     (parser: GuidoParser, file: string): ARHandler;
    string2AR   (parser: GuidoParser, gmnCode: string): ARHandler;

    getStream   (gStream: GuidoStream): string;
    stream2AR   (p: GuidoParser,stream: GuidoStream): ARHandler;

    parserGetErrorCode (p: GuidoParser): ParserError;

    openStream      (): GuidoStream;
    closeStream     (s: GuidoStream): GuidoErrCode;
    writeStream     (s: GuidoStream, str: string): GuidoErrCode;
	resetStream     (s: GuidoStream): GuidoErrCode;
}
