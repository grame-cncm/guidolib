///<reference path="../libGUIDOEngine.d.ts"/>
///<reference path="../libGUIDOEngine.ts"/>
///<reference path="node.d.ts"/>

interface checkfunction     { (n: any): boolean; } 

// guidoEngine var
//------------------------------------
var gmnString = '[ f g b/8 d f/4 e]';
var gmnStream = '[ f g b/8 d f/4 e';

var bbMap   : number = 1;
var valTest : number = 8;
var page    : number = 1;

var guidoEngine         : GuidoEngineAdapter;
var parseur             : GuidoParser;
var ar                  : ARHandler;
var gr                  : GRHandler;       
var date                : GuidoDate = {num: 1, denom: 1};
var duration            : GuidoDate = {num: 1, denom: 1};
var guidoPageFormat     : GuidoPageFormat;
var guidoStream         : GuidoStream;
var guidoLayoutSettings : GuidoLayoutSettings;

// guidoMap var
//------------------------------------
var w       : number = 1;
var h       : number = 1;
var pageNum : number = 1;
var staff   : number = 1;
var voice   : number = 1;
var x       : number = 1;
var y       : number = 1;

var map     : Time2GraphicMap;
var t       : TimeSegment;
var rect    : Rect;
var f       : TimeMapCollector;

var guidoScoreMap       : GuidoScoreMapAdapter;

// guidoFactory var
//------------------------------------ 
var inEventName : string = 'rest';
var dots        : number = 1;
var accident    : number = 1;
var octave      : number = 2;
var numerator   : number = 1;
var denominator : number = 1; 
var name        : string = 'Name';
var tagID       : number = 1;
var valStr      : string = 'toto';
var valNum      : number = 1;
var unit        : string = 'unit';

var guidoFactory : GUIDOFactoryAdapter;

// guidoPianoRoll var
//------------------------------------ 
var height          : number = 1;
var width           : number = 1;
var midiFileName    : string = 'toto';
var enabled         : boolean = true;
var voiceNum        : number = 1;
var r: number =150, g: number = 150, b: number = 150, a: number = 0;
var color           : number = 2;
var mode            : number = 1;

