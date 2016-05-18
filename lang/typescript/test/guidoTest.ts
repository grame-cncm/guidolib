///<reference path="../libGUIDOEngine.d.ts"/>
///<reference path="../libGUIDOEngine.ts"/>

interface checkfunction             { (n: any): boolean; } 

var gmnString = '[a e m]';
var gmnStream = '[a e m';

var bbMap: number     = 1;
var valTest: number   = 8;
var page:number      = 1;

var guidoEngine         : GuidoEngineAdapter;
var parseur             : GuidoParser;
var ar                  : ARHandler;
var gr                  : GRHandler;       
var date                : GuidoDate;
var guidoPageFormat     : GuidoPageFormat;
var guidoStream         : GuidoStream;
var guidoLayoutSettings : GuidoLayoutSettings;

function initGlobalVar() {
    let content = readfileJS();
    eval(content.toString());    
    guidoEngine         = new Module.GuidoEngineAdapter;
    guidoEngine.init();
    parseur             = guidoEngine.openParser();
    ar                  = guidoEngine.string2AR(parseur, gmnString);
    gr                  = guidoEngine.ar2gr(ar);       
    date                = guidoEngine.getPageDate(gr, page);
    guidoPageFormat     = guidoEngine.getDefaultPageFormat();
    guidoStream         = guidoEngine.openStream();
    guidoLayoutSettings = guidoEngine.getDefaultLayoutSettings();   
}


class guidoTest {
    
// CLOSURES
//------------------------------------

    _checkRun       ()                  : checkfunction { return (m) => true }          
    _checkVoid      ()                  : checkfunction { return (m) => (typeof m == 'undefined'); }      
    _checkBool      (b: boolean)        : checkfunction { return (m) => (b === m); }   
    _checkStr       (i: string)         : checkfunction { return (m) => (i === m); }
    _checkNum       (n: number)         : checkfunction { return (m) => (n === m); }
    _checkArray     (a: Array<any>)     : checkfunction { return (m) => (a === m); }
    _checkErrCode   (e: number)         : checkfunction { return (m) => (e === m.value); }
    _checkVersion   (v: GuidoVersion)   : checkfunction { return (m) => (v === m); }
    _checkARHandler (ar: ARHandler)     : checkfunction { return (m) => (ar === m); }
    _checkGRHandler (gr: GRHandler)     : checkfunction { return (m) => (gr === m); }
    _checkGuidoLayoutSettings (s: GuidoLayoutSettings) : checkfunction { return (m) => (s === m); }
   
    _checkIsNum     ()         : checkfunction { return (m) => (typeof m == 'number'); }
    _checkIsStr     ()         : checkfunction { return (m) => (typeof m == 'string'); }
    _checkIsDate    ()         : checkfunction { return (m) => (typeof m.num == 'number' && typeof m.denom == 'number' ); }
    
    _checkIsPageFormat ()      : checkfunction { return (m) => (typeof m.width == 'number'          && typeof m.height == 'number'      && 
                                                                typeof m.marginleft == 'number'     && typeof m.margintop == 'number'   &&
                                                                typeof m.marginright == 'number'    && typeof m.marginbottom == 'number'
                                                                ); }
    
    _checkIsVersion ()         : checkfunction { return (m) => (typeof m.major == 'number'  && typeof m.minor == 'number'   &&
                                                                typeof m.sub == 'number'    && typeof m.str == 'string'
                                                                ); }
    _checkIsGuidoLayoutSettings () : checkfunction { return (m) => (typeof m.systemsDistance == 'number'        && typeof m.systemsDistribution == 'number' && 
                                                                    typeof m.systemsDistribLimit == 'number'    && typeof m.force == 'number'               &&
                                                                    typeof m.spring == 'number'                 && typeof m.neighborhoodSpacing == 'number' &&
                                                                    typeof m.optimalPageFill == 'number'        && typeof m.resizePage2Music == 'number'    &&
                                                                    typeof m.proportionalRenderingForceMultiplicator == 'number'
                                                                    ); }
    _checkIsParserErr ()       : checkfunction { return (m) => (typeof m.line == 'number' && typeof m.col == 'number' && typeof m.msg == 'string'); }

// test process
//------------------------------------
    processTest(): void {
        console.log('************************* New test *************************');
        
        this.testMisc();
        this.testBuild(); 
        this.testBrows(); 
        this.testPages();
        this.testParse(); 
        this.testEnd();       
        this.todo();    
    }  
    
    testExpect(f: string, check : checkfunction): void {
        let result = eval(f);
        let expected = check(result);
        if (expected) {
            console.log ("OK        "  + f);
        }
        else {
            console.log ("ERROR     "  + f + " => " + result);
            console.log(result);
        }
    } 
    
// different tests
//------------------------------------    
    
    testMisc(): void {
        initGlobalVar();
        console.log("----Miscellaneous----");
        this.testExpect("guidoEngine.getVersion()",                 this._checkIsVersion());        
        this.testExpect("guidoEngine.checkVersionNums(1, 0, 2)",    this._checkErrCode(GuidoErrCode.guidoNoErr));
        this.testExpect("guidoEngine.getLineSpace()",               this._checkIsNum());
        //this.testExpect("guidoEngine.markVoice(this.fAr, 3, ?, ?, ?, ?, ?)", this._check?(6));
        console.log("\n");
    }   
 
