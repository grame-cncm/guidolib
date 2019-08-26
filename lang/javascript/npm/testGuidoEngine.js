
"use strict";

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
	spring : 1.1,
	neighborhoodSpacing : 0,
	optimalPageFill : 0,
	resizePage2Music : 1,
	proportionalRenderingForceMultiplicator : 0
};


const fs = require ("fs");
const GuidoEngine = require ("./guidoengine");

var engine = new GuidoEngine().initialize ("./libGUIDOEngine").then (doit);


function doit (engine) {
	console.log( "Guido Engine version " + engine.getVersion().str);
 	run ( engine );
}


//----------------------------------------------------------------------------
// Parse to ar and convert to gr + change settings and convert to svg
//----------------------------------------------------------------------------
function ar2gr (engine )
{
	console.log( "\nParse and AR to GR :");
	var p = engine.openParser();
	var ar = engine.string2AR (p, gmn);
	engine.closeParser(p);
	
	// Draw in red
	engine.markVoice(ar, 2, {num : 1, denom : 4}, {num : 3, denom : 4}, 255, 0, 0);
	var gr = engine.ar2gr(ar);
	console.log ("  ar2gr: " + gr);
	engine.resizePageToMusic (gr);	

	var result = engine.gr2SVG(gr, 1, true, 0);
	fs.writeFileSync("gr2SVG1.svg", result, "utf8" );
	console.log ("  SVG flushed to gr2SVG1.svg");
	
	// Modify settings and re-draw
	engine.updateGRSettings (gr, settings);
	console.log ("  updateGRSettings");
	result = engine.gr2SVG(gr, 1, true, 0);
	fs.writeFileSync("gr2SVG2.svg", result, "utf8" );
	console.log ("  SVG flushed to gr2SVG2.svg");

	engine.freeGR (gr);
	engine.freeAR (ar);
}

//----------------------------------------------------------------------------
// Misc. functions
//----------------------------------------------------------------------------
function misc (engine )
{
	console.log( "Conversion tools :");
	console.log( "  unit2CM:     " + engine.unit2CM(237.10630798339844));
	console.log( "  cm2Unit:     " + engine.cm2Unit(0.00421751756221056));
	console.log( "  unit2Inches: " + engine.unit2Inches(602.25));
	console.log( "  inches2Unit: " + engine.inches2Unit(0.001660440000705421));

	console.log( "Misc :");
	console.log( "  checkVersionNums: " + engine.checkVersionNums(1, 5, 9).value);
	console.log( "  getLineSpace:     " + engine.getLineSpace());
	console.log( "  getDrawBoundingBoxes: " + engine.getDrawBoundingBoxes());
	engine.setDrawBoundingBoxes(2);
	console.log( "  getDrawBoundingBoxes: " + engine.getDrawBoundingBoxes());
}


function run (engine) {
 	ar2gr 	(engine);
	misc 	(engine);
}

