///<reference path="../libGUIDOEngine.d.ts"/>

interface checkfunction             { (n: any): boolean; } 

class guidoTest {
    
    protected fGMN              : string;
    protected fGuidoEngine      : GuidoEngineAdapter;
    protected fGuidoVersion     : GuidoVersion; 
    protected fP                : GuidoParser;
    protected fAr               : ARHandler;   
    protected fGr               : GRHandler;
    protected fGuidoLayoutSettings: GuidoLayoutSettings; 
    protected fDate             : GuidoDate;  
    protected fPage             : number;
    protected fbbMap            : number;
    protected fGuidoPageFormat  : GuidoPageFormat;
    protected fValTest          : number;
    protected fGuidoStream      : GuidoStream;
 
    constructor (inputString: string) {
        this.fGMN = inputString;
    }

// CLOSURES
//------------------------------------
    
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
    _checkIsParser  ()         : checkfunction { return (m) => (typeof m == typeof this.fP); }
    _checkIsStream  ()         : checkfunction { return (m) => (typeof m == typeof this.fGuidoStream); }
    
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

// init
//------------------------------------
    init(): void {

        let content = this.readfileJS();
        eval(content.toString());
                        
        this.fGuidoEngine   = new Module.GuidoEngineAdapter;
        this.fGuidoEngine.init();
        
        this.fP     = this.fGuidoEngine.openParser();
        this.fAr    = this.fGuidoEngine.string2AR(this.fP, this.fGMN);
        this.fGr    = this.fGuidoEngine.ar2gr(this.fAr);
        
        this.fbbMap     = 1;
        this.fValTest   = 8;
        this.fPage      = 1;

        this.fGuidoPageFormat       = this.fGuidoEngine.getDefaultPageFormat();
        this.fGuidoStream           = this.fGuidoEngine.openStream();
        this.fGuidoLayoutSettings   = this.fGuidoEngine.getDefaultLayoutSettings(); 
        this.fDate                  = this.fGuidoEngine.getPageDate(this.fGr, this.fPage);
    }
    
    readfileJS(): string {
            let fs = require('fs');
            return fs.readFileSync('../../javascript/libGUIDOEngine.js');
    }

// test process
//------------------------------------
    processTest(): void {
        console.log('************************* New test *************************');
        
        this.init();
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
            console.log ("OK        "  + f + " => " + result);
        }
        else {
            console.log ("ERROR     "  + f + " => " + result);
            console.log(result);
        }
    } 
    
    testRun(f: string): void {
        let result = eval(f);
        if (result) {
            console.log ("OK        "  + f + " => " + result);
        }
        else {
            console.log ("ERROR     "  + f + " => " + result);
            console.log(result);
        }
    } 
    
