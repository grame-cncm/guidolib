/*
 a simple test file
 intended to verify that the js interface works
 should be launched using 'node'
*/


function debugmsg(str){
	typeof console !== 'undefined' ? console.log(str) : print(str);
}

var gar = require('./libGuidoAR.js');

debugmsg ("version " + gar.guidoarVersionString());
var seq = gar.gmnGSeq("[a b]", "[c d]");
debugmsg (seq.err.value);
debugmsg (seq.str);

var par = gar.gmnGPar("[a b]", "[c d]");
debugmsg (par.err.value);
debugmsg (par.str);

var dur = gar.gmnDuration ("[ a a a a g/1 g g g]");
debugmsg (dur);