    testBuild(): void {
        initGlobalVar();
        console.log("----Building abstract and graphic representations----");
        this.testExpect("guidoEngine.init()",                                     this._checkErrCode(GuidoErrCode.guidoNoErr));
        this.testExpect("guidoEngine.ar2gr(ar)",                                  this._checkRun());        
        //this.testRun("guidoEngine.ar2gr(ar, guidoLayoutSettings)");        
        this.testExpect("guidoEngine.updateGR(gr)",                               this._checkErrCode(GuidoErrCode.guidoNoErr));        
        //this.testExpect("guidoEngine.updateGR(gr, this.guidoLayoutSettings)",   this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoEngine.freeAR(ar)",                                 this._checkVoid());        
        this.testExpect("guidoEngine.freeGR(gr)",                                 this._checkVoid());
        this.testExpect("guidoEngine.getErrorString(GuidoErrCode.guidoNoErr)",    this._checkStr("No error"));
        this.testExpect("guidoEngine.getDefaultLayoutSettings()",                 this._checkIsGuidoLayoutSettings());
        console.log("\n"); 
    }    
 
    testBrows(): void {
        initGlobalVar();
        console.log("----Browsing music pages----");
        this.testExpect("guidoEngine.countVoices(ar)",          this._checkIsNum());
        this.testExpect("guidoEngine.getPageCount(gr)",         this._checkIsNum());
        this.testExpect("guidoEngine.getSystemCount(gr, page)", this._checkIsNum());
        this.testExpect("guidoEngine.duration(gr)",             this._checkIsDate());
        this.testExpect("guidoEngine.findEventPage(gr, date)",  this._checkIsNum());
        this.testExpect("guidoEngine.findPageAt(gr, date)",     this._checkIsNum());
        this.testExpect("guidoEngine.getPageDate(gr, page)",    this._checkIsDate());
        console.log("\n");  
    } 
    
    testPages(): void {
        initGlobalVar();
        console.log("----Score drawing and pages formating----");
        //this.testExpect("guidoEngine.onDraw(?)",                            this._checkErrCode(GuidoErrCode.guidoNoErr));
        this.testExpect("guidoEngine.gr2SVG(gr, page, true, 0)",              this._checkIsStr());
        this.testExpect("guidoEngine.abstractExport(gr, page)",               this._checkIsStr());
        this.testExpect("guidoEngine.binaryExport(gr, page)",                 this._checkIsStr());
        this.testExpect("guidoEngine.setDrawBoundingBoxes(bbMap)",            this._checkVoid());
        this.testExpect("guidoEngine.getDrawBoundingBoxes()",                 this._checkIsNum());
        this.testExpect("guidoEngine.getPageFormat(gr, page)",                this._checkIsPageFormat());
        this.testExpect("guidoEngine.getDefaultPageFormat()",                 this._checkIsPageFormat());
        this.testExpect("guidoEngine.setDefaultPageFormat(guidoPageFormat)",  this._checkVoid());
        this.testExpect("guidoEngine.unit2CM(valTest)",                       this._checkIsNum());
        this.testExpect("guidoEngine.cm2Unit(valTest)",                       this._checkIsNum());
        this.testExpect("guidoEngine.unit2Inches(valTest)",                   this._checkIsNum());
        this.testExpect("guidoEngine.inches2Unit(valTest)",                   this._checkIsNum());
        this.testExpect("guidoEngine.resizePageToMusic(gr)",                  this._checkErrCode(GuidoErrCode.guidoNoErr));
        console.log("\n");  
    } 
    
    testParse(): void {
        initGlobalVar();
        console.log("----Parsing GMN files, strings and guido streams----");
        this.testExpect("guidoEngine.openParser()",                           this._checkRun());
        this.testExpect("guidoEngine.openStream()",                           this._checkRun());
        this.testExpect("guidoEngine.getStream(guidoStream)",                 this._checkIsStr());
        this.testExpect("guidoEngine.file2AR(parseur, gmnString)",            this._checkRun());
        this.testExpect("guidoEngine.string2AR(parseur, gmnString)",          this._checkRun());
        //this.testExpect("guidoEngine.stream2AR(parseur, gmnString)",          this._checkRun());
        this.testExpect("guidoEngine.parserGetErrorCode(parseur)",            this._checkIsParserErr());
        this.testExpect("guidoEngine.closeStream(guidoStream)",               this._checkErrCode(GuidoErrCode.guidoNoErr));
        this.testExpect("guidoEngine.writeStream(guidoStream, gmnString)",    this._checkErrCode(GuidoErrCode.guidoNoErr));
        this.testExpect("guidoEngine.resetStream(guidoStream)",               this._checkErrCode(GuidoErrCode.guidoNoErr));   
        this.testExpect("guidoEngine.closeParser(parseur)",                   this._checkErrCode(GuidoErrCode.guidoNoErr));
        console.log("\n");
    }    
    
    testEnd(): void {
        initGlobalVar();
        console.log("----End of methods tests----");
        this.testExpect("guidoEngine.shutdown()",   this._checkVoid());
        console.log("\n");  
    }                      
    
    todo(): void {
        console.log("------------- TODO -------------"      + "\n" +
                    "Missing declarations in .cpp file : Timing measurements"  + "\n" +
                    "=> guidoEngine.getParsingTime"         + "\n" +
                    "=> guidoEngine.getAR2GRTime"           + "\n" + 
                    "=> guidoEngine.getOnDrawTime"          + "\n" +
                    "=> guidoEngine.setSymbolPath"          + "\n" +
                    "=> guidoEngine.getSymbolPath"          + "\n" +
                    "Missing declarations in .cpp file : Parsing GMN files, strings and guido streams"  + "\n" +
                    "=> guidoEngine.GuidoStream2GuidoString"    + "\n" +
                    "=> guidoEngine.GuidoFreeStreamString"      + "\n"     
                    );     
    }
}    