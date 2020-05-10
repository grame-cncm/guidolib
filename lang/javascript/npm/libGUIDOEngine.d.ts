declare var GuidoModule : any;

// GUIDOEngine
//--------------------------------------------------------------
declare enum GuidoMapping {}
declare enum GuidoErrCode {}

interface ARHandler     {}
interface GRHandler     {}

interface GuidoParser   {}
interface GuidoStream   {}
interface VGDevice      {}

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
	checkLyricsCollisions: number;
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

interface GuidoEngineAdapter {
    constructor: GuidoEngineAdapter;
        
    init        (desc?: GuidoInitDesc): GuidoErrCode;
    shutdown    (): void;
    
	ar2gr               (ar: ARHandler): GRHandler;
	ar2grSettings       (ar: ARHandler, settings: GuidoLayoutSettings): GRHandler;	    	
    updateGR            (gr: GRHandler): GuidoErrCode;
    updateGRSettings    (gr: GRHandler, settings: GuidoLayoutSettings): GuidoErrCode;
    showElement    		(gr: GRHandler, elt: number, status: boolean): GuidoErrCode;

	freeAR      (ar: ARHandler): void;
	freeGR      (gr: GRHandler): void;
    
    getParsingTime  (ar: ARHandler): number;
    getAR2GRTime    (gr: GRHandler): number;
    getOnDrawTime   (gr: GRHandler): number;

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
    gr2SVGColored (gr: GRHandler, page: number, r: number, g: number, b: number, embedFont: boolean) : string

    abstractExport  (handle: GRHandler, page: number): string;
    binaryExport    (handle: GRHandler, page: number): string;
	javascriptExport(handle: GRHandler, page: number): GuidoErrCode;

	setDrawBoundingBoxes    (bbMap: number): void;
	getDrawBoundingBoxes    (): number;

    getPageFormat           (gr: GRHandler, pageNum: number): GuidoPageFormat;
    setDefaultPageFormat    (format: GuidoPageFormat): void;
	getDefaultPageFormat    (): GuidoPageFormat; 

    unit2CM     (val: number): number;
	cm2Unit     (val: number): number;
	unit2Inches (val: number): number;
    inches2Unit (val: number): number;

    resizePageToMusic   (inHandleGR: GRHandler): GuidoErrCode;
		
	getVersion          (): GuidoVersion;
	getVersionStr       (): string;
    checkVersionNums    (major: number, minor: number, sub: number): GuidoErrCode;

	getLineSpace        (): number;

    markVoice   (inHandleAR: ARHandler, voicenum: number,
                date: GuidoDate, duration: GuidoDate,
                red: number, green: number, blue: number ): GuidoErrCode;

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

// GUIDOMap
//--------------------------------------------------------------
declare enum GuidoElementSelector {}
declare enum GuidoElementType {}

interface TimeSegment {
    start   : string,
    end     : string,
}

interface Rect {
	left    : number;
	top     : number;
	right   : number;
	bottom  : number;
}

interface Time2GraphicElt {
    graph: Rect;
    time: TimeSegment;
}
type Time2GraphicMap = Array<Time2GraphicElt>;

interface MapCollector {
    Graph2TimeMap   (box: Rect, dates: TimeSegment, infos: GuidoElementInfos): void;
}

interface RectInfos {
    ftime   : TimeSegment;	
    fInfos  : GuidoElementInfos;
    time()  : TimeSegment;
	infos() : GuidoElementInfos;	
}

interface TimeMapCollector {
    Time2TimeMap    (from: TimeSegment, to: TimeSegment): void ;
}

interface GuidoElementInfos {
	type        : GuidoElementType;		
	staffNum    : number;	
	voiceNum    : number;	
    midiPitch   : number; 
}

interface GUIDOScoreMap {
    constructor: GUIDOScoreMap;

    getPageMap      (gr: GRHandler, pagenum: number, w: number, h: number)                      : string;
    getStaffMap     (gr: GRHandler, pagenum: number, w: number, h: number, staff: number)       : string;
    getVoiceMap     (gr: GRHandler, pagenum: number, w: number, h: number, voice: number)       : string;
    getSystemMap    (gr: GRHandler, pagenum: number, w: number, h: number)                      : string;
    getTime         (date: GuidoDate, map: string)                                              : string;
    getPoint        (x: number, y: number, map: string)                                         : string;
    getTimeMap      (ar: ARHandler)                                                             : string;
    getPianoRollMap (pr: PianoRoll, width: number, height: number)								: string;
}

interface TimeSegment {
    constructor: TimeSegment;
	empty       () : boolean;
	intersect   (ts: TimeSegment): boolean;
	include     (date: GuidoDate): boolean;
	include     (ts: TimeSegment): boolean;
}

// GUIDOFactory
//--------------------------------------------------------------
interface ARFactoryHandler {} 

interface GUIDOFactoryAdapter {
	factory: ARFactoryHandler;
    constructor: GUIDOFactoryAdapter;
	
