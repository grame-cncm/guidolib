
"use strict";

const fs = require ("fs");
const GuidoEngine = require ("../guidoengine");
const test = require ("./testAPI.ts.node");
const GuidoModule = require ("../libGUIDOEngine");

GuidoModule().then ( (module) => { doit (new GuidoEngine (module)); } );

function log(str) 		{ console.log (str); }
function svg(str, name) { 
	fs.writeFileSync (name + ".svg", str, "utf8" );
	log ("  SVG flushed to " + name);
}

function doit (engine) {
	console.log( "Guido Engine version " + engine.getVersion().str);
 	test.run ( engine, log, svg );
}
