///<reference path="libGUIDOEngine.d.ts"/>
//----------------------------------------------------------------------------
// GUIDOEngine interface
//----------------------------------------------------------------------------
class GuidoEngine {
    constructor(module) {
        this.fEngine = new module.GuidoEngineAdapter();
        this.fScoreMap = new module.GUIDOScoreMap();
        this.fPianoRoll = new module.GUIDOPianoRollAdapter();
        this.fFactory = new module.GUIDOFactoryAdapter();
        this.fSPR = new module.GUIDOReducedProportionalAdapter();
        this.fEngine.init();
    }
    //------------------------------------------------------------------------
    // Guido Engine interface
    start() { this.fEngine.init(); }
    shutdown() { this.fEngine.shutdown(); }
    ar2gr(ar) { return this.fEngine.ar2gr(ar); }
    ar2grSettings(ar, settings) { return this.fEngine.ar2grSettings(ar, settings); }
    updateGR(gr) { return this.fEngine.updateGR(gr); }
    updateGRSettings(gr, settings) { return this.fEngine.updateGRSettings(gr, settings); }
    freeAR(ar) { this.fEngine.freeAR(ar); }
    freeGR(gr) { this.fEngine.freeGR(gr); }
    getDefaultLayoutSettings() { return this.fEngine.getDefaultLayoutSettings(); }
    resizePageToMusic(gr) { return this.fEngine.resizePageToMusic(gr); }
    getErrorString(errCode) { return this.fEngine.getErrorString(errCode); }
    showElement(gr, elt, status) { return this.fEngine.showElement(gr, elt, status); }
    countVoices(ar) { return this.fEngine.countVoices(ar); }
    getPageCount(gr) { return this.fEngine.getPageCount(gr); }
    getSystemCount(gr, page) { return this.fEngine.getSystemCount(gr, page); }
    duration(gr) { return this.fEngine.duration(gr); }
    findEventPage(gr, date) { return this.fEngine.findEventPage(gr, date); }
    findPageAt(gr, date) { return this.fEngine.findPageAt(gr, date); }
    getPageDate(gr, pageNum) { return this.fEngine.getPageDate(gr, pageNum); }
    gr2SVG(gr, page, embedFont, mappingMode) { return this.fEngine.gr2SVG(gr, page, embedFont, mappingMode); }
    gr2SVGColored(gr, page, r, g, b, embedFont) { return this.fEngine.gr2SVGColored(gr, page, r, g, b, embedFont); }
    abstractExport(gr, page) { return this.fEngine.abstractExport(gr, page); }
    binaryExport(gr, page) { return this.fEngine.binaryExport(gr, page); }
    jsExport(gr, page) { return this.fEngine.javascriptExport(gr, page); }
    setDefaultPageFormat(format) { this.fEngine.setDefaultPageFormat(format); }
    getDefaultPageFormat() { return this.fEngine.getDefaultPageFormat(); }
    setDrawBoundingBoxes(bmap) { this.fEngine.setDrawBoundingBoxes(bmap); }
    getDrawBoundingBoxes() { return this.fEngine.getDrawBoundingBoxes(); }
    getPageFormat(gr, page) { return this.fEngine.getPageFormat(gr, page); }
    unit2CM(val) { return this.fEngine.unit2CM(val); }
    cm2Unit(val) { return this.fEngine.cm2Unit(val); }
    unit2Inches(val) { return this.fEngine.unit2Inches(val); }
    inches2Unit(val) { return this.fEngine.inches2Unit(val); }
    getLineSpace() { return this.fEngine.getLineSpace(); }
    getVersion() { return this.fEngine.getVersion(); }
    getFloatVersion() { let v = this.fEngine.getVersion(); return parseFloat(v.major + "." + v.minor + v.sub); }
    getVersionStr() { return this.fEngine.getVersionStr(); }
    checkVersionNums(major, minor, sub) { return this.fEngine.checkVersionNums(major, minor, sub); }
    markVoice(ar, voicenum, date, duration, r, g, b) { return this.fEngine.markVoice(ar, voicenum, date, duration, r, g, b); }
    openParser() { return this.fEngine.openParser(); }
    closeParser(p) { return this.fEngine.closeParser(p); }
    file2AR(p, file) { return this.fEngine.file2AR(p, file); }
    string2AR(p, gmn) { return this.fEngine.string2AR(p, gmn); }
    parserGetErrorCode(p) { return this.fEngine.parserGetErrorCode(p); }
    openStream() { return this.fEngine.openStream(); }
    closeStream(s) { return this.fEngine.closeStream(s); }
    getStream(s) { return this.fEngine.getStream(s); }
    stream2AR(p, stream) { return this.fEngine.stream2AR(p, stream); }
    writeStream(s, str) { return this.fEngine.writeStream(s, str); }
    resetStream(s) { return this.fEngine.resetStream(s); }
    getParsingTime(ar) { return this.fEngine.getParsingTime(ar); }
    getAR2GRTime(gr) { return this.fEngine.getAR2GRTime(gr); }
    getOnDrawTime(gr) { return this.fEngine.getOnDrawTime(gr); }
    //------------------------------------------------------------------------
    // Guido mappings interface
    getPageMap(gr, page, w, h) { return this.fScoreMap.getPageMap(gr, page, w, h); }
    getStaffMap(gr, page, w, h, staff) { return this.fScoreMap.getStaffMap(gr, page, w, h, staff); }
    getVoiceMap(gr, page, w, h, voice) { return this.fScoreMap.getVoiceMap(gr, page, w, h, voice); }
    getSystemMap(gr, page, w, h) { return this.fScoreMap.getSystemMap(gr, page, w, h); }
    getTime(date, map) { return this.fScoreMap.getTime(date, map); }
    getPoint(x, y, map) { return this.fScoreMap.getPoint(x, y, map); }
    getTimeMap(ar) { return this.fScoreMap.getTimeMap(ar); }
    getPianoRollMap(pr, width, height) { return this.fScoreMap.getPianoRollMap(pr, width, height); }
    //------------------------------------------------------------------------
    // Guido piano roll interface
    pianoRoll() { return this.fPianoRoll; }
    ar2PianoRoll(type, ar) { return this.fPianoRoll.ar2PianoRoll(type, ar); }
    destroyPianoRoll(pr) { return this.fPianoRoll.destroyPianoRoll(pr); }
    prSetLimits(pr, limits) { return this.fPianoRoll.setLimits(pr, limits); }
    prEnableKeyboard(pr, status) { return this.fPianoRoll.enableKeyboard(pr, status); }
    prGetKeyboardWidth(pr, height) { return this.fPianoRoll.getKeyboardWidth(pr, height); }
    prEnableAutoVoicesColoration(pr, status) { return this.fPianoRoll.enableAutoVoicesColoration(pr, status); }
    prSetVoiceColor(pr, voice, r, g, b, a) { return this.fPianoRoll.setRGBColorToVoice(pr, voice, r, g, b, a); }
    prSetVoiceNamedColor(pr, voice, c) { return this.fPianoRoll.setColorToVoice(pr, voice, c); }
    prRemoveVoiceColor(pr, voice) { return this.fPianoRoll.removeColorToVoice(pr, voice); }
    prEnableMeasureBars(pr, status) { return this.fPianoRoll.enableMeasureBars(pr, status); }
    prSetPitchLinesDisplayMode(pr, mode) { return this.fPianoRoll.setPitchLinesDisplayMode(pr, mode); }
    proll2svg(pr, w, h) { return this.fPianoRoll.svgExport(pr, w, h); }
    prGetMap(pr, width, height) { return this.fPianoRoll.getMap(pr, width, height); }
    prSvgExport(pr, width, height) { return this.fPianoRoll.svgExport(pr, width, height); }
    prJsExport(pr, width, height) { return this.fPianoRoll.javascriptExport(pr, width, height); }
    //------------------------------------------------------------------------
    // Reduced Proportional representation
    // no relay for the interface
    reducedProp() { return this.fSPR; }
    //------------------------------------------------------------------------
    // Guido factory interface
    openMusic() { return this.fFactory.openMusic(); }
    closeMusic() { return this.fFactory.closeMusic(); }
    openVoice() { return this.fFactory.openVoice(); }
    closeVoice() { return this.fFactory.closeVoice(); }
    openChord() { return this.fFactory.openChord(); }
    closeChord() { return this.fFactory.closeChord(); }
    insertCommata() { return this.fFactory.insertCommata(); }
    openEvent(name) { return this.fFactory.openEvent(name); }
    closeEvent() { return this.fFactory.closeEvent(); }
    addSharp() { return this.fFactory.addSharp(); }
    addFlat() { return this.fFactory.addFlat(); }
    setEventDots(dots) { return this.fFactory.setEventDots(dots); }
    setEventAccidentals(acc) { return this.fFactory.setEventAccidentals(acc); }
    setOctave(oct) { return this.fFactory.setOctave(oct); }
    setDuration(numerator, denominator) { return this.fFactory.setDuration(numerator, denominator); }
    openTag(name, tagID) { return this.fFactory.openTag(name, tagID); }
    openRangeTag(name, tagID) { return this.fFactory.openRangeTag(name, tagID); }
    endTag() { return this.fFactory.endTag(); }
    closeTag() { return this.fFactory.closeTag(); }
    addTagParameterString(val) { return this.fFactory.addTagParameterString(val); }
    addTagParameterInt(val) { return this.fFactory.addTagParameterInt(val); }
    addTagParameterFloat(val) { return this.fFactory.addTagParameterFloat(val); }
    setParameterName(name) { return this.fFactory.setParameterName(name); }
    setParameterUnit(unit) { return this.fFactory.setParameterUnit(unit); }
}
var GuidoMapping;
(function (GuidoMapping) {
    GuidoMapping[GuidoMapping["kNoMapping"] = 0] = "kNoMapping";
    GuidoMapping[GuidoMapping["kVoiceMapping"] = 1] = "kVoiceMapping";
    GuidoMapping[GuidoMapping["kStaffMapping"] = 2] = "kStaffMapping";
    GuidoMapping[GuidoMapping["kSystemMapping"] = 4] = "kSystemMapping";
})(GuidoMapping || (GuidoMapping = {}));
var GuidoErrCode;
(function (GuidoErrCode) {
    //! null is used to denote no error
    GuidoErrCode[GuidoErrCode["guidoNoErr"] = 0] = "guidoNoErr";
    //! error while parsing the Guido format
    GuidoErrCode[GuidoErrCode["guidoErrParse"] = -1] = "guidoErrParse";
    //! memory allocation error
    GuidoErrCode[GuidoErrCode["guidoErrMemory"] = -2] = "guidoErrMemory";
    //! error while reading or writing a file
    GuidoErrCode[GuidoErrCode["guidoErrFileAccess"] = -3] = "guidoErrFileAccess";
    //! the user cancelled the action
    GuidoErrCode[GuidoErrCode["guidoErrUserCancel"] = -4] = "guidoErrUserCancel";
    //! the music font is not available
    GuidoErrCode[GuidoErrCode["guidoErrNoMusicFont"] = -5] = "guidoErrNoMusicFont";
    //! the text font is not available
    GuidoErrCode[GuidoErrCode["guidoErrNoTextFont"] = -6] = "guidoErrNoTextFont";
    //! bad parameter used as argument
    GuidoErrCode[GuidoErrCode["guidoErrBadParameter"] = -7] = "guidoErrBadParameter";
    //! invalid handler used
    GuidoErrCode[GuidoErrCode["guidoErrInvalidHandle"] = -8] = "guidoErrInvalidHandle";
    //! required initialisation has not been performed
    GuidoErrCode[GuidoErrCode["guidoErrNotInitialized"] = -9] = "guidoErrNotInitialized";
    //! the action failed
    GuidoErrCode[GuidoErrCode["guidoErrActionFailed"] = -10] = "guidoErrActionFailed";
})(GuidoErrCode || (GuidoErrCode = {}));
var GuidoElementSelector;
(function (GuidoElementSelector) {
    GuidoElementSelector[GuidoElementSelector["kGuidoPage"] = 0] = "kGuidoPage";
    GuidoElementSelector[GuidoElementSelector["kGuidoSystem"] = 1] = "kGuidoSystem";
    GuidoElementSelector[GuidoElementSelector["kGuidoSystemSlice"] = 2] = "kGuidoSystemSlice";
    GuidoElementSelector[GuidoElementSelector["kGuidoStaff"] = 3] = "kGuidoStaff";
    /*kGuidoMeasure,*/
    GuidoElementSelector[GuidoElementSelector["kGuidoBar"] = 4] = "kGuidoBar";
    GuidoElementSelector[GuidoElementSelector["kGuidoEvent"] = 5] = "kGuidoEvent";
    GuidoElementSelector[GuidoElementSelector["kGuidoScoreElementEnd"] = 6] = "kGuidoScoreElementEnd";
})(GuidoElementSelector || (GuidoElementSelector = {}));
var GuidoElementType;
(function (GuidoElementType) {
    GuidoElementType[GuidoElementType["kNote"] = 1] = "kNote";
    GuidoElementType[GuidoElementType["kRest"] = 2] = "kRest";
    GuidoElementType[GuidoElementType["kEmpty"] = 3] = "kEmpty";
    GuidoElementType[GuidoElementType["kBar"] = 4] = "kBar";
    GuidoElementType[GuidoElementType["kRepeatBegin"] = 5] = "kRepeatBegin";
    GuidoElementType[GuidoElementType["kRepeatEnd"] = 6] = "kRepeatEnd";
    GuidoElementType[GuidoElementType["kStaff"] = 7] = "kStaff";
    GuidoElementType[GuidoElementType["kSystemSlice"] = 8] = "kSystemSlice";
    GuidoElementType[GuidoElementType["kSystem"] = 9] = "kSystem";
    GuidoElementType[GuidoElementType["kPage"] = 10] = "kPage";
})(GuidoElementType || (GuidoElementType = {}));
var PianoRollType;
(function (PianoRollType) {
    PianoRollType[PianoRollType["kSimplePianoRoll"] = 0] = "kSimplePianoRoll";
    PianoRollType[PianoRollType["kTrajectoryPianoRoll"] = 1] = "kTrajectoryPianoRoll";
})(PianoRollType || (PianoRollType = {}));
var PianoRollLineMode;
(function (PianoRollLineMode) {
    // pîano roll: pitch line display modes
    PianoRollLineMode[PianoRollLineMode["kPRCLine"] = 1] = "kPRCLine";
    PianoRollLineMode[PianoRollLineMode["kPRCSharpLine"] = 2] = "kPRCSharpLine";
    PianoRollLineMode[PianoRollLineMode["kPRDLine"] = 4] = "kPRDLine";
    PianoRollLineMode[PianoRollLineMode["kPRDSharpLine"] = 8] = "kPRDSharpLine";
    PianoRollLineMode[PianoRollLineMode["kPRELine"] = 16] = "kPRELine";
    PianoRollLineMode[PianoRollLineMode["kPRFLine"] = 32] = "kPRFLine";
    PianoRollLineMode[PianoRollLineMode["kPRFSharpLine"] = 64] = "kPRFSharpLine";
    PianoRollLineMode[PianoRollLineMode["kPRGLine"] = 128] = "kPRGLine";
    PianoRollLineMode[PianoRollLineMode["kPRGSharpLine"] = 256] = "kPRGSharpLine";
    PianoRollLineMode[PianoRollLineMode["kPRALine"] = 512] = "kPRALine";
    PianoRollLineMode[PianoRollLineMode["kPRASharpLine"] = 1024] = "kPRASharpLine";
    PianoRollLineMode[PianoRollLineMode["kPRBLine"] = 2048] = "kPRBLine";
    PianoRollLineMode[PianoRollLineMode["kPRAutoLines"] = 0] = "kPRAutoLines";
    PianoRollLineMode[PianoRollLineMode["kPRNoLine"] = -1] = "kPRNoLine";
})(PianoRollLineMode || (PianoRollLineMode = {}));
///<reference path="../guidoengine.ts"/>
//----------------------------------------------------------------------------
var gmn = '{ \
[ 	\\meter<"2/4"> \\stemsUp \
	\\beam(g2*1/32 e*1/16 c*3/32) c2*1/8 \\beam(a1*1/16 c2 f) \
	\\beam(g/32 d/16 h1*3/32) d2*1/8 \\beam(h1*1/16 d2 g2) \
], \
[	\\staff<1> \\stemsDown g1*1/8 e \\beam(g/16 d f a) a/8 e  \\beam(a/16 e g h) ], \
[	\\staff<2> \\meter<"2/4"> \\stemsUp a0*1/4 f h c1 ], \
[	\\staff<2> \\stemsDown f0*1/4 d g a ] \
}';
var settings = {
    systemsDistance: 75,
    systemsDistribution: 1,
    systemsDistribLimit: 0.25,
    force: 750,
    spring: 1.52,
    neighborhoodSpacing: 1,
    optimalPageFill: 0,
    resizePage2Music: 1,
    proportionalRenderingForceMultiplicator: 0,
    checkLyricsCollisions: 0
};
//----------------------------------------------------------------------------
// Parse to ar and convert to gr + change settings and convert to svg
//----------------------------------------------------------------------------
function ar2gr(engine, log, svg) {
    log("\nParse and AR to GR :");
    var p = engine.openParser();
    var ar = engine.string2AR(p, gmn);
    engine.closeParser(p);
    // Draw in red
    engine.markVoice(ar, 2, { num: 1, denom: 4 }, { num: 3, denom: 4 }, 255, 0, 0);
    var gr = engine.ar2gr(ar);
    log("  ar2gr: " + gr);
    engine.resizePageToMusic(gr);
    var result = engine.gr2SVG(gr, 1, true, 0);
    svg(result, "gr2SVG1");
    // Modify settings and re-draw
    engine.updateGRSettings(gr, settings);
    log("  updateGRSettings");
    result = engine.gr2SVG(gr, 1, true, 0);
    svg(result, "gr2SVG2");
    log("  countVoices:    " + engine.countVoices(ar));
    log("  getPageCount:   " + engine.getPageCount(gr));
    log("  getSystemCount: " + engine.getSystemCount(gr, 1));
    var d = engine.duration(gr);
    log("  duration:       " + d.num + "/" + d.denom);
    engine.freeGR(gr);
    engine.freeAR(ar);
}
//----------------------------------------------------------------------------
// test guido streams
//----------------------------------------------------------------------------
function streams(engine, log, svg) {
    log("\nGuido streams :");
    var p = engine.openParser();
    var stream = engine.openStream();
    var code = '{[f g b/8 d f/4 e';
    engine.writeStream(stream, code);
    var ar = engine.stream2AR(p, stream);
    var gr = engine.ar2gr(ar);
    var result = engine.gr2SVG(gr, 1, true, 0);
    svg(result, "gr2SVG3");
    engine.resetStream(stream);
    engine.getStream(stream);
    engine.closeStream(stream);
    engine.closeParser(p);
    engine.freeAR(ar);
    engine.freeGR(gr);
}
//----------------------------------------------------------------------------
// test guido streams
//----------------------------------------------------------------------------
function scanMap(map) {
    var m = eval(map);
    return m.length;
}
function maps(engine, log) {
    log("\nMap tests :");
    var p = engine.openParser();
    var ar = engine.string2AR(p, gmn);
    var gr = engine.ar2gr(ar);
    engine.closeParser(p);
    let pagemap = engine.getPageMap(gr, 1, 1000.0, 1000.0);
    log("  getPageMap:\t" + scanMap(pagemap) + " elements");
    let staffmap = engine.getStaffMap(gr, 1, 1000.0, 1000.0, 1);
    log("  getStaffMap:\t" + scanMap(staffmap) + " elements");
    let voicemap = engine.getVoiceMap(gr, 1, 1000.0, 1000.0, 1);
    log("  getVoiceMap:\t" + scanMap(voicemap) + " elements");
    let systemmap = engine.getSystemMap(gr, 1, 1000.0, 1000.0);
    log("  getSystemMap:\t" + scanMap(systemmap) + " elements");
    var guidoDate = {
        "num": 2,
        "denom": 4
    };
    let mapTime = engine.getTime(guidoDate, voicemap);
    log("  getTime:\t" + scanMap(mapTime) + " elements");
    let map = engine.getPoint(100, 100, systemmap);
    log("  getPoint:\t" + scanMap(map) + " elements");
    let timemap = engine.getTimeMap(ar);
    log("  getTimeMap:\t" + scanMap(timemap) + " elements");
}
//----------------------------------------------------------------------------
// Reduced Proportional tests
//----------------------------------------------------------------------------
function reducedPropTest(engine, log, svg) {
    log("\Reduced Proportional tests :");
    let p = engine.openParser();
    let ar = engine.string2AR(p, gmn);
    engine.closeParser(p);
    let rpe = engine.reducedProp();
    let rp = rpe.ar2RProportional(ar);
    let result = rpe.svgExport(rp, 512, 256);
    svg(result, "Reduced Proportional SVG1");
    let startDate = { num: 1, denom: 4 };
    let endDate = { num: 3, denom: 4 };
    rpe.setLimits(rp, startDate, endDate, 40, 80);
    rpe.enableAutoVoicesColoration(rp, true);
    result = rpe.svgExport(rp, 512, 256);
    svg(result, "Reduced Proportional SVG2");
    engine.freeAR(ar);
    rpe.destroyRProportional(rp);
}
//----------------------------------------------------------------------------
// Piano roll tests
//----------------------------------------------------------------------------
function pianoRollTest(engine, log, svg) {
    log("\nPiano roll tests :");
    let p = engine.openParser();
    let ar = engine.string2AR(p, gmn);
    engine.closeParser(p);
    let pr = engine.ar2PianoRoll(0, ar);
    let result = engine.proll2svg(pr, 512, 256);
    svg(result, "PianoRoll SVG1");
    let limitParams = {
        startDate: { num: 1, denom: 4 },
        endDate: { num: 3, denom: 4 },
        lowPitch: 40,
        highPitch: 80
    };
    engine.prSetLimits(pr, limitParams);
    engine.prEnableKeyboard(pr, true);
    engine.prEnableAutoVoicesColoration(pr, true);
    engine.prSetPitchLinesDisplayMode(pr, PianoRollLineMode.kPRDSharpLine + PianoRollLineMode.kPRGSharpLine);
    result = engine.proll2svg(pr, 512, 256);
    svg(result, "PianoRoll SVG2");
    let pmap = engine.getPianoRollMap(pr, 512, 256);
    log("  getPianoRollMap:\t" + scanMap(pmap) + " elements");
    engine.freeAR(ar);
    engine.destroyPianoRoll(pr);
}
//----------------------------------------------------------------------------
// Misc. functions
//----------------------------------------------------------------------------
function misc(engine, log) {
    log("\nConversion tools :");
    log("  unit2CM:     " + engine.unit2CM(237.10630798339844));
    log("  cm2Unit:     " + engine.cm2Unit(0.00421751756221056));
    log("  unit2Inches: " + engine.unit2Inches(602.25));
    log("  inches2Unit: " + engine.inches2Unit(0.001660440000705421));
    log("\nMisc :");
    log("  checkVersionNums: " + engine.checkVersionNums(1, 5, 9));
    log("  getLineSpace:     " + engine.getLineSpace());
    log("  getDrawBoundingBoxes: " + engine.getDrawBoundingBoxes());
    engine.setDrawBoundingBoxes(2);
    log("  getDrawBoundingBoxes: " + engine.getDrawBoundingBoxes());
}
function run(engine, log, svg) {
    ar2gr(engine, log, svg);
    streams(engine, log, svg);
    misc(engine, log);
    maps(engine, log);
    pianoRollTest(engine, log, svg);
    reducedPropTest(engine, log, svg);
    log("\nEnd of tests");
}
var process;
var module;
if ((typeof process !== 'undefined') && (process.release.name === 'node')) {
    module.exports = run;
}
