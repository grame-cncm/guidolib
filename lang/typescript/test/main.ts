///<reference path="libGUIDOEngine.d.ts"/>
///<reference path="node.d.ts"/>
///<reference path="guidoTest.ts"/>


var guidoEngine: GuidoEngineAdapter;
	
function main() {
	var test = "[a g d]";
	let content = readfileJS();
    eval(content.toString());
	// string parsing test
	//loaderTest(test);
	
	// full test Guido
	fullTest(test);
}

function readfileJS(): string {
        let fs = require('fs');
	    return fs.readFileSync('libGUIDOEngine.js');
}


function fullTest(i: string): void {
	var guidoFullTest = new guidoTest(i);
	guidoFullTest.processTest();	
}


function loaderTest(i: string): void {
	let loader = new guidoLoader(); 
	loader.initLoader(guidoEngine,i)
	delete guidoEngine;		
}


main();

