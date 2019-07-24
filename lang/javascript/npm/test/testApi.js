
const Module = require("../libGUIDOEngine");

var guidoEngine;
var module;
Module().then(function(instance) {
	module = instance;
	guidoEngine = new module.GuidoEngineAdapter();
	guidoEngine.init();
	test();
});


var svgstart = 22;
var svgend   = 400;


var settings1= {
	systemsDistance : 75,
	systemsDistribution : 1,
	systemsDistribLimit : 0.25,
	force : 750,
	spring : 1.1,
	neighborhoodSpacing : 0,
	optimalPageFill : 1,
	resizePage2Music : 1,
	proportionalRenderingForceMultiplicator : 0
};
var settings2= {
	systemsDistance : 75,
	systemsDistribution : 1,
	systemsDistribLimit : 0.25,
	force : 600,
	spring : 1.7,
	neighborhoodSpacing : 0,
	optimalPageFill : 1,
	resizePage2Music : 1,
	proportionalRenderingForceMultiplicator : 0
};


//----------------------------------------------------------------------------
// Parse to ar and convert to gr + change settings and convert to svg
//----------------------------------------------------------------------------
function buildGr( gmn ) {
	console.log( "\nParse and AR to GR :");
	var p = guidoEngine.openParser();
	var ar = guidoEngine.string2AR (p, gmn);
	
	// Draw in red
	guidoEngine.markVoice(ar, 2, {num : 1, denom : 4}, {num : 3, denom : 4}, 255, 0, 0);
	var gr = guidoEngine.ar2grSettings(ar, settings1);
	guidoEngine.resizePageToMusic(gr).value;	

	var result = guidoEngine.gr2SVG(gr, 1, true, 0);
	console.log ("  SVG: " + result.slice(svgstart,svgend));
	
	// Modify settings and re-draw
	guidoEngine.updateGRSettings(gr, settings2);
	result = guidoEngine.gr2SVG(gr, 1, true, 0);
	console.log ("  SVG: " + result.slice(svgstart,svgend));

	guidoEngine.freeAR(ar);
	guidoEngine.closeParser(p);
	return gr;
}


//----------------------------------------------------------------------------
// test guido streams
//----------------------------------------------------------------------------
function testStream() {
	console.log( "\nGuido streams :");
	
	var p = guidoEngine.openParser();
	// Test stream and draw 
	var stream = guidoEngine.openStream();
	var code = '{[f g b/8 d f/4 e';
	guidoEngine.writeStream(stream, code);
	
	var ar = guidoEngine.stream2AR(p, stream);
	var gr = guidoEngine.ar2gr(ar);
	var result = guidoEngine.gr2SVG(gr, 1, true, 0);
	console.log ("  SVG: " + result.slice(svgstart,svgend));
	
	guidoEngine.resetStream(stream);
	guidoEngine.getStream(stream);
	guidoEngine.closeStream(stream);

	guidoEngine.freeAR(ar);
	guidoEngine.freeGR(gr);
}

function doit( gr ) 
{
	// Test abstract export
	var abstractExport = guidoEngine.abstractExport(gr, 1);	
	
	// Test methods
	var layoutSettings = guidoEngine.getDefaultLayoutSettings();
}
			
function testMap( gr ) 
{
	console.log( "\nMap tests :");
	// Score Map test
	var scoreMap = new module.GUIDOScoreMap;

	pagemap_n = scoreMap.getPageMap( gr, 1, 1000.0, 1000.0 );
	staffmap_n = scoreMap.getStaffMap( gr, 1, 1000.0, 1000.0, 1 );
	voicemap_n = scoreMap.getVoiceMap( gr, 1, 1000.0, 1000.0, 1 );
	systemmap_n = scoreMap.getSystemMap( gr, 1, 1000.0, 1000.0);

	var guidoDate = {
		"num" : 2,
		"denom" : 4
	};
	mapTime =  scoreMap.getTime(guidoDate, pagemap_n);
	map =  scoreMap.getPoint(10, 10, pagemap_n);
	timemap_n = scoreMap.getTimeMap(ar);
	
	// Free resource
	delete scoreMap;
}


