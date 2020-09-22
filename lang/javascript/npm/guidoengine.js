
"use strict";

//----------------------------------------------------------------------------
// the guido engine interface
//----------------------------------------------------------------------------
class GuidoEngine {

    constructor(module) {
        this.fModule   = module;
        this.fEngine   = new module.GuidoEngineAdapter();
        this.fEngine.init();
        this.fPRoll    = new module.GUIDOPianoRollAdapter();
        this.fSPR      = new module.GUIDOReducedProportionalAdapter();
        this.fScoreMap = new module.GUIDOScoreMap();    
        this.fFactory  = new module.GUIDOFactoryAdapter();    

        // pîano roll: pitch line display modes
        this.kPRCLine      =  1;
        this.kPRCSharpLine =  1<<1;
        this.kPRDLine      =  1<<2;
        this.kPRDSharpLine =  1<<3;
        this.kPRELine      =  1<<4;
        this.kPRFLine      =  1<<5;
        this.kPRFSharpLine =  1<<6;
        this.kPRGLine      =  1<<7;
        this.kPRGSharpLine =  1<<8;
        this.kPRALine      =  1<<9;
        this.kPRASharpLine =  1<<10;
        this.kPRBLine      =  1<<11;
        this.kPRAutoLines  =  0;
        this.kPRNoLine     = -1;
    }
            
    //------------------------------------------------------------------------
    // Guido Engine interface
    shutdown ()                     { this.fEngine.shutdown(); }
    
    ar2gr (ar)                      { return this.fEngine.ar2gr (ar); }
    ar2grSettings (ar, settings)    { return this.fEngine.ar2gr (ar, settings); }
    ar2midi (ar, file)              { return this.fModule.ar2MIDIFile (ar); }
    ar2midiSettings (ar, settings)  { return this.fEngine.ar2MIDIFile (ar, settings); }
    updateGR (gr)                   { return this.fEngine.updateGR (gr); }
    updateGRSettings (gr, settings) { return this.fEngine.updateGRSettings (gr, settings); }
    freeAR (ar)                     { this.fEngine.freeAR (ar); }
    freeGR (gr)                     { this.fEngine.freeGR (gr); }
    getDefaultLayoutSettings ()     { return this.fEngine.getDefaultLayoutSettings(); }

    resizePageToMusic ( gr )        { return this.fEngine.resizePageToMusic ( gr ); }
    getErrorString ()               { return this.fEngine.getErrorString(); }
    showElement (gr, elt, status)   { return this.fEngine.showElement( gr, elt, status); }

    countVoices ( ar )              { return this.fEngine.countVoices (ar); }
    getPageCount ( gr )             { return this.fEngine.getPageCount (gr); }
    getSystemCount ( gr, page)      { return this.fEngine.getSystemCount ( gr, page ); }
    duration ( gr )                 { return this.fEngine.duration(gr); }

    findEventPage ( gr, date )      { return this.fEngine.findEventPage ( gr, date ); }
    findPageAt ( gr, date )         { return this.fEngine.findPageAt (gr, date); }
    getPageDate ( gr, pagenum )     { return this.fEngine.getPageDate (gr, pagenum); }

    gr2SVG ( gr, page, embedFont, mappingMode )  { return this.fEngine.gr2SVG ( gr, page, embedFont, mappingMode ); }
    gr2SVGColored (gr, page, r, g, b, embedFont) { return this.fEngine.gr2SVGColored (gr, page, r, g, b, embedFont); }

    abstractExport ( gr, page )     { return this.fEngine.abstractExport ( gr, page ); }
    binaryExport ( gr, page )       { return this.fEngine.binaryExport ( gr, page ); }
    javascriptExport ( gr, page )   { return this.fEngine.javascriptExport ( gr, page ); }

    setDrawBoundingBoxes ( bmap)    { this.fEngine.setDrawBoundingBoxes ( bmap ); }
    getDrawBoundingBoxes ()         { return this.fEngine.getDrawBoundingBoxes(); }
    getPageFormat ( gr, page )      { return this.fEngine.getPageFormat ( gr, page ); }

    setDefaultPageFormat ( format ) { this.fEngine.setDefaultPageFormat ( format ); }
    getDefaultPageFormat()             { return this.fEngine.getDefaultPageFormat(); }

    unit2CM ( val )                 { return this.fEngine.unit2CM ( val ); }
    cm2Unit ( val )                 { return this.fEngine.cm2Unit ( val ); }
    unit2Inches ( val )             { return this.fEngine.unit2Inches ( val ); }
    inches2Unit ( val )             { return this.fEngine.inches2Unit ( val ); }
    getLineSpace()                  { return this.fEngine.getLineSpace(); }

    getVersion ()                   { return this.fEngine.getVersion(); }
    getVersionStr ()                { return this.fEngine.getVersionStr(); }
    checkVersionNums(maj, min, sub) { return this.fEngine.checkVersionNums ( maj, min, sub ); }


    markVoice ( gr, voice, date, duration, r, g, b ) { return this.fEngine.markVoice( gr, voice, date, duration, r, g, b ); }

    openParser()                    { return this.fEngine.openParser(); }
    closeParser(parser)             { return this.fEngine.closeParser ( parser ); }
    file2AR    ( parser, file )     { return this.fEngine.file2AR ( parser, file ); }
    string2AR  ( parser, gmn )      { return this.fEngine.string2AR ( parser, gmn ); }
    parserGetErrorCode ( parser )   { return this.fEngine.parserGetErrorCode ( parser ); }

    openStream  ()                  { return this.fEngine.openStream (); }
    closeStream ( stream )          { return this.fEngine.closeStream ( stream ); }
    getStream   ( stream )          { return this.fEngine.getStream ( stream ); }
    stream2AR   ( parser, stream )  { return this.fEngine.stream2AR ( parser, stream ); }
    writeStream ( stream, gmn )     { return this.fEngine.writeStream ( stream, gmn ); }
    resetStream ( stream )          { return this.fEngine.resetStream ( stream ); }

    getParsingTime ( ar )           { return this.fEngine.getParsingTime ( ar ); }
    getAR2GRTime   ( gr )           { return this.fEngine.getAR2GRTime ( gr ); }
    getOnDrawTime  ( gr )           { return this.fEngine.getOnDrawTime ( gr ); }

    //------------------------------------------------------------------------
    // Guido mappings interface
    getPageMap  ( gr, page, w, h )        { return this.fScoreMap.getPageMap ( gr, page, w, h ); }
    getStaffMap ( gr, page, w, h, staff ) { return this.fScoreMap.getStaffMap ( gr, page, w, h, staff ); }
    getVoiceMap ( gr, page, w, h, voice ) { return this.fScoreMap.getVoiceMap ( gr, page, w, h, voice ); }
    getSystemMap( gr, page, w, h )        { return this.fScoreMap.getSystemMap( gr, page, w, h ); }
    getTime     ( date, map )             { return this.fScoreMap.getTime ( date, map ); }
    getPoint    ( x, y, map )             { return this.fScoreMap.getPoint ( x, y, map ); }
    getTimeMap  ( gr )                    { return this.fScoreMap.getTimeMap ( gr ); }
    getPianoRollMap ( proll, w, h )       { return this.fScoreMap.getPianoRollMap ( proll, w, h ); }

    //------------------------------------------------------------------------
    // Guido piano roll interface
    ar2PianoRoll        ( type, ar )           { return this.fPRoll.ar2PianoRoll ( type, ar ); }
    destroyPianoRoll    ( proll )              { return this.fPRoll.destroyPianoRoll ( proll ); }
    setLimits           ( proll, limits )      { return this.fPRoll.setLimits ( proll, limits ); }
    enableKeyboard      ( proll, status)       { return this.fPRoll.enableKeyboard ( proll, status ); }
    getKeyboardWidth    ( proll, height)       { return this.fPRoll.getKeyboardWidth ( proll, height ); }
    enableAutoVoicesColoration (proll, status) { return this.fPRoll.enableAutoVoicesColoration ( proll, status ); }
    setRGBColorToVoice  ( proll, voice, r, g, b, a) { return this.fPRoll.setRGBColorToVoice ( proll, voice, r, g, b, a ); }
    setHtmlColorToVoice ( proll, voice, c)     { return this.fPRoll.setHtmlColorToVoice ( proll, voice, c ); }
    setColorToVoice 	( proll, voice, c)     { return this.fPRoll.setColorToVoice ( proll, voice, c ); }
    removeColorToVoice  ( proll, voice )       { return this.fPRoll.removeColorToVoice ( proll, voice ); }
    enableMeasureBars   ( proll, status )      { return this.fPRoll.enableMeasureBars ( proll, status ); }
    setPitchLinesDisplayMode (proll, mode)     { return this.fPRoll.setPitchLinesDisplayMode ( proll, mode ); }
    proll2svg           ( proll, w, h )        { return this.fPRoll.svgExport ( proll, w, h  ); }

    //------------------------------------------------------------------------
    // Reduced Proportional representation
    // no relay for the interface
    reducedProp()               { return this.fSPR; }


    //------------------------------------------------------------------------
    // Guido factory interface
    openMusic  ()                { return this.fFactory.openMusic (); }
    closeMusic ()                { return this.fFactory.closeMusic (); }
    openVoice  ()                { return this.fFactory.openVoice (); }
    closeVoice ()                { return this.fFactory.closeVoice (); }
    openChord  ()                { return this.fFactory.openChord (); }
    closeChord ()                { return this.fFactory.closeChord (); }
    insertCommata ()             { return this.fFactory.insertCommata (); }

    openEvent (name)             { return this.fFactory.openEvent ( name ); }
    closeEvent ()                { return this.fFactory.closeEvent (); }
    addSharp ()                  { return this.fFactory.addSharp (); }
    addFlat ()                   { return this.fFactory.addFlat (); }
    setEventDots (ndots)         { return this.fFactory.setEventDots ( ndots ); }
    setEventAccidentals (acc)    { return this.fFactory.setEventAccidentals ( acc ); }
    setOctave (oct)              { return this.fFactory.setOctave ( oct ); }
    setDuration ( n, d )         { return this.fFactory.setDuration ( n, d ); }

    openTag ( name, id )         { return this.fFactory.openTag ( name, id ); }
    openRangeTag ( name, id )    { return this.fFactory.openRangeTag ( name, id ); }
    endTag ()                    { return this.fFactory.endTag ( ); }
    closeTag ()                  { return this.fFactory.closeTag ( ); }
    addTagParameterString (str)  { return this.fFactory.addTagParameterString ( str ); }
    addTagParameterInt ( val )   { return this.fFactory.addTagParameterInt ( val ); }
    addTagParameterFloat ( val)  { return this.fFactory.addTagParameterFloat ( val ); }
    setParameterName ( name )    { return this.fFactory.setParameterName ( name ); }
    setParameterUnit ( unit )    { return this.fFactory.setParameterUnit ( unit ); }

}


if ((typeof process !== 'undefined') && (process.release.name === 'node')) {
    module.exports = GuidoEngine;
}
