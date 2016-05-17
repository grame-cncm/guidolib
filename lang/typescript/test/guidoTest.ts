///<reference path="libGUIDOEngine.d.ts"/>

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
    
    _checkVoid      ()                  : checkfunction { return (m) => (typeof m == undefined); }      
    _checkBool      (b: boolean)        : checkfunction { return (m) => (b === m); }   
    _checkStr       (i: string)         : checkfunction { return (m) => (i === m); }
    _checkNum       (n: number)         : checkfunction { return (m) => (n === m); }
    _checkArray     (a: Array<any>)     : checkfunction { return (m) => (a === m); }
    _checkErrCode   (e: GuidoErrCode)   : checkfunction { return (m) => (e === m); }
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


// init
//------------------------------------
    init(): void {

        let content = this.readfileJS();
        eval(content.toString());
                        
        this.fGuidoEngine   = new Module.GuidoEngineAdapter;
        this.fGuidoEngine.init();
 
        this.fGuidoVersion = this.fGuidoEngine.getVersion();

        this.fP     = this.fGuidoEngine.openParser();
        this.fAr    = this.fGuidoEngine.string2AR(this.fP, this.fGMN);
        this.fGr    = this.fGuidoEngine.ar2gr(this.fAr);
        
        this.fGuidoLayoutSettings = this.fGuidoEngine.getDefaultLayoutSettings(); 
        
        this.fPage          = 1;
        this.fbbMap         = 1;

        this.fDate = this.fGuidoEngine.getPageDate(this.fGr, this.fPage);
        this.fDate.num      = 6;
        this.fDate.denom    = 7;  
        
        this.fGuidoPageFormat = this.fGuidoEngine.getDefaultPageFormat();
                
        this.fValTest = 8;
        
        this.fGuidoStream = this.fGuidoEngine.openStream();
    }
    
    readfileJS(): string {
            let fs = require('fs');
            return fs.readFileSync('libGUIDOEngine.js');
    }