	openMusic   () : GuidoErrCode;
	closeMusic  () : ARHandler;

    openVoice   () : GuidoErrCode;
    closeVoice  () : GuidoErrCode;

    openChord   () : GuidoErrCode;
    closeChord  () : GuidoErrCode;

    insertCommata(): GuidoErrCode;
	
    openEvent   (inEventName: string): GuidoErrCode;
    closeEvent  (): GuidoErrCode;

    addSharp    (): GuidoErrCode ;
    addFlat     (): GuidoErrCode;

    setEventDots        (dots: number): GuidoErrCode;
    setEventAccidentals (accident: number): GuidoErrCode;

    setOctave   (octave: number): GuidoErrCode;
    setDuration (numerator: number, denominator: number): GuidoErrCode;

    openTag                 (name: string, tagID: number): GuidoErrCode;
    openRangeTag            (name: string, tagID: number): GuidoErrCode;
    endTag                  (): GuidoErrCode;
    closeTag                (): GuidoErrCode;
    addTagParameterString   (val: string): GuidoErrCode;
    addTagParameterInt      (val: number): GuidoErrCode;
    addTagParameterFloat    (val: number): GuidoErrCode;

    setParameterName(name: string): GuidoErrCode;
    setParameterUnit(unit: string): GuidoErrCode;
}

// GUIDOPianoRoll
//--------------------------------------------------------------
declare enum PianoRollType {}

interface LimitParams {
    startDate   : GuidoDate;
    endDate     : GuidoDate;
    lowPitch    : number;
    highPitch   : number;    
}

interface PianoRoll {}

interface GUIDOPianoRollAdapter {
	constructor: GUIDOPianoRollAdapter;
		
    ar2PianoRoll    (type: PianoRollType, arh: ARHandler): PianoRoll;
    //midi2PianoRoll  (type: PianoRollType, midiFileName: string): PianoRoll;

    destroyPianoRoll(pr: PianoRoll): GuidoErrCode;
    
    setLimits       (pr: PianoRoll, limitParams: LimitParams): GuidoErrCode;
    enableKeyboard  (pr: PianoRoll, enabled: boolean): GuidoErrCode;
    getKeyboardWidth(pr: PianoRoll, height: number): number;

    enableAutoVoicesColoration  (pr: PianoRoll, enabled: boolean): GuidoErrCode;
    setRGBColorToVoice          (pr: PianoRoll, voiceNum: number, r: number, g: number, b: number, a: number): GuidoErrCode;
    setColorToVoice             (pr: PianoRoll, voiceNum: number, color: string): GuidoErrCode;
    removeColorToVoice          (pr: PianoRoll, voiceNum: number): GuidoErrCode;
    enableMeasureBars           (pr: PianoRoll, enabled: boolean): GuidoErrCode;

    setPitchLinesDisplayMode    (pr: PianoRoll, mode: number): GuidoErrCode;
    getMap          (pr: PianoRoll, width: number, height: number): string;

    onDraw          (pr: PianoRoll, width: number, height: number, dev: VGDevice): GuidoErrCode;
	svgExport       (pr: PianoRoll, width: number, height: number): string;
    javascriptExport(pr: PianoRoll, width: number, height: number): GuidoErrCode;
}

interface ReducedProportional {}

interface GUIDOReducedProportionalAdapter {
	constructor: GUIDOReducedProportionalAdapter;
		
    ar2RProportional    (arh: ARHandler): ReducedProportional;
    destroyRProportional(rp: ReducedProportional): GuidoErrCode;
    
    drawDurationLines       (rp: ReducedProportional, status: boolean): GuidoErrCode;
    enableAutoVoicesColoration  (rp: ReducedProportional, enabled: boolean): GuidoErrCode;
    setRGBColorToVoice          (rp: ReducedProportional, voiceNum: number, r: number, g: number, b: number, a: number): GuidoErrCode;
    setHtmlColorToVoice         (rp: ReducedProportional, voiceNum: number, color: string): GuidoErrCode;
    removeColorToVoice          (rp: ReducedProportional, voiceNum: number): GuidoErrCode;
    enableMeasureBars           (rp: ReducedProportional, enabled: boolean): GuidoErrCode;
    setLimits       (rp: ReducedProportional, start: GuidoDate, end: GuidoDate, lowpitch: number, highpitch: number): GuidoErrCode;

    svgExport       (rp: ReducedProportional, width: number, height: number): string;
    javascriptExport(rp: ReducedProportional, width: number, height: number): GuidoErrCode;
}
