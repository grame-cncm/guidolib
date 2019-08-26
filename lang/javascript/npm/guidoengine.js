
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
	// async initialization
	initialize (lib) {
		if ((typeof process !== 'undefined') && (process.release.name === 'node')) {
			var guidomodule = require (lib);
			return new Promise ( (success) => {
					guidomodule().then ( (instance) => {
					this.moduleInit (instance);
					success ( this ); 
				});
			});
		}
		else {
			var module = GuidoModule();
			return new Promise ( (success, failure) => {
				module['onRuntimeInitialized'] = () => {
				this.moduleInit (module);
				success ( this ); 
				}
			});
		}
	}
	
	//------------------------------------------------------------------------
	// async initialization
	moduleInit ( module ) {
		this.fEngine = new module.GuidoEngineAdapter();
		this.fEngine.init();
		this.fPRoll  	= new module.GUIDOPianoRollAdapter();
		this.fSPR  		= new module.GUIDOReducedProportionalAdapter();
		this.fScoreMap 	= new module.GUIDOScoreMap();		
	}
		
	//------------------------------------------------------------------------
	// Guido Engine interface
	shutdown () 					{ this.fEngine.shutdown(); }
	
	ar2gr (ar)						{ return this.fEngine.ar2gr (ar); }
	ar2grSettings (ar, settings) 	{ return this.fEngine.ar2gr (ar, settings); }
	updateGR (gr)					{ return this.fEngine.updateGR (gr); }
	updateGRSettings (ar, settings)	{ return this.fEngine.updateGR (gr, settings); }
	freeAR (ar) 					{ this.fEngine.freeAR (ar); }
	freeGR (gr) 					{ this.fEngine.freeGR (gr); }
	getDefaultLayoutSettings () 	{ return this.fEngine.getDefaultLayoutSettings(); }

	resizePageToMusic ( gr ) 		{ return this.fEngine.resizePageToMusic ( gr ); }
	getErrorString () 				{ return this.fEngine.getErrorString(); }
	showElement (gr, elt, status) 	{ return this.fEngine.showElement( gr, elt, status); }

	countVoices ( ar ) 				{ return this.fEngine.countVoices (ar); }
	getPageCount ( gr ) 			{ return this.fEngine.getPageCount (gr); }
	getSystemCount ( gr, page) 		{ return this.fEngine.getSystemCount ( gr, page ); }
	duration ( gr ) 				{ return this.fEngine.duration(); }

	findEventPage ( gr, date ) 		{ return this.fEngine.findEventPage ( gr, date ); }
	findPageAt ( gr, date ) 		{ return this.fEngine.findPageAt (gr, date); }
	getPageDate ( gr, pagenum ) 	{ return this.fEngine.getPageDate (gr, pagenum); }

	gr2SVG ( gr, page, embedFont, mappingMode )  { return this.fEngine.gr2SVG ( gr, page, embedFont, mappingMode ); }
	gr2SVGColored (gr, page, r, g, b, embedFont) { return this.fEngine.gr2SVGColored (gr, page, r, g, b, embedFont); }

	abstractExport ( gr, page ) 	{ return this.fEngine.abstractExport ( gr, page ); }
	binaryExport ( gr, page ) 		{ return this.fEngine.binaryExport ( gr, page ); }
	javascriptExport ( gr, page ) 	{ return this.fEngine.javascriptExport ( gr, page ); }

	setDrawBoundingBoxes ( bmap) 	{ this.fEngine.setDrawBoundingBoxes ( bmap ); }
	getDrawBoundingBoxes () 		{ return this.fEngine.getDrawBoundingBoxes(); }
	getPageFormat ( gr, page ) 		{ return this.fEngine.getPageFormat ( gr, page ); }

	setDefaultPageFormat ( format ) { this.fEngine.setDefaultPageFormat ( format ); }
	getDefaultPageFormat() 			{ return this.fEngine.getDefaultPageFormat(); }

	unit2CM ( val ) 				{ return this.fEngine.unit2CM ( val ); }
	cm2Unit ( val ) 				{ return this.fEngine.cm2Unit ( val ); }
	unit2Inches ( val ) 			{ return this.fEngine.unit2Inches ( val ); }
	inches2Unit ( val ) 			{ return this.fEngine.inches2Unit ( val ); }
	getLineSpace() 					{ return this.fEngine.getLineSpace(); }

	getVersion () 					{ return this.fEngine.getVersion(); }
	getVersionStr () 				{ return this.fEngine.getVersionStr(); }
	checkVersionNums(maj, min, sub) { return this.fEngine.checkVersionNums ( maj, min, sub ); }


	markVoice ( gr, date, duration, r, g, b ) { return this.fEngine.markVoice( gr, date, duration, r, g, b ); }

	openParser() 					{ return this.fEngine.openParser(); }
	closeParser(parser) 			{ return this.fEngine.closeParser ( parser ); }
	file2AR		( parser, file ) 	{ return this.fEngine.file2AR ( parser, file ); }
	string2AR	( parser, gmn ) 	{ return this.fEngine.string2AR ( parser, gmn ); }
	parserGetErrorCode ( parser )	{ return this.fEngine.parserGetErrorCode ( parser ); }

	openStream	() 					{ return this.fEngine.openStream (); }
	closeStream	( stream ) 			{ return this.fEngine.closeStream ( stream ); }
	getStream	( stream ) 			{ return this.fEngine.getStream ( stream ); }
	stream2AR	( parser, stream ) 	{ return this.fEngine.stream2AR ( parser, stream ); }
	writeStream	( stream, gmn ) 	{ return fthis.fEngine.writeStream ( stream, gmn ); }
	resetStream	( stream ) 			{ return fthis.fEngine.resetStream ( stream ); }

	getParsingTime 	( ar ) 			{ return this.fEngine.getParsingTime ( ar ); }
	getAR2GRTime	( gr ) 			{ return this.fEngine.getAR2GRTime ( gr ); }
	getOnDrawTime	( gr ) 			{ return this.fEngine.getOnDrawTime ( gr ); }

	//------------------------------------------------------------------------
	// Guido mappings interface
	getPageMap 	( gr, page, w, h ) 			{ return this.fScoreMap.getPageMap ( gr, page, w, h ); }
	getStaffMap ( gr, page, w, h, staff ) 	{ return this.fScoreMap.getStaffMap ( gr, page, w, h, staff ); }
	getVoiceMap	( gr, page, w, h, voice ) 	{ return this.fScoreMap.getVoiceMap ( gr, page, w, h, voice ); }
	getSystemMap( gr, page, w, h ) 			{ return this.fScoreMap.getSystemMap( gr, page, w, h ); }
	getTime		( date, map ) 				{ return this.fScoreMap.getTime ( date, map ); }
	getPoint	( x, y, map ) 				{ return this.fScoreMap.getPoint ( x, y, map ); }
	getTimeMap	( gr ) 						{ return this.fScoreMap.getTimeMap ( gr ); }
	getPianoRollMap ( proll, w, h ) 		{ return this.fScoreMap.getPianoRollMap ( proll, w, h ); }

}

if ((typeof process !== 'undefined') && (process.release.name === 'node')) {
	module.exports = GuidoEngine;
}