//----------------------------------------------------------------------------
// Piano roll tests
//----------------------------------------------------------------------------
function pianoRollTest (module, gmn)
{
	console.log( "\nPiano roll tests :");
	var p = guidoEngine.openParser();
	var ar = guidoEngine.string2AR (p, gmn);
	// Piano Roll test
	var pianoRoll = new module.GUIDOPianoRollAdapter;
	var pr = pianoRoll.ar2PianoRoll(module.PianoRollType.kSimplePianoRoll, ar);	
	var result = pianoRoll.svgExport(pr, -1, -1);
	console.log ("  SVG: " + result.slice(svgstart,svgend));
	
	limitParams = {
		startDate : {num : 1, denom : 4},
		endDate : {num : 3, denom : 4},
		lowPitch : 1,
		highPitch : 7
	};
	pianoRoll.setLimits(pr, limitParams);
	pianoRoll.enableKeyboard(pr, true);
	pianoRoll.enableAutoVoicesColoration(pr, true);
	pianoRoll.setPitchLinesDisplayMode(pr, module.kDSharpLine + module.kGSharpLine);
	result = pianoRoll.svgExport(pr, -1, -1);
	console.log ("  SVG: " + result.slice(svgstart,svgend));
	console.log ("  getKeyboardWidth: " + pianoRoll.getKeyboardWidth(pr, 50));	

	var scoreMap = new module.GUIDOScoreMap;
	console.log ("  getPianoRollMap:  " + scoreMap.getPianoRollMap(pr, -1, -1));
	delete scoreMap;

	guidoEngine.freeAR(ar);
	guidoEngine.closeParser(p);
	return pianoRoll;
}

//----------------------------------------------------------------------------
// Factory test
//----------------------------------------------------------------------------
function factoryTest (module, pianoRoll)
{
	console.log( "\nFactory tests :");
	var factory = new module.GUIDOFactoryAdapter;
	factory.openMusic();
	factory.openVoice();
	factory.openTag("meter", 1);
	factory.addTagParameterString("3/4");
	factory.closeTag();
	factory.openChord();
	factory.openEvent("b");
	factory.addSharp();
	factory.closeEvent();
	factory.openEvent("g");
	factory.addFlat();
	factory.closeEvent();
	factory.closeChord();
	factory.openEvent("c");
	factory.setEventAccidentals(1);
	factory.setOctave(2);
	factory.setDuration(1, 2);
	factory.closeEvent();
	factory.openRangeTag("tremolo",1);
	factory.openEvent("c");
	factory.setEventAccidentals(1);
	factory.setOctave(1);
	factory.setDuration(1, 2);
	factory.closeEvent();
	factory.endTag();
	
	factory.closeVoice();
	factory.openVoice();
	factory.openEvent("e");
	factory.addSharp();
	factory.closeEvent();
	factory.closeVoice();
	
	factory.openVoice();
	factory.openEvent("g");
	factory.closeEvent();
	factory.openEvent("f");
	factory.closeEvent();
	factory.closeVoice();
	var ar = factory.closeMusic();
	console.log ("  closeMusic: " + ar);
	
	pr = pianoRoll.ar2PianoRoll(module.PianoRollType.kTrajectoryPianoRoll, ar);
	pianoRoll.setHtmlColorToVoice(pr, 1, 0x6495ed);
	pianoRoll.setRGBColorToVoice(pr, 2, 255, 0, 0, 255);
	pianoRoll.setRGBColorToVoice(pr, 3, 0, 255, 0, 255);
	pianoRoll.removeColorToVoice(pr, 3);
	pianoRoll.enableMeasureBars(pr, true);
	
	let result = pianoRoll.svgExport(pr, -1, -1);
	console.log ("  SVG: " + result.slice(svgstart,svgend));
	
	pianoRoll.destroyPianoRoll(pr);
	delete factory;
}

