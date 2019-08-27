
"use strict";

const fs = require ("fs");
const GuidoEngine = require ("./guidoengine");
const run = require ("./testAPI");

var engine = new GuidoEngine().initialize ("./libGUIDOEngine").then (doit);

function log(str) 		{ console.log (str); }
function svg(str, name) { 
	fs.writeFileSync (name + ".svg", str, "utf8" );
	log ("  SVG flushed to " + name);
}

function doit (engine) {
	console.log( "Guido Engine version " + engine.getVersion().str);
 	run ( engine, log, svg );
}
