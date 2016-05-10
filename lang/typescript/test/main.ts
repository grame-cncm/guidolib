///<reference path="libGUIDOEngine.d.ts"/>
///<reference path="node.d.ts"/>
	
function main(i: string) {
	let loader = new guidoLoader(); 
	loader.initLoader(guidoEngine,i)
	delete guidoEngine;	
}

var guidoEngine: GuidoEngineAdapter;
var test = "[a g d]";
main(test);

