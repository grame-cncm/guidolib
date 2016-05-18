///<reference path="../libGUIDOEngine.d.ts"/>

class guidoLoader {

    processGMN(guidoEngine: GuidoEngineAdapter, i: string): string {
        let p = guidoEngine.openParser();
	    let ar = guidoEngine.string2AR(p, i);
	    guidoEngine.closeParser(p);
        
        let gr = guidoEngine.ar2gr(ar);
	    let result = guidoEngine.gr2SVG(gr, 1, true, 0);
	    guidoEngine.freeGR(gr);
	    guidoEngine.freeAR(ar);
        
        document.getElementById('canvasContainer').innerHTML = result;
         
        //console.log('résultat du parsing' + result);
        return result;
    }
    
    // pour NODE
    //readfileJS(): string {
        //let fs = require('fs');
	    //return fs.readFileSync('libGUIDOEngine.js');
    //}
     
    initLoader(guidoEngine: GuidoEngineAdapter, i: string): void {
        //let content = this.readfileJS();
        //eval(content.toString());

        guidoEngine = new Module.GuidoEngineAdapter;
        guidoEngine.init();
        this.processGMN(guidoEngine, i);
    }
    
    closeLoader(): void {
        guidoEngine.shutdown();
        //delete guidoEngine;    
    }    
}