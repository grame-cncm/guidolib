///<reference path="libGUIDOEngine.d.ts"/>


//----------------------------------------------------------------------------
// GUIDOEngine interface
//----------------------------------------------------------------------------
class GuidoEngine {

    private	fEngine    :   GuidoEngineAdapter;
    private	fPianoRoll :   GUIDOPianoRollAdapter;
    private	fScoreMap  :   GUIDOScoreMap;
    private	fFactory   :   GUIDOFactoryAdapter;
    private	fSPR       :   GUIDOReducedProportionalAdapter;

    constructor (module: any) {
        this.fEngine 		= new module.GuidoEngineAdapter();
        this.fScoreMap      = new module.GUIDOScoreMap();
        this.fPianoRoll		= new module.GUIDOPianoRollAdapter();
        this.fFactory		= new module.GUIDOFactoryAdapter();
        this.fSPR           = new module.GUIDOReducedProportionalAdapter();
        this.fEngine.init();   
    }
        
    //------------------------------------------------------------------------
    // Guido Engine interface
    start () : void                 { this.fEngine.init(); }
    shutdown () : void              { this.fEngine.shutdown(); }
    
    ar2gr (ar: ARHandler) : GRHandler                                                  { return this.fEngine.ar2gr (ar); }
    ar2grSettings       (ar: ARHandler, settings: GuidoLayoutSettings): GRHandler      { return this.fEngine.ar2grSettings(ar, settings); }    	
    updateGR            (gr: GRHandler): GuidoErrCode                                  { return this.fEngine.updateGR(gr); }
    updateGRSettings    (gr: GRHandler, settings: GuidoLayoutSettings): GuidoErrCode   { return this.fEngine.updateGRSettings (gr, settings); }
	freeAR              (ar: ARHandler): void           { this.fEngine.freeAR(ar); }
	freeGR              (gr: GRHandler): void           { this.fEngine.freeGR(gr); }
	getDefaultLayoutSettings (): GuidoLayoutSettings    { return this.fEngine.getDefaultLayoutSettings(); }

    resizePageToMusic ( gr: GRHandler ): GuidoErrCode   { return this.fEngine.resizePageToMusic ( gr ); }
    getErrorString    (errCode: GuidoErrCode): string   { return this.fEngine.getErrorString(errCode); }
    showElement    	  (gr: GRHandler, elt: number, status: boolean): GuidoErrCode    { return this.fEngine.showElement (gr, elt, status); }
    	
    countVoices     (ar: ARHandler): number                 { return this.fEngine.countVoices (ar); }
    getPageCount    (gr: GRHandler): number                 { return this.fEngine.getPageCount (gr); }
    getSystemCount  (gr: GRHandler, page: number): number   { return this.fEngine.getSystemCount(gr, page); }
    duration        (gr: GRHandler): GuidoDate              { return this.fEngine.duration(gr); }
 
	findEventPage   (gr: GRHandler , date: GuidoDate): number      { return this.fEngine.findEventPage (gr, date); }
	findPageAt      (gr: GRHandler, date: GuidoDate): number       { return this.fEngine.findPageAt (gr, date); }
    getPageDate     (gr: GRHandler, pageNum: number): GuidoDate    { return this.fEngine.getPageDate (gr, pageNum); }

    gr2SVG ( gr: GRHandler, page: number, embedFont: boolean, mappingMode: number )  : string
                            { return this.fEngine.gr2SVG ( gr, page, embedFont, mappingMode ); }
    gr2SVGColored (gr: GRHandler, page: number, r: number, g: number, b: number, embedFont: boolean) : string
                            { return this.fEngine.gr2SVGColored (gr, page, r, g, b, embedFont); }

    abstractExport ( gr: GRHandler, page: number )  : string  { return this.fEngine.abstractExport ( gr, page ); }
    binaryExport ( gr: GRHandler, page: number )    : string  { return this.fEngine.binaryExport ( gr, page ); }
    jsExport ( gr: GRHandler, page: number ): GuidoErrCode    { return this.fEngine.javascriptExport ( gr, page ); }

    setDefaultPageFormat ( format: GuidoPageFormat ): void  { this.fEngine.setDefaultPageFormat ( format ); }
    getDefaultPageFormat () :GuidoPageFormat                { return this.fEngine.getDefaultPageFormat(); }

    setDrawBoundingBoxes (bmap: number): void           { this.fEngine.setDrawBoundingBoxes ( bmap ); }
    getDrawBoundingBoxes () : number                    { return this.fEngine.getDrawBoundingBoxes(); }
    getPageFormat ( gr: GRHandler, page: number ) :GuidoPageFormat     { return this.fEngine.getPageFormat ( gr, page ); }

    unit2CM     (val: number): number   { return this.fEngine.unit2CM ( val ); }
	cm2Unit     (val: number): number   { return this.fEngine.cm2Unit ( val ); }
	unit2Inches (val: number): number   { return this.fEngine.unit2Inches ( val ); }
    inches2Unit (val: number): number   { return this.fEngine.inches2Unit ( val ); }
    getLineSpace(): number              { return this.fEngine.getLineSpace(); }

