
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

var settings= {
	systemsDistance : 75,
	systemsDistribution : 1,
	systemsDistribLimit : 0.25,
	force : 750,
	spring : 1.52,
	neighborhoodSpacing : 1,
	optimalPageFill : 0,
	resizePage2Music : 1,
	proportionalRenderingForceMultiplicator : 0,
	checkLyricsCollisions : 0
};

//----------------------------------------------------------------------------
interface TLog { (text: string): void; }
interface TSvg { (text: string, name: string): void; }

//----------------------------------------------------------------------------
// Parse to ar and convert to gr + change settings and convert to svg
//----------------------------------------------------------------------------
function ar2gr (engine: GuidoEngine, log: TLog, svg: TSvg )
{
	log( "\nParse and AR to GR :");
	var p = engine.openParser();
	var ar = engine.string2AR (p, gmn);
	engine.closeParser(p);
	
	// Draw in red
	engine.markVoice(ar, 2, {num : 1, denom : 4}, {num : 3, denom : 4}, 255, 0, 0);
	var gr = engine.ar2gr(ar);
	log ("  ar2gr: " + gr);
	engine.resizePageToMusic (gr);	

	var result = engine.gr2SVG(gr, 1, true, 0);
	svg (result, "gr2SVG1");
	
	// Modify settings and re-draw
	engine.updateGRSettings (gr, settings);
	log ("  updateGRSettings");
	result = engine.gr2SVG(gr, 1, true, 0);
	svg (result, "gr2SVG2");

	log ("  countVoices:    " + engine.countVoices(ar));
	log ("  getPageCount:   " + engine.getPageCount(gr));
	log ("  getSystemCount: " + engine.getSystemCount(gr, 1));
	var d = engine.duration(gr);
	log ("  duration:       " + d.num  + "/" + d.denom);

	engine.freeGR (gr);
	engine.freeAR (ar);
}