//----------------------------------------------------------------------------
// print page format
//----------------------------------------------------------------------------
function grMisc (gr)
{
	console.log( "\nPage format :");
	var pageFormat = guidoEngine.getPageFormat(gr, 1);
	console.log ( "  width: " + pageFormat.width + " height: " + pageFormat.height);	
	pageFormat.width = 2000.2;
	pageFormat.height = 3000.1;		
	guidoEngine.setDefaultPageFormat(pageFormat);
	pageFormat = guidoEngine.getDefaultPageFormat();
	console.log ( "  width: " + pageFormat.width + " height: " + pageFormat.height);

	console.log( "\nScore infos :");
	console.log ( "  getPageCount:   " + guidoEngine.getPageCount(gr));
	console.log ( "  getSystemCount: " + guidoEngine.getSystemCount(gr, 1));
	var date = guidoEngine.duration(gr);
	console.log ( "  duration:       " + date.num + "/" + date.denom);
	
	date = { num : 1, denom : 4};
	console.log ( "  findEventPage:  " + guidoEngine.findEventPage(gr, date));
	console.log ( "  findPageAt:     " + guidoEngine.findPageAt(gr, date));	
	var date = guidoEngine.getPageDate(gr, 1);
	console.log ( "  getPageDate:    " + date.num + "/" + date.denom);
}


//----------------------------------------------------------------------------
// Basic tests
//----------------------------------------------------------------------------
function basic ()
{
	var version = guidoEngine.getVersion();
	console.log( "Guido Engine version " + version.major + "." + version.minor + "." + version.sub + " / String version = " + version.str);
	console.log( "\nConversion tools :");
	console.log( "  unit2CM:     " + guidoEngine.unit2CM(2.3));
	console.log( "  cm2Unit:     " + guidoEngine.cm2Unit(2.5));
	console.log( "  unit2Inches: " + guidoEngine.unit2Inches(3.1));
	console.log( "  unit2Inches: " + guidoEngine.unit2Inches(1.3));
	console.log( "\nMisc :");
	console.log( "  checkVersionNums: " + guidoEngine.checkVersionNums(1, 5, 9).value);
	console.log( "  getLineSpace:     " + guidoEngine.getLineSpace());
	console.log( "  getDrawBoundingBoxes: " + guidoEngine.getDrawBoundingBoxes());
	guidoEngine.setDrawBoundingBoxes(2);
	console.log( "  getDrawBoundingBoxes: " + guidoEngine.getDrawBoundingBoxes());
}

//----------------------------------------------------------------------------
// Parse errors
//----------------------------------------------------------------------------
function parseError ()
{
	// Test parser error
	console.log( "\nParser errors :");
	var parser = guidoEngine.openParser();
	var ar = guidoEngine.string2AR(parser, "[a b c ( b]");
	var errorCode = guidoEngine.parserGetErrorCode(parser);
	console.log ( "error with [a b c ( b] line " + errorCode.line + " col " + errorCode.col + " message: " + errorCode.msg );
	guidoEngine.closeParser(parser);
}

function test() {
	basic();
	parseError();
	var gmn = "{ [f# g#/8 a&/4 b&/8 f#/4 g#], [f g a/8 b f/4 g] } ";
	var gr = buildGr (gmn);
	grMisc (gr);
	testStream();

//doit("{ [f# g#/8 a&/4 b&/8 f#/4 g#], [f g a/8 b f/4 g] } ");

	guidoEngine.freeGR(gr);

	var pianoRoll = pianoRollTest(module, gmn);
	factoryTest (module, pianoRoll);

// Stop and delete engine.
	guidoEngine.shutdown();
	delete guidoEngine;

	console.log( "=================== End of GUIDO API test ===================");
	process.exit(0);
}