//var dev             : VGDevice = 
var endDate         : GuidoDate = {num: 2, denom: 2};  
var limitParams     : LimitParams = {startDate: date, endDate: endDate, lowPitch: 1, highPitch: 2};
var pr              : PianoRoll;
var type            : PianoRollType = PianoRollType.kSimplePianoRoll;
var guidoPianoRoll  : GUIDOPianoRollAdapter;

  
// init
//------------------------------------
function init() {
	var guido = require('../../javascript/libGUIDOEngine.js');
    guidoEngine     = new guido.GuidoEngineAdapter;
    guidoEngine.init();
    guidoScoreMap   = new guido.GUIDOScoreMap;
    guidoFactory    = new guido.GUIDOFactoryAdapter(); 
    guidoPianoRoll  = new guido.GUIDOPianoRollAdapter;    
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
   
    _checkIsBool    ()         : checkfunction { return (m) => (typeof m == 'boolean'); }   
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
		init();
        console.log('************************* New test *************************');
        console.log('**               GuidoEngine Version ' + guidoEngine.getVersion().str + '                **');  
        console.log('************************************************************' + '\n');  
        
        this.processMoteur();
        this.processMap();
        this.processFactory();
        this.processPianoRoll();
        this.todo();
        
        console.log('************************* Test End *************************');             
    }
    
    processMoteur():void {
        console.log('   ** Test GuidoEgineAdapter **' + '\n');
        
        this.testMisc();
        this.testBuild(); 
        this.testBrows(); 
        this.testPages();
        this.testParse();
        this.testEnd();       
    } 
    
    processMap():void {
        console.log('   ** Test GuidoMap **' + '\n');
        parseur = this.testExpect("guidoEngine.openParser()", 			        this._checkRun());
        ar = this.testExpect("guidoEngine.string2AR(parseur, gmnString)", 	    this._checkRun());
        gr = this.testExpect("guidoEngine.ar2gr(ar)",                           this._checkRun());       

        this.testExpect("guidoScoreMap.getPageMap(gr, pageNum, w, h)",          this._checkRun());        
        this.testExpect("guidoScoreMap.getStaffMap(gr, pageNum, w, h, staff)",  this._checkRun());        
        this.testExpect("guidoScoreMap.getVoiceMap(gr, pageNum, w, h, voice)",  this._checkRun());        
        this.testExpect("guidoScoreMap.getSystemMap(gr, pageNum, w, h)",        this._checkRun());        
        //this.testExpect("guidoScoreMap.getTime(date, map, t, rect)",            this._checkIsBool());        
        //this.testExpect("guidoScoreMap.getPoint(x, y, map, t, rect)",           this._checkIsBool());        
        //this.testExpect("guidoScoreMap.getTimeMap(gr, f)",                      this._checkErrCode(GuidoErrCode.guidoNoErr));        
   
        this.testExpect("guidoEngine.freeGR(gr)",                               this._checkVoid());
        this.testExpect("guidoEngine.freeAR(ar)",                               this._checkVoid());        
        this.testExpect("guidoEngine.closeParser(parseur)",                     this._checkErrCode(GuidoErrCode.guidoNoErr));        
        console.log('\n');
    } 
    
    processFactory(): void {
        console.log('   ** Test GuidoFactory **' + '\n');
        this.testExpect("guidoFactory.openMusic()",                         this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoFactory.openVoice()",                         this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoFactory.openChord()",                         this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoFactory.insertCommata()",                     this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoFactory.openEvent(inEventName)",              this._checkErrCode(GuidoErrCode.guidoNoErr));        
        //this.testExpect("guidoFactory.openRangeTag(name, tagID)",           this._checkErrCode(GuidoErrCode.guidoNoErr));        
        //this.testExpect("guidoFactory.openTag(name, tagID)",                this._checkErrCode(GuidoErrCode.guidoNoErr));               
        //this.testExpect("guidoFactory.addSharp()",                          this._checkErrCode(GuidoErrCode.guidoNoErr));        
        //this.testExpect("guidoFactory.addFlat()",                           this._checkErrCode(GuidoErrCode.guidoNoErr));      
        //this.testExpect("guidoFactory.setEventAccidentals(accident)",       this._checkErrCode(GuidoErrCode.guidoNoErr));          
        this.testExpect("guidoFactory.setEventDots(dots)",                  this._checkErrCode(GuidoErrCode.guidoNoErr));                
        this.testExpect("guidoFactory.setOctave(octave)",                   this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoFactory.setDuration(numerator, denominator)", this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoFactory.endTag()",                            this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoFactory.addTagParameterString(valStr)",       this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoFactory.addTagParameterInt(valNum)",          this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoFactory.setParameterName(name)",              this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoFactory.setParameterUnit(unit)",              this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoFactory.closeTag()",                          this._checkErrCode(GuidoErrCode.guidoNoErr));
        this.testExpect("guidoFactory.closeEvent()",                        this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoFactory.closeChord()",                        this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoFactory.closeVoice()",                        this._checkErrCode(GuidoErrCode.guidoNoErr));                
        this.testExpect("guidoFactory.closeMusic()",                        this._checkRun());        
        console.log('\n');                    
    }
    
    processPianoRoll(): void {
        console.log('   ** Test GuidoPianoRoll **' + '\n');
        parseur = this.testExpect("guidoEngine.openParser()", 			                this._checkRun());
        ar = this.testExpect("guidoEngine.string2AR(parseur, gmnString)", 	            this._checkRun());               
        pr = this.testExpect("guidoPianoRoll.ar2PianoRoll(type, ar)",                   this._checkRun());        
        this.testExpect("guidoPianoRoll.svgExport(pr, width, height)",                  this._checkIsStr()); 
//        console.log(guidoPianoRoll.svgExport(pr, -1, -1));       
        // use canvas
        //this.testExpect("guidoPianoRoll.javascriptExport(pr, width, height)",         this._checkErrCode(GuidoErrCode.guidoNoErr));                
        this.testExpect("guidoPianoRoll.setLimits(pr, limitParams)",                    this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoPianoRoll.enableKeyboard(pr, enabled)",                   this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoPianoRoll.getKeyboardWidth(pr, height)",                  this._checkIsNum());        
        this.testExpect("guidoPianoRoll.enableAutoVoicesColoration(pr, enabled)",       this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoPianoRoll.setRGBColorToVoice(pr, voiceNum, r, g, b, a)",  this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoPianoRoll.setHtmlColorToVoice(pr, voiceNum, color)",      this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoPianoRoll.removeColorToVoice(pr, voiceNum)",              this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoPianoRoll.enableMeasureBars(pr, enabled)",                this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoPianoRoll.setPitchLinesDisplayMode(pr, mode)",            this._checkErrCode(GuidoErrCode.guidoNoErr));          
        this.testExpect("guidoPianoRoll.destroyPianoRoll(pr)",                          this._checkErrCode(GuidoErrCode.guidoNoErr));                
        this.testExpect("guidoEngine.freeAR(ar)",                                       this._checkVoid());        
        this.testExpect("guidoEngine.closeParser(parseur)",                             this._checkErrCode(GuidoErrCode.guidoNoErr));              
        console.log('\n');
    }
    
    testExpect(f: string, check : checkfunction): any {
        let result = eval(f);
        let expected = check(result);
        if (expected) {
            console.log ("OK        "  + f);
        }
        else {
            console.log ("ERROR     "  + f + " => " + result);
            console.log(result);
        }
        return result;
    } 
    
// different tests
//------------------------------------    
    
    testMisc(): void {
        console.log("----Miscellaneous----");
        parseur = this.testExpect("guidoEngine.openParser()", 				        this._checkRun());
        ar = this.testExpect("guidoEngine.string2AR(parseur, gmnString)",           this._checkRun());
        
        this.testExpect("guidoEngine.init()",                                       this._checkErrCode(GuidoErrCode.guidoNoErr));
        this.testExpect("guidoEngine.getVersion()",                                 this._checkIsVersion());        
        this.testExpect("guidoEngine.checkVersionNums(1, 0, 4)",                    this._checkErrCode(GuidoErrCode.guidoNoErr));
        this.testExpect("guidoEngine.getLineSpace()",                               this._checkIsNum());
        this.testExpect("guidoEngine.markVoice(ar, voiceNum, date, duration, 10, 10, 10)", this._checkErrCode(GuidoErrCode.guidoNoErr));
        
        this.testExpect("guidoEngine.freeAR(ar)",                                   this._checkVoid());        
        this.testExpect("guidoEngine.closeParser(parseur)",                         this._checkErrCode(GuidoErrCode.guidoNoErr));
        console.log("\n");
    }   
 
    testBuild(): void {
        console.log("----Building abstract and graphic representations----");
        parseur = this.testExpect("guidoEngine.openParser()", 				            this._checkRun());
        ar = this.testExpect("guidoEngine.string2AR(parseur, gmnString)", 	            this._checkRun());
        gr = this.testExpect("guidoEngine.ar2gr(ar)",                                   this._checkRun());        
        guidoLayoutSettings = this.testExpect("guidoEngine.getDefaultLayoutSettings()", this._checkRun());
        
        this.testExpect("guidoEngine.ar2grSettings(ar, guidoLayoutSettings)",           this._checkRun());        
        this.testExpect("guidoEngine.updateGR(gr)",                                     this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoEngine.updateGRSettings(gr, guidoLayoutSettings)",        this._checkErrCode(GuidoErrCode.guidoNoErr));        
        this.testExpect("guidoEngine.freeGR(gr)",                                       this._checkVoid());
        this.testExpect("guidoEngine.freeAR(ar)",                                       this._checkVoid());        
        this.testExpect("guidoEngine.closeParser(parseur)",                             this._checkErrCode(GuidoErrCode.guidoNoErr));
        this.testExpect("guidoEngine.getErrorString(GuidoErrCode.guidoNoErr)",          this._checkStr("No error"));
        this.testExpect("guidoEngine.getDefaultLayoutSettings()",                       this._checkIsGuidoLayoutSettings());
        console.log("\n"); 
    }    
 
    testBrows(): void {
        console.log("----Browsing music pages----");
        parseur = this.testExpect("guidoEngine.openParser()", 				this._checkRun());
        ar = this.testExpect("guidoEngine.string2AR(parseur, gmnString)", 	this._checkRun());
        gr = this.testExpect("guidoEngine.ar2gr(ar)",                       this._checkRun());  
              
        this.testExpect("guidoEngine.countVoices(ar)",                      this._checkIsNum());
        this.testExpect("guidoEngine.getPageCount(gr)",                     this._checkIsNum());
        this.testExpect("guidoEngine.getSystemCount(gr, page)",             this._checkIsNum());
        this.testExpect("guidoEngine.duration(gr)",                         this._checkIsDate());
        this.testExpect("guidoEngine.findEventPage(gr, date)",              this._checkIsNum());
        this.testExpect("guidoEngine.findPageAt(gr, date)",                 this._checkIsNum());
        this.testExpect("guidoEngine.getPageDate(gr, page)",                this._checkIsDate());
        
        this.testExpect("guidoEngine.freeGR(gr)",                           this._checkVoid());
        this.testExpect("guidoEngine.freeAR(ar)",                           this._checkVoid());        
        this.testExpect("guidoEngine.closeParser(parseur)",                 this._checkErrCode(GuidoErrCode.guidoNoErr));
        console.log("\n");  
    } 
    
    testPages(): void {
        console.log("----Score drawing and pages formating----");
        parseur = this.testExpect("guidoEngine.openParser()", 				    this._checkRun());
        ar = this.testExpect("guidoEngine.string2AR(parseur, gmnString)", 	    this._checkRun());
        gr = this.testExpect("guidoEngine.ar2gr(ar)",                           this._checkRun());  

        this.testExpect("guidoEngine.gr2SVG(gr, page, true, 0)",                this._checkIsStr());
        this.testExpect("guidoEngine.abstractExport(gr, page)",                 this._checkIsStr());
        this.testExpect("guidoEngine.binaryExport(gr, page)",                   this._checkIsStr());
        this.testExpect("guidoEngine.setDrawBoundingBoxes(bbMap)",              this._checkVoid());
        this.testExpect("guidoEngine.getDrawBoundingBoxes()",                   this._checkIsNum());
        this.testExpect("guidoEngine.getPageFormat(gr, page)",                  this._checkIsPageFormat());
        guidoPageFormat = this.testExpect("guidoEngine.getDefaultPageFormat()", this._checkIsPageFormat());
        this.testExpect("guidoEngine.setDefaultPageFormat(guidoPageFormat)",    this._checkVoid());
        this.testExpect("guidoEngine.unit2CM(valTest)",                         this._checkIsNum());
        this.testExpect("guidoEngine.cm2Unit(valTest)",                         this._checkIsNum());
        this.testExpect("guidoEngine.unit2Inches(valTest)",                     this._checkIsNum());
        this.testExpect("guidoEngine.inches2Unit(valTest)",                     this._checkIsNum());
        this.testExpect("guidoEngine.resizePageToMusic(gr)",                    this._checkErrCode(GuidoErrCode.guidoNoErr));
        
        this.testExpect("guidoEngine.freeGR(gr)",                               this._checkVoid());
        this.testExpect("guidoEngine.freeAR(ar)",                               this._checkVoid());        
        this.testExpect("guidoEngine.closeParser(parseur)",                     this._checkErrCode(GuidoErrCode.guidoNoErr));
        console.log("\n");  
    } 
    
    testParse(): void {
        console.log("----Parsing GMN files, strings and guido streams----");
        parseur = this.testExpect("guidoEngine.openParser()",               this._checkRun());

        this.testExpect("guidoEngine.file2AR(parseur, 'test.gmn')",         this._checkRun());
        this.testExpect("guidoEngine.string2AR(parseur, gmnString)",        this._checkRun());

        guidoStream = this.testExpect("guidoEngine.openStream()",           this._checkRun());
        this.testExpect("guidoEngine.writeStream(guidoStream, gmnStream)",  this._checkErrCode(GuidoErrCode.guidoNoErr));
        this.testExpect("guidoEngine.writeStream(guidoStream, 'a')",        this._checkErrCode(GuidoErrCode.guidoNoErr));
        this.testExpect("guidoEngine.stream2AR(parseur, guidoStream)",      this._checkRun());
        this.testExpect("guidoEngine.getStream(guidoStream)",               this._checkIsStr());

        this.testExpect("guidoEngine.parserGetErrorCode(parseur)",          this._checkIsParserErr());
        this.testExpect("guidoEngine.resetStream(guidoStream)",             this._checkErrCode(GuidoErrCode.guidoNoErr));   
        this.testExpect("guidoEngine.closeStream(guidoStream)",             this._checkErrCode(GuidoErrCode.guidoNoErr));
        this.testExpect("guidoEngine.closeParser(parseur)",                 this._checkErrCode(GuidoErrCode.guidoNoErr));
        console.log("\n");
    }    
    
    testEnd(): void {
        console.log("----End of guidoEngine test----");
        this.testExpect("guidoEngine.shutdown()",   this._checkVoid());
        console.log("\n");  
    }                      
    
    todo(): void {
        console.log("------------- TODO -------------"      + "\n" +
            "Missing declarations in main.cpp interface : guidoEngine"  + "\n" +
                    "=> guidoEngine.getParsingTime"             + "\n" +
                    "=> guidoEngine.getAR2GRTime"               + "\n" + 
                    "=> guidoEngine.getOnDrawTime"              + "\n" +
                    "=> guidoEngine.setSymbolPath"              + "\n" +
                    "=> guidoEngine.getSymbolPath"              + "\n" +
                    "=> guidoEngine.GuidoStream2GuidoString"    + "\n" +
                    "=> guidoEngine.GuidoFreeStreamString"      + "\n" +  
                    "=> guidoEngine.onDraw"                     + "\n" +  
            "Missing declarations in main.cpp interface : PianoRoll"    + "\n" +
                    "=> guidoPianoRoll.midi2PianoRoll"          + "\n" +
                    "=> guidoPianoRoll.getMap"                  + "\n" + 
                    "=> guidoPianoRoll.onDraw"                  + "\n" +
            "Others"    + "\n" +
                    "=> Module.GuidoScoreMapAdapter is undefined"   + "\n" +
                    "=> Some error to fix in guidoFactory"          + "\n" +
                    "=> More arguments in GuidoMap than expected"   + "\n"                                         
                    );     
    }
}    

        //this.testExpect("guidoEngine.onDraw(?)",                              this._checkErrCode(GuidoErrCode.guidoNoErr));              

        //this.testExpect("guidoPianoRoll.midi2PianoRoll(type, midiFileName)",            this._checkRun());        
        //this.testExpect("guidoPianoRoll.getMap(pr, width, height)",                     this._checkRun());        
        //this.testExpect("guidoPianoRoll.onDraw(pr, width, height, dev))",               this._checkErrCode(GuidoErrCode.guidoNoErr)); 