    getVersion () : GuidoVersion        { return this.fEngine.getVersion(); }
    getFloatVersion () : number         { let v = this.fEngine.getVersion(); return parseFloat(v.major + "." + v.minor + v.sub); }
    getVersionStr () : string           { return this.fEngine.getVersionStr(); }
    checkVersionNums(major: number, minor: number, sub: number): GuidoErrCode  { return this.fEngine.checkVersionNums ( major, minor, sub ); }

    markVoice   (ar: ARHandler, voicenum: number,
        date: GuidoDate, duration: GuidoDate,
        r: number, g: number, b: number ): GuidoErrCode  { return this.fEngine.markVoice( ar, voicenum, date, duration, r, g, b ); }

    openParser      (): GuidoParser                         { return this.fEngine.openParser(); }
    closeParser     (p: GuidoParser): GuidoErrCode          { return this.fEngine.closeParser ( p ); }
    file2AR     (p: GuidoParser, file: string): ARHandler   { return this.fEngine.file2AR ( p, file ); }
    string2AR   (p: GuidoParser, gmn: string): ARHandler    { return this.fEngine.string2AR ( p, gmn ); }
    parserGetErrorCode ( p: GuidoParser ): ParserError      { return this.fEngine.parserGetErrorCode ( p ); }

    openStream   (): GuidoStream                     { return this.fEngine.openStream (); }
    closeStream  (s: GuidoStream): GuidoErrCode      { return this.fEngine.closeStream ( s ); }
    getStream    (s: GuidoStream): string            { return this.fEngine.getStream ( s ); }
    stream2AR    (p: GuidoParser,stream: GuidoStream): ARHandler    { return this.fEngine.stream2AR ( p, stream ); }
    writeStream  (s: GuidoStream, str: string): GuidoErrCode        { return this.fEngine.writeStream ( s, str ); }
	resetStream  (s: GuidoStream): GuidoErrCode      { return this.fEngine.resetStream ( s ); }
                       
    getParsingTime  (ar: ARHandler): number              { return this.fEngine.getParsingTime(ar); }
    getAR2GRTime    (gr: GRHandler): number              { return this.fEngine.getAR2GRTime(gr); }
    getOnDrawTime   (gr: GRHandler): number              { return this.fEngine.getOnDrawTime(gr); }

    //------------------------------------------------------------------------
    // Guido mappings interface
    getPageMap      (gr: GRHandler, page: number, w: number, h: number)                : string { return this.fScoreMap.getPageMap ( gr, page, w, h ); }
    getStaffMap     (gr: GRHandler, page: number, w: number, h: number, staff: number) : string { return this.fScoreMap.getStaffMap ( gr, page, w, h, staff ); }
    getVoiceMap     (gr: GRHandler, page: number, w: number, h: number, voice: number) : string { return this.fScoreMap.getVoiceMap ( gr, page, w, h, voice ); }
    getSystemMap    (gr: GRHandler, page: number, w: number, h: number)                : string { return this.fScoreMap.getSystemMap( gr, page, w, h ); }
    getTime         (date: GuidoDate, map: string)                                     : string { return this.fScoreMap.getTime ( date, map ); }
    getPoint        (x: number, y: number, map: string)                                : string { return this.fScoreMap.getPoint ( x, y, map ); }
    getTimeMap      (ar: ARHandler)                                                    : string { return this.fScoreMap.getTimeMap ( ar ); }
    getPianoRollMap (pr: PianoRoll, width: number, height: number)					   : string { return this.fScoreMap.getPianoRollMap ( pr, width, height ); }

    //------------------------------------------------------------------------
    // Guido piano roll interface
    pianoRoll() : GUIDOPianoRollAdapter { return this.fPianoRoll; }

    ar2PianoRoll        ( type: PianoRollType, ar: ARHandler): PianoRoll        { return this.fPianoRoll.ar2PianoRoll ( type, ar ); }
    destroyPianoRoll    ( pr: PianoRoll ): GuidoErrCode                         { return this.fPianoRoll.destroyPianoRoll ( pr ); }
    prSetLimits           ( pr: PianoRoll, limits: LimitParams ): GuidoErrCode    { return this.fPianoRoll.setLimits ( pr, limits ); }
    prEnableKeyboard      ( pr: PianoRoll, status: boolean) : GuidoErrCode        { return this.fPianoRoll.enableKeyboard ( pr, status ); }
    prGetKeyboardWidth    ( pr: PianoRoll, height: number)  : number              { return this.fPianoRoll.getKeyboardWidth ( pr, height ); }
    prEnableAutoVoicesColoration (pr: PianoRoll, status: boolean): GuidoErrCode   { return this.fPianoRoll.enableAutoVoicesColoration ( pr, status ); }
    