// different tests
//------------------------------------    
    
    testMisc(): void {
        console.log("----Miscellaneous----");
        this.testExpect("this.fGuidoEngine.getVersion()",                 this._checkIsVersion());        
        this.testExpect("this.fGuidoEngine.checkVersionNums(1, 0, 2)",    this._checkErrCode(0));
        this.testExpect("this.fGuidoEngine.getLineSpace()",               this._checkNum(50));
        //this.testExpect("this.fGuidoEngine.markVoice(this.fAr, 3, ?, ?, ?, ?, ?)", this._check?(6));
        console.log("\n");
    }   
 
    testBuild(): void {
        console.log("----Building abstract and graphic representations----");
        this.testExpect("this.fGuidoEngine.init()",                                         this._checkErrCode(0));
        this.testRun("this.fGuidoEngine.ar2gr(this.fAr)");        
        //this.testExpect("this.fGuidoEngine.ar2gr(this.fAr, this.GuidoLayoutSettings)",      this._checkGRHandler(this.fGr));        
        this.testExpect("this.fGuidoEngine.updateGR(this.fGr)",                             this._checkErrCode(0));        
        //this.testExpect("this.fGuidoEngine.updateGR(this.fGr, this.GuidoLayoutSettings)",   this._checkErrCode(0));        
        this.testExpect("this.fGuidoEngine.freeAR(this.fAr)",                               this._checkVoid());        
        this.testExpect("this.fGuidoEngine.freeGR(this.fGr)",                               this._checkVoid());
        //this.testExpect("this.fGuidoEngine.getErrorString(GuidoErrCode.guidoNoErr)",        this._checkStr("null is used to denote no error"));
        this.testExpect("this.fGuidoEngine.getDefaultLayoutSettings()",                     this._checkIsGuidoLayoutSettings());
        console.log("\n"); 
    }    
 
    testBrows(): void {
        console.log("----Browsing music pages----");
        this.testExpect("this.fGuidoEngine.countVoices(this.fAr)",                this._checkIsNum());
        //this.testExpect("this.fGuidoEngine.getPageCount(this.fGr)",               this._checkIsNum());
        //this.testExpect("this.fGuidoEngine.getSystemCount(this.fGr, this.fPage)", this._checkIsNum());
        this.testExpect("this.fGuidoEngine.duration(this.fGr)",                   this._checkIsDate());
        //this.testExpect("this.fGuidoEngine.findEventPage(this.fGr, this.fDate)",  this._checkIsNum());
        //this.testExpect("this.fGuidoEngine.findPageAt(this.fGr, this.fDate)",     this._checkIsNum());
        //this.testExpect("this.fGuidoEngine.getPageDate(this.fGr, this.fPageNum)", this._checkIsDate());
        console.log("\n");  
    } 
    
    testPages(): void {
        console.log("----Score drawing and pages formating----");
        //this.testExpect("this.fGuidoEngine.onDraw(?)",                                      this._checkErrCode(GuidoErrCode.guidoNoErr));
        //this.testExpect("this.fGuidoEngine.gr2SVG(this.fGr, this.fPage, true, 0)",          this._checkIsStr());
        //this.testExpect("this.fGuidoEngine.abstractExport(this.fGr, this.fPage)",           this._checkIsStr());
        //this.testExpect("this.fGuidoEngine.binaryExport(this.fGr, this.fPage)",             this._checkIsStr());
        this.testExpect("this.fGuidoEngine.setDrawBoundingBoxes(this.fbbMap)",              this._checkVoid());
        this.testExpect("this.fGuidoEngine.getDrawBoundingBoxes()",                         this._checkIsNum());
        //this.testExpect("this.fGuidoEngine.getPageFormat(this.fGr, this.fPage)",            this._checkIsPageFormat());
        this.testExpect("this.fGuidoEngine.getDefaultPageFormat()",                         this._checkIsPageFormat());
        this.testExpect("this.fGuidoEngine.setDefaultPageFormat(this.fGuidoPageFormat)",    this._checkVoid());
        this.testExpect("this.fGuidoEngine.unit2CM(this.fValTest)",                         this._checkIsNum());
        this.testExpect("this.fGuidoEngine.cm2Unit(this.fValTest)",                         this._checkIsNum());
        this.testExpect("this.fGuidoEngine.unit2Inches(this.fValTest)",                     this._checkIsNum());
        this.testExpect("this.fGuidoEngine.inches2Unit(this.fValTest)",                     this._checkIsNum());
        this.testExpect("this.fGuidoEngine.resizePageToMusic(this.fGr)",                    this._checkErrCode(0));
        console.log("\n");  
    } 
    
    testParse(): void {
        console.log("----Parsing GMN files, strings and guido streams----");
        this.testRun("this.fGuidoEngine.openParser()");
        this.testRun("this.fGuidoEngine.openStream()");
        this.testExpect("this.fGuidoEngine.getStream(this.fGuidoStream)",                   this._checkIsStr());
        //this.testExpect("this.fGuidoEngine.GuidoFreeStreamString(?)",                        this._checkVoid());
        this.testRun("this.fGuidoEngine.file2AR(this.fP, this.fGMN)");
        this.testRun("this.fGuidoEngine.string2AR(this.fP, this.fGMN)");
        this.testRun("this.fGuidoEngine.stream2AR(this.fP, this.fGuidoStream)");
        //this.testExpect("this.fGuidoEngine.GuidoStream2GuidoString(this.fGuidoStream)",      this._checkIsStr());
        this.testExpect("this.fGuidoEngine.parserGetErrorCode(this.fP)",                    this._checkIsParserErr());
        this.testExpect("this.fGuidoEngine.closeStream(this.fGuidoStream)",                 this._checkErrCode(0));
        this.testExpect("this.fGuidoEngine.writeStream(this.fGuidoStream, this.fGMN)",      this._checkErrCode(0));
        this.testExpect("this.fGuidoEngine.resetStream(this.fGuidoStream)",                 this._checkErrCode(0));   
        this.testExpect("this.fGuidoEngine.closeParser(this.fP)",                           this._checkErrCode(0));
        console.log("\n");
    }    
    
    testEnd(): void {
        console.log("----End of methods tests----");
        this.testExpect("this.fGuidoEngine.shutdown()",   this._checkVoid());
        console.log("\n");  
    }                      
    
    todo(): void {
        console.log("------------- TODO -------------"      + "\n" +
                    "Missing declarations in .cpp file : Timing measurements"  + "\n" +
                    "=> guidoEngine.getParsingTime"         + "\n" +
                    "=> guidoEngine.getAR2GRTime"           + "\n" + 
                    "=> guidoEngine.getOnDrawTime"          + "\n" +
                    "=> guidoEngine.setSymbolPath"          + "\n" +
                    "=> guidoEngine.getSymbolPath"          + "\n"     
                    );     
    }
}    
