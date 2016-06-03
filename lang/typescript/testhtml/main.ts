///<reference path="libGUIDOEngine.d.ts"/>
///<reference path="node.d.ts"/>
	
function main() {
	let loader = new guidoLoader();
	let i = inputZone.value;
	console.log("i = " + i); 
	loader.initLoader(guidoEngine,i)
	delete guidoEngine;	
}

var guidoEngine: GuidoEngineAdapter;
var inputZone: HTMLTextAreaElement = <HTMLTextAreaElement>document.getElementById('gmnSandbox');
inputZone.addEventListener("input", main);

