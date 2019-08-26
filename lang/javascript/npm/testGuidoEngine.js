
"use strict";

var gmn = '{ \
	[ 	\meter<"2/4"> \stemsUp \
		\beam(g2*1/32 e*1/16 c*3/32) c2*1/8 \beam(a1*1/16 c2 f) \
		\beam(g/32 d/16 h1*3/32) d2*1/8 \beam(h1*1/16 d2 g2) \
	], \
	[	\staff<1> \stemsDown g1*1/8 e \beam(g/16 d f a) a/8 e  \beam(a/16 e g h) ], \
	[	\staff<2> \meter<"2/4"> \stemsUp a0*1/4 f h c1 ], \
	[	\staff<2> \stemsDown f0*1/4 d g a ] \
}';


const GuidoEngine = require ("./guidoengine");

var engine = new GuidoEngine().initialize ("./libGUIDOEngine").then (doit);


function doit (engine) {
	console.log( "Guido Engine version " + engine.getVersion().str);
 	run ( engine );
}


function basic (engine )
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
	basic (engine);
}

