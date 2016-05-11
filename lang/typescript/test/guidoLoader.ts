///<reference path="libGUIDOEngine.d.ts"/>

class guidoLoader {

    processGMN(guidoEngine: GuidoEngineAdapter, i: string): string {
        let p = guidoEngine.openParser();
	    let ar = guidoEngine.string2AR(p, i);
	    guidoEngine.closeParser(p);
        
        let gr = guidoEngine.ar2gr(ar);
	    let result = guidoEngine.gr2SVG(gr, 1, true, 0);
	    guidoEngine.freeGR(gr);
	    guidoEngine.freeAR(ar);
                 
        console.log('résultat du parsing : ' + result);
        this.test(guidoEngine, i);
        return result;

    }
    
    // pour NODE
    readfileJS(): string {
        let fs = require('fs');
	    return fs.readFileSync('libGUIDOEngine.js');
    }
     
    initLoader(guidoEngine: GuidoEngineAdapter, i: string): void {
        let content = this.readfileJS();
        eval(content.toString());

        guidoEngine = new Module.GuidoEngineAdapter;
        guidoEngine.init();
        this.processGMN(guidoEngine, i);
    }
    
    closeLoader(): void {
        guidoEngine.shutdown();
        //delete guidoEngine;    
    }   
    
    test(guidoEngine: GuidoEngineAdapter, i: string): void {
        let p = guidoEngine.openParser();
	    let ar = guidoEngine.string2AR(p, i);
        let gr = guidoEngine.ar2gr(ar);
	    let result = guidoEngine.gr2SVG(gr, 1, true, 0);
        
        console.log('init : ' + guidoEngine.init())
        console.log('defaut settings(force) : ' + guidoEngine.getDefaultLayoutSettings().force)
        console.log('bounding boxes : ' + guidoEngine.getDrawBoundingBoxes())
        console.log('getdefaut page format (width) : ' + guidoEngine.getDefaultPageFormat().width)
        console.log('setdefaut page format (width) : ' + guidoEngine.setDefaultPageFormat(guidoEngine.getDefaultPageFormat()))
        
        console.log('version : ' +  guidoEngine.getVersion().major + '.' + 
                                    guidoEngine.getVersion().minor + '.' +
                                    guidoEngine.getVersion().sub + ' / en string : ' +
                                    guidoEngine.getVersion().str)
        console.log('checkVersion : ' + guidoEngine.checkVersionNums(1, 6, 2))
                                    
        console.log('line space : ' + guidoEngine.getLineSpace())
        console.log('getErrorStr : ' + guidoEngine.getErrorString(guidoEngine.updateGR(gr))) 
        console.log('open parser : ' + guidoEngine.openParser())
        console.log('open stream : ' + guidoEngine.openStream()) 
        
        console.log('ar2gr : ' + guidoEngine.ar2gr(ar)) 
        console.log('updateGR : ' + guidoEngine.updateGR(gr)) 
        console.log('onDraw : ' + guidoEngine.onDraw) 
        
        // OK console.log('gr2SVG : ' + guidoEngine.gr2SVG(gr, 1, true, 0)) 
        console.log('abstractExport : ' + guidoEngine.abstractExport(gr, 1)) 
        console.log('binaryExport : ' + guidoEngine.binaryExport(gr, 1)) 
        //console.log('javaScriptExport : ' + guidoEngine.javascriptExport(gr, 0))
        
        console.log('ssetDrawBound : ' + guidoEngine.setDrawBoundingBoxes(1)) 
        console.log('getDrawBoundingBoxes : ' + guidoEngine.getDrawBoundingBoxes())  
        
        console.log('unit2CM : ' + guidoEngine.unit2CM(4))  
        console.log('cm2Unit : ' + guidoEngine.cm2Unit(4))  
        console.log('unit2Inches : ' + guidoEngine.unit2Inches(3))  
        console.log('inches2Unit : ' + guidoEngine.inches2Unit(3))  
        
        console.log('resizePageToMusic : ' + guidoEngine.resizePageToMusic(gr))
          
        console.log('setSymbol : ' + guidoEngine.setSymbolPath)
        //console.log('getSymbol : ' + guidoEngine.getSymbolPath(ar))
        
        //console.log('getParsingTime : ' + guidoEngine.getParsingTime(ar))
        //console.log('getAR2GRTime : ' + guidoEngine.getAR2GRTime(gr))
        //console.log('getOnDrawTime : ' + guidoEngine.getOnDrawTime(gr))

        console.log('file2Ar : ' + guidoEngine.file2AR(p, i))        
        
        let stream = guidoEngine.openStream()
        console.log('openStream : ' + stream) 
        console.log('getStream : ' + guidoEngine.getStream(stream)) 
        console.log('whriteStream : ' + guidoEngine.writeStream(stream, 'mouahah')) 
        console.log('resetStream : ' + guidoEngine.resetStream(stream))
        console.log('closeStream : ' + guidoEngine.closeStream(stream))
         
        console.log('freeAR : ' + guidoEngine.freeAR(ar)) 
        console.log('freeGR : ' + guidoEngine.freeGR(gr)) 
        console.log('shutdown : ' + guidoEngine.shutdown())
        
        console.log('closeParser : ' + guidoEngine.closeParser(p))


    } 
}