    prSetVoiceColor  ( pr: PianoRoll, voice: number, r: number, g: number, b: number, a: number): GuidoErrCode { return this.fPianoRoll.setRGBColorToVoice ( pr, voice, r, g, b, a ); }
    prSetVoiceNamedColor ( pr: PianoRoll, voice: number, c: string): GuidoErrCode { return this.fPianoRoll.setColorToVoice ( pr, voice, c ); }
    prRemoveVoiceColor  ( pr: PianoRoll, voice: number )   : GuidoErrCode         { return this.fPianoRoll.removeColorToVoice ( pr, voice ); }
    prEnableMeasureBars   ( pr: PianoRoll, status: boolean ) : GuidoErrCode       { return this.fPianoRoll.enableMeasureBars ( pr, status ); }
    prSetPitchLinesDisplayMode (pr: PianoRoll, mode: number): GuidoErrCode        { return this.fPianoRoll.setPitchLinesDisplayMode ( pr, mode ); }
    
    proll2svg ( pr: PianoRoll, w: number, h: number )                           { return this.fPianoRoll.svgExport ( pr, w, h  ); }
    prGetMap  (pr: PianoRoll, width: number, height: number): string            { return this.fPianoRoll.getMap ( pr, width, height  ); }

	prSvgExport(pr: PianoRoll, width: number, height: number): string           { return this.fPianoRoll.svgExport ( pr, width, height  ); }
    prJsExport (pr: PianoRoll, width: number, height: number): GuidoErrCode     { return this.fPianoRoll.javascriptExport ( pr, width, height  ); }

    //------------------------------------------------------------------------
    // Reduced Proportional representation
    // no relay for the interface
    reducedProp() : GUIDOReducedProportionalAdapter { return this.fSPR; }


    //------------------------------------------------------------------------
    // Guido factory interface
	openMusic   () : GuidoErrCode       { return this.fFactory.openMusic (); }
	closeMusic  () : ARHandler          { return this.fFactory.closeMusic (); }
    openVoice   () : GuidoErrCode       { return this.fFactory.openVoice (); }
    closeVoice  () : GuidoErrCode       { return this.fFactory.closeVoice (); }
    openChord   () : GuidoErrCode       { return this.fFactory.openChord (); }
    closeChord  () : GuidoErrCode       { return this.fFactory.closeChord (); }
    insertCommata(): GuidoErrCode       { return this.fFactory.insertCommata (); }
	
    openEvent   (name: string): GuidoErrCode { return this.fFactory.openEvent ( name ); }
    closeEvent  (): GuidoErrCode        { return this.fFactory.closeEvent (); }
    addSharp    (): GuidoErrCode        { return this.fFactory.addSharp (); }
    addFlat     (): GuidoErrCode        { return this.fFactory.addFlat (); }
    setEventDots        (dots: number): GuidoErrCode { return this.fFactory.setEventDots ( dots ); }
    setEventAccidentals (acc: number): GuidoErrCode  { return this.fFactory.setEventAccidentals ( acc ); }  
    setOctave   (oct: number): GuidoErrCode          { return this.fFactory.setOctave ( oct ); }
    setDuration (numerator: number, denominator: number): GuidoErrCode  { return this.fFactory.setDuration ( numerator, denominator ); }

    openTag                 (name: string, tagID: number): GuidoErrCode { return this.fFactory.openTag ( name, tagID ); }
    openRangeTag            (name: string, tagID: number): GuidoErrCode { return this.fFactory.openRangeTag ( name, tagID ); }
    endTag                  (): GuidoErrCode                { return this.fFactory.endTag(); }
    closeTag                (): GuidoErrCode                { return this.fFactory.closeTag ( ); }
    addTagParameterString   (val: string): GuidoErrCode     { return this.fFactory.addTagParameterString ( val ); }
    addTagParameterInt      (val: number): GuidoErrCode     { return this.fFactory.addTagParameterInt ( val ); }
    addTagParameterFloat    (val: number): GuidoErrCode     { return this.fFactory.addTagParameterFloat ( val ); }

    setParameterName(name: string): GuidoErrCode        { return this.fFactory.setParameterName ( name ); }
    setParameterUnit(unit: string): GuidoErrCode        { return this.fFactory.setParameterUnit ( unit ); }
}

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
enum PianoRollLineMode {
    // pîano roll: pitch line display modes
    kPRCLine      =  1,
    kPRCSharpLine =  1<<1,
    kPRDLine      =  1<<2,
    kPRDSharpLine =  1<<3,
    kPRELine      =  1<<4,
    kPRFLine      =  1<<5,
    kPRFSharpLine =  1<<6,
    kPRGLine      =  1<<7,
    kPRGSharpLine =  1<<8,
    kPRALine      =  1<<9,
    kPRASharpLine =  1<<10,
    kPRBLine      =  1<<11,
    kPRAutoLines  =  0,
    kPRNoLine     = -1
}
