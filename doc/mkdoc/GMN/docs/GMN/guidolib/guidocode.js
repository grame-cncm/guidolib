
var guidoEngine;
var gSettings;
var gParser;

function processGMNDiv( div, parser) {
	var pre = div.getElementsByTagName('pre')[0];
	var content = pre.innerHTML;
 	content = content.replace(/&lt;/g, "<").replace(/&gt;/g, ">").replace(/&amp;/g, "&");
//console.log ("processGMNDiv " + content);
	var ar = guidoEngine.string2AR(parser, content);
	if (ar) {
		var gr = guidoEngine.ar2gr(ar);
// 		div.innerHTML = guidoEngine.gr2SVG(gr, 1, false, 0);
		var p = guidoEngine.getPageCount (gr);
		if (p==1) div.innerHTML = guidoEngine.gr2SVGColored (gr, 1, 0, 0, 0, false);
		else {
			div.removeChild(pre);
			for (i=1; i <=p; i++) {
			var pagediv = document.getElementById('p'+i)
			pagediv.innerHTML = guidoEngine.gr2SVGColored (gr, i, 0, 0, 0, false);
			}
		}
		guidoEngine.freeGR(gr);
		guidoEngine.freeAR(ar);		
	}
	else {
		var error = guidoEngine.parserGetErrorCode( gParser );
		console.log ( error.msg + " line " + error.line + " col " + error.col);
	}
}

function processGMNCode() {
	var divs = document.querySelectorAll(".guido-code");
	divs.forEach( function(div) { processGMNDiv (div, gParser) });
}

var module = GuidoModule();
module['onRuntimeInitialized'] = function() {
	guidoEngine = new module.GuidoEngineAdapter();
	var version = guidoEngine.getVersion();
	console.log( "Guido Engine version " + version.str);
	guidoEngine.init();
	gSettings = guidoEngine.getDefaultLayoutSettings();
	gParser  = guidoEngine.openParser();
	processGMNCode();
}