// test process
//------------------------------------
    processTest(): void {
        console.log('************************* New test *************************');
        console.log('processTest begining')
        
        this.init();
        console.log(this.fGuidoEngine.getPageCount(this.fGr))
        this.testMethod();  
        this.todo();    
    }  
    
    test(f: string, check : checkfunction): void {
        let result = eval(f);
        let expected = check(result);
        if (expected) {
            console.log ("OK        "  + f + " => " + result);
            console.log(result);
        }
        else {
            console.log ("ERROR     "  + f + " => " + result);
            console.log(result);
        }
    } 
    
    testMethod(): void {
        console.log("----Miscellaneous----");
        this.test("this.fGuidoEngine.getVersion()",                 this._checkVersion(this.fGuidoEngine.getVersion()));        
        this.test("this.fGuidoEngine.checkVersionNums(1, 5, 2)",    this._checkErrCode(0));
        this.test("this.fGuidoEngine.getLineSpace()",               this._checkNum(50));
        //this.test("this.fGuidoEngine.markVoice(this.fAr, 3, ?, ?, ?, ?, ?)", this._check?(6));
        console.log("\n");
        
        console.log("----Building abstract and graphic representations----");
        this.test("this.fGuidoEngine.init()",                                           this._checkErrCode(0));
        this.test("this.fGuidoEngine.ar2gr(this.fAr)",                                  this._checkGRHandler(this.fGr));        
        //this.test("this.fGuidoEngine.ar2gr(this.fAr, this.GuidoLayoutSettings)",        this._checkGRHandler(this.fGr));        
        this.test("this.fGuidoEngine.updateGR(this.fGr)",                               this._checkErrCode(0));        
        //this.test("this.fGuidoEngine.updateGR(this.fGr, this.GuidoLayoutSettings)",     this._checkErrCode(0));        
        this.test("this.fGuidoEngine.freeAR(this.fAr)",                                 this._checkVoid());        
        this.test("this.fGuidoEngine.freeGR(this.fGr)",                                 this._checkVoid());
        //this.test("this.fGuidoEngine.getErrorString(GuidoErrCode.guidoNoErr)",          this._checkStr("null is used to denote no error"));
        this.test("this.fGuidoEngine.getDefaultLayoutSettings()",                       this._checkGuidoLayoutSettings(this.fGuidoLayoutSettings));
        console.log("\n");        
        
        console.log("----Browsing music pages----");
        this.test("this.fGuidoEngine.countVoices(this.fAr)",                this._checkIsNum());
        //this.test("this.fGuidoEngine.getPageCount(this.fGr)",               this._checkIsNum());
        //this.test("this.fGuidoEngine.getSystemCount(this.fGr, this.fPage)", this._checkIsNum());
        this.test("this.fGuidoEngine.duration(this.fGr)",                   this._checkIsDate());
        //this.test("this.fGuidoEngine.findEventPage(this.fGr, this.fDate)",  this._checkIsNum());
        //this.test("this.fGuidoEngine.findPageAt(this.fGr, this.fDate)",     this._checkIsNum());
        //this.test("this.fGuidoEngine.getPageDate(this.fGr, this.fPageNum)", this._checkIsDate());
        console.log("\n");        

        console.log("----Score drawing and pages formating----");
        //this.test("this.fGuidoEngine.onDraw(?)",                                    this._checkErrCode(GuidoErrCode.guidoNoErr));
        //this.test("this.fGuidoEngine.gr2SVG(this.fGr, this.fPage, true, 0)",       this._checkIsStr());
        //this.test("this.fGuidoEngine.abstractExport(this.fGr, this.fPage)",         this._checkIsStr());
        //this.test("this.fGuidoEngine.binaryExport(this.fGr, this.fPage)",           this._checkIsStr());
        this.test("this.fGuidoEngine.setDrawBoundingBoxes(this.fbbMap)",             this._checkVoid());
        this.test("this.fGuidoEngine.getDrawBoundingBoxes()",                       this._checkIsNum());
        //this.test("this.fGuidoEngine.getPageFormat(this.fGr, this.fPage)",          this._checkIsPageFormat());
        this.test("this.fGuidoEngine.getDefaultPageFormat()",                       this._checkIsPageFormat());
        this.test("this.fGuidoEngine.setDefaultPageFormat(this.fGuidoPageFormat)",  this._checkVoid());
        this.test("this.fGuidoEngine.unit2CM(this.fValTest)",                       this._checkIsNum());
        this.test("this.fGuidoEngine.cm2Unit(this.fValTest)",                       this._checkIsNum());
        this.test("this.fGuidoEngine.unit2Inches(this.fValTest)",                   this._checkIsNum());
        this.test("this.fGuidoEngine.inches2Unit(this.fValTest)",                   this._checkIsNum());
        this.test("this.fGuidoEngine.resizePageToMusic(this.fGr)",                  this._checkErrCode(0));
        console.log("\n");        

        console.log("----Parsing GMN files, strings and guido streams----");
        this.test("this.fGuidoEngine.openParser()",                                    this._checkIsParser());
        this.test("this.fGuidoEngine.openStream()",                                    this._checkIsStream());
        this.test("this.fGuidoEngine.getStream(this.fGuidoStream)",                    this._checkIsStr());
        //this.test("this.fGuidoEngine.GuidoFreeStreamString(?)",                             this._checkVoid());
        this.test("this.fGuidoEngine.file2AR(this.fP, this.fGMN)",                     this._checkARHandler(this.fAr));
        this.test("this.fGuidoEngine.string2AR(this.fP, this.fGMN)",                   this._checkARHandler(this.fAr));
        this.test("this.fGuidoEngine.stream2AR(this.fP, this.fGuidoStream)",           this._checkARHandler(this.fAr));
        //this.test("this.fGuidoEngine.GuidoStream2GuidoString(this.fGuidoStream)",           this._checkIsStr());
        this.test("this.fGuidoEngine.parserGetErrorCode(this.fP)",    this._checkErrCode(0));
        this.test("this.fGuidoEngine.closeStream(this.fGuidoStream)",                  this._checkErrCode(0));
        this.test("this.fGuidoEngine.writeStream(this.fGuidoStream, this.fGMN)",       this._checkErrCode(0));
        this.test("this.fGuidoEngine.resetStream(this.fGuidoStream)",                  this._checkErrCode(0));   
        this.test("this.fGuidoEngine.closeParser(this.fP)",                            this._checkErrCode(0));
        console.log("\n");       
 
 
        console.log("----End of methods tests----");
        this.test("this.fGuidoEngine.shutdown()",   this._checkVoid());
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
