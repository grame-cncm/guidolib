///<reference path="../libGuidoAR.d.ts"/>
///<reference path="../libGuidoAR.ts"/>

interface checkfunction     { (n: any): boolean; } 

var guidoAR : GuidoAR;
var gmn     : string =  '[f g b/8 d f/4 e]';
var gmn2    : string =  '[a e f]';

var interval: number = 1;
var gmnSpec : string = '[a]';
var nvoices : number = 4;
var duration: rational = { num: 1, denom: 1 };
var n       : number = 1;
var mode    : TApplyMode = TApplyMode.kApplyOnce;
var pmode   : chordPitchMode = chordPitchMode.kUseLowest;
var mult    : number = 2;
var index   : number = 1;
var voice   : number = 1;


class guidoARTest {

    processTest(): void {
        guidoAR = require('../../javascript/libGuidoAR.js');   
        console.log('************************* New test *************************');
        console.log('**                GuidoAR Version ' + guidoAR.guidoarVersionString() + '                **');  
        console.log('************************************************************' + '\n');  
        
        this.processGuidoAR();
        
        console.log('************************* Test End *************************');             
    }



    _checkIsBool    ()         : checkfunction { return (m) => (typeof m == 'boolean'); }   
    _checkIsStr     ()         : checkfunction { return (m) => (typeof m == 'string'); }
    _checkIsNum     ()         : checkfunction { return (m) => (typeof m == 'number'); }    
    _checkIsRatio   ()         : checkfunction { return (m) => (typeof m.num == 'number' && typeof m.denom == 'number' ); }

    _checkGarOut    (e: number): checkfunction { return (m) => (e === m.err.value) && typeof m.str == 'string'; }

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


    processGuidoAR():void {
        this.testExpect("guidoAR.guidoarVersion()",                 this._checkIsNum());        
        this.testExpect("guidoAR.guidoarVersionString()",           this._checkIsStr());        
     
        this.testExpect("guidoAR.gmn2unrolled(gmn)",                this._checkGarOut(garErr.kNoErr)); 

        this.testExpect("guidoAR.gmnVTranpose(gmn, interval)",      this._checkGarOut(garErr.kNoErr));        
        this.testExpect("guidoAR.gmnGTranpose(gmn, gmnSpec)",       this._checkGarOut(garErr.kNoErr));  

        this.testExpect("guidoAR.gmnVTop(gmn, nvoices)",            this._checkGarOut(garErr.kNoErr));        
        this.testExpect("guidoAR.gmnGTop(gmn, gmnSpec)",            this._checkGarOut(garErr.kNoErr)); 

        this.testExpect("guidoAR.gmnVBottom(gmn, nvoices)",         this._checkGarOut(garErr.kNoErr));        
        this.testExpect("guidoAR.gmnGBottom(gmn, gmnSpec)",         this._checkGarOut(garErr.kNoErr));

        this.testExpect("guidoAR.gmnVHead(gmn, duration)",          this._checkGarOut(garErr.kNoErr));        
        this.testExpect("guidoAR.gmnGHead(gmn, gmnSpec)",           this._checkGarOut(garErr.kNoErr));

        this.testExpect("guidoAR.gmnVEHead(gmn, n)",                this._checkGarOut(garErr.kNoErr));        
        this.testExpect("guidoAR.gmnGEHead(gmn, gmnSpec)",          this._checkGarOut(garErr.kNoErr));

        this.testExpect("guidoAR.gmnVTail(gmn, duration)",          this._checkGarOut(garErr.kNoErr));        
        this.testExpect("guidoAR.gmnGTail(gmn, gmnSpec)",           this._checkGarOut(garErr.kNoErr));

        this.testExpect("guidoAR.gmnVETail(gmn, n)",                this._checkGarOut(garErr.kNoErr));        
        this.testExpect("guidoAR.gmnGETail(gmn, gmnSpec)",          this._checkGarOut(garErr.kNoErr));
        
        this.testExpect("guidoAR.gmnGSeq(gmn, gmn2)",               this._checkGarOut(garErr.kNoErr));        

        this.testExpect("guidoAR.gmnGPar(gmn, gmn2)",               this._checkGarOut(garErr.kNoErr));
        this.testExpect("guidoAR.gmnGRPar(gmn, gmn2)",              this._checkGarOut(garErr.kNoErr));

        this.testExpect("guidoAR.gmnGMirror(gmn, gmn2)",            this._checkGarOut(garErr.kNoErr));        

        this.testExpect("guidoAR.gmnGSetDuration(gmn, gmnSpec)",    this._checkGarOut(garErr.kNoErr));        

        this.testExpect("guidoAR.gmnApplyRythm(gmn, gmnSpec, mode)",        this._checkGarOut(garErr.kNoErr));
        this.testExpect("guidoAR.gmnApplyPitch(gmn, gmnSpec, mode, pmode)", this._checkGarOut(garErr.kNoErr));

        this.testExpect("guidoAR.gmnVSetDuration(gmn, duration)",           this._checkGarOut(garErr.kNoErr));
        this.testExpect("guidoAR.gmnVMultDuration(gmn, mult)",              this._checkGarOut(garErr.kNoErr));

        this.testExpect("guidoAR.gmncheck(gmn)",                            this._checkIsBool());        
        this.testExpect("guidoAR.gmnDuration(gmn)",                         this._checkIsRatio());        
        this.testExpect("guidoAR.gmnEv2Time(gmn, index, voice)",            this._checkIsRatio());        

        console.log('\n');
    }     
}