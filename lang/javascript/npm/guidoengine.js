
"use strict";

//----------------------------------------------------------------------------
// the guido engine interface
//----------------------------------------------------------------------------
class GuidoEngine {
	constructor() {
		this.fEngine = 0;
		this.fScoreMap = 0;
		this.fPRoll = 0;
		this.fSPR = 0;
	}

	//------------------------------------------------------------------------
	// initialization
	initialize() {
		var module = GuidoModule();
		module['onRuntimeInitialized'] = () => {
			this.fEngine = new module.GuidoEngineAdapter();
			var version = this.fEngine.getVersion();
			console.log( "Guido Engine version " + version.str);
			this.fEngine.init();
			this.fPRoll  	= new GuidoPRoll(new module.GUIDOPianoRollAdapter());
			this.fSPR  		= new GuidoSPR(new module.GUIDOReducedProportionalAdapter());
			this.fScoreMap 	= new GUIDOScoreMap( new module.GuidoScoreMap());
		}
	}
	

	//------------------------------------------------------------------------
	// Guido Engine interface
	shutdown () 					{ fEngine.shutdown(); }
	
	ar2gr (ar)						{ return fEngine.ar2gr (ar); }
	ar2grSettings (ar, settings) 	{ return fEngine.ar2gr (ar, settings); }
	updateGR (gr)					{ return fEngine.updateGR (gr); }
	updateGRSettings (ar, settings)	{ return fEngine.updateGR (gr, settings); }
	freeAR (ar) 					{ fEngine.freeAR (ar); }
	freeGR (gr) 					{ fEngine.freeGR (gr); }
	getDefaultLayoutSettings () 	{ return fEngine.getDefaultLayoutSettings(); }

	resizePageToMusic ( gr ) 		{ return fEngine.resizePageToMusic ( gr ); }
	getErrorString () 				{ return fEngine.getErrorString(); }
	showElement (gr, elt, status) 	{ return fEngine.showElement( gr, elt, status); }

	countVoices ( ar ) 				{ return fEngine.countVoices (ar); }
	getPageCount ( gr ) 			{ return fEngine.getPageCount (gr); }
	getSystemCount ( gr, page) 		{ return fEngine.getSystemCount ( gr, page ); }
	duration ( gr ) 				{ return fEngine.duration(); }

	findEventPage ( gr, date ) 		{ return fEngine.findEventPage ( gr, date ); }
	findPageAt ( gr, date ) 		{ return fEngine.findPageAt (gr, date); }
	getPageDate ( gr, pagenum ) 	{ return fEngine.getPageDate (gr, pagenum); }

	gr2SVG ( gr, page, embedFont, mappingMode )  { return fEngine.gr2SVG ( gr, page, embedFont, mappingMode ); }
	gr2SVGColored (gr, page, r, g, b, embedFont) { return fEngine.gr2SVGColored (gr, page, r, g, b, embedFont); }

	abstractExport ( gr, page ) 	{ return fEngine.abstractExport ( gr, page ); }
	binaryExport ( gr, page ) 		{ return fEngine.binaryExport ( gr, page ); }
	javascriptExport ( gr, page ) 	{ return fEngine.javascriptExport ( gr, page ); }

	setDrawBoundingBoxes(int bmap) 	{ fEngine.setDrawBoundingBoxes ( bmap ); }
	getDrawBoundingBoxes() 			{ return fEngine.getDrawBoundingBoxes(); }
	getPageFormat ( gr, page ) 		{ return fEngine.getPageFormat ( gr, page ); }

	setDefaultPageFormat ( format ) { fEngine.setDefaultPageFormat ( format ); }
	getDefaultPageFormat() 			{ return fEngine.getDefaultPageFormat(); }

	unit2CM ( val ) 				{ return fEngine.unit2CM ( val ); }
	cm2Unit ( val ) 				{ return fEngine.cm2Unit ( val ); }
	unit2Inches ( val ) 			{ return fEngine.unit2Inches ( val ); }
	inches2Unit ( val ) 			{ return fEngine.inches2Unit ( val ); }
	getLineSpace() 					{ return fEngine.getLineSpace(); }

	getVersion () 					{ return fEngine.getVersion(); }
	getVersionStr () 				{ return fEngine.getVersionStr(); }
	checkVersionNums(maj, min, sub) { return fEngine.checkVersionNums ( maj, min, sub ); }


	markVoice ( gr, date, duration, r, g, b ) { return fEngine.markVoice( gr, date, duration, r, g, b ); }

	openParser() 					{ return fEngine.openParser(); }
	closeParser(parser) 			{ return fEngine.closeParser ( parser ); }
	file2AR		( parser, file ) 	{ return fEngine.file2AR ( parser, file ); }
	string2AR	( parser, gmn ) 	{ return fEngine.string2AR ( parser, gmn ); }
	parserGetErrorCode ( parser )	{ return fEngine.parserGetErrorCode ( parser ); }

	openStream	() 					{ return fEngine.openStream (); }
	closeStream	( stream ) 			{ return fEngine.closeStream ( stream ); }
	getStream	( stream ) 			{ return fEngine.getStream ( stream ); }
	stream2AR	( parser, stream ) 	{ return fEngine.stream2AR ( parser, stream ); }
	writeStream	( stream, gmn ) 	{ return ffEngine.writeStream ( stream, gmn ); }
	resetStream	( stream ) 			{ return ffEngine.resetStream ( stream ); }

	getParsingTime 	( ar ) 			{ return fEngine.getParsingTime ( ar ); }
	getAR2GRTime	( gr ) 			{ return fEngine.getAR2GRTime ( gr ); }
	getOnDrawTime	( gr ) 			{ return fEngine.getOnDrawTime ( gr ); }

	//------------------------------------------------------------------------
	// Guido mappings interface
	getPageMap 	( gr, page, w, h ) 			{ return fEngine.getPageMap ( gr, page, w, h ); }
	getStaffMap ( gr, page, w, h, staff ) 	{ return fEngine.getStaffMap ( gr, page, w, h, staff ); }
	getVoiceMap	( gr, page, w, h, voice ) 	{ return fEngine.getVoiceMap ( gr, page, w, h, voice ); }
	getSystemMap( gr, page, w, h ) 			{ return fEngine.getSystemMap( gr, page, w, h ); }
	getTime		( date, map ) 				{ return fEngine.getTime ( date, map ); }
	getPoint	( x, y, map ) 				{ return fEngine.getPoint ( x, y, map ); }
	getTimeMap	( gr ) 						{ return fEngine.getTimeMap ( gr ); }
	getPianoRollMap ( proll, w, h ) 		{ return fEngine.getPianoRollMap ( proll, w, h ); }

}