//----------------------------------------------------------------------------
// test guido streams
//----------------------------------------------------------------------------
function streams ( engine: GuidoEngine, log: TLog, svg: TSvg ) {
	log( "\nGuido streams :");
	
	var p = engine.openParser();
	var stream = engine.openStream();
	var code = '{[f g b/8 d f/4 e';
	engine.writeStream(stream, code);
	
	var ar = engine.stream2AR(p, stream);
	var gr = engine.ar2gr(ar);
	var result = engine.gr2SVG(gr, 1, true, 0);
	svg (result, "gr2SVG3");
	
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
function scanMap( map: string ) 
{
	var m = eval(map);
	return m.length;
}

function maps( engine: GuidoEngine, log: TLog ) 
{
	log( "\nMap tests :");
	var p = engine.openParser();
	var ar = engine.string2AR (p, gmn);
	var gr = engine.ar2gr(ar);
	engine.closeParser(p);


	let pagemap 	= engine.getPageMap( gr, 1, 1000.0, 1000.0 );
	log ("  getPageMap:\t" + scanMap(pagemap) + " elements");
	let staffmap 	= engine.getStaffMap( gr, 1, 1000.0, 1000.0, 1 );
	log ("  getStaffMap:\t" + scanMap(staffmap) + " elements");
	let voicemap 	= engine.getVoiceMap( gr, 1, 1000.0, 1000.0, 1 );
	log ("  getVoiceMap:\t" + scanMap(voicemap) + " elements");
	let systemmap 	= engine.getSystemMap( gr, 1, 1000.0, 1000.0);
	log ("  getSystemMap:\t" + scanMap(systemmap) + " elements");

	var guidoDate = {
		"num" : 2,
		"denom" : 4
	};
	let mapTime 	= engine.getTime(guidoDate, voicemap);
	log ("  getTime:\t" + scanMap(mapTime) + " elements");
	let map 		= engine.getPoint(100, 100, systemmap);
	log ("  getPoint:\t" + scanMap(map) + " elements");
	let timemap 	= engine.getTimeMap(ar);	
	log ("  getTimeMap:\t" + scanMap(timemap) + " elements");
}

//----------------------------------------------------------------------------
// Reduced Proportional tests
//----------------------------------------------------------------------------
function reducedPropTest (engine: GuidoEngine, log: TLog, svg: TSvg)
{
	log( "\Reduced Proportional tests :");
	let p = engine.openParser();
	let ar = engine.string2AR (p, gmn);
	engine.closeParser(p);

	let rpe = engine.reducedProp();
	let rp = rpe.ar2RProportional (ar);	
	let result = rpe.svgExport(rp, 512, 256);
	svg (result, "Reduced Proportional SVG1");
	
	let startDate = {num : 1, denom : 4};
	let endDate = {num : 3, denom : 4};
	rpe.setLimits(rp, startDate, endDate, 40, 80);
	rpe.enableAutoVoicesColoration(rp, true);
	result = rpe.svgExport(rp, 512, 256);
	svg (result, "Reduced Proportional SVG2");
	engine.freeAR(ar);
	rpe.destroyRProportional(rp);
}

//----------------------------------------------------------------------------
// Piano roll tests
//----------------------------------------------------------------------------
function pianoRollTest (engine: GuidoEngine, log: TLog, svg: TSvg)
{
	log( "\nPiano roll tests :");
	let p = engine.openParser();
	let ar = engine.string2AR (p, gmn);
	engine.closeParser(p);

	let pr = engine.ar2PianoRoll (0, ar);	
	let result = engine.proll2svg(pr, 512, 256);
	svg (result, "PianoRoll SVG1");
	
	let limitParams = {
		startDate : {num : 1, denom : 4},
		endDate : {num : 3, denom : 4},
		lowPitch : 40,
		highPitch : 80
	};
	engine.prSetLimits(pr, limitParams);
	engine.prEnableKeyboard(pr, true);
	engine.prEnableAutoVoicesColoration(pr, true);
	engine.prSetPitchLinesDisplayMode(pr, PianoRollLineMode.kPRDSharpLine + PianoRollLineMode.kPRGSharpLine);
	result = engine.proll2svg(pr, 512, 256);
	svg (result, "PianoRoll SVG2");
	let pmap = engine.getPianoRollMap(pr, 512, 256);
	log ("  getPianoRollMap:\t" + scanMap(pmap) + " elements");

	engine.freeAR(ar);
	engine.destroyPianoRoll(pr);
}

//----------------------------------------------------------------------------
// Misc. functions
//----------------------------------------------------------------------------
function misc (engine: GuidoEngine, log: TLog )
{
	log( "\nConversion tools :");
	log( "  unit2CM:     " + engine.unit2CM(237.10630798339844));
	log( "  cm2Unit:     " + engine.cm2Unit(0.00421751756221056));
	log( "  unit2Inches: " + engine.unit2Inches(602.25));
	log( "  inches2Unit: " + engine.inches2Unit(0.001660440000705421));

	log( "\nMisc :");
	log( "  checkVersionNums: " + engine.checkVersionNums(1, 5, 9));
	log( "  getLineSpace:     " + engine.getLineSpace());
	log( "  getDrawBoundingBoxes: " + engine.getDrawBoundingBoxes());
	engine.setDrawBoundingBoxes(2);
	log( "  getDrawBoundingBoxes: " + engine.getDrawBoundingBoxes());
}


function run (engine: GuidoEngine, log: TLog, svg: TSvg) {
 	ar2gr 	(engine, log, svg);
	streams (engine, log, svg);
	misc 	(engine, log);
	maps 	(engine, log);
	pianoRollTest (engine, log, svg);
	reducedPropTest (engine, log, svg);
	log ("\nEnd of tests");
}

var process: any;
var module: any;
if ((typeof process !== 'undefined') && (process.release.name === 'node')) {
	module.exports = run;
}
