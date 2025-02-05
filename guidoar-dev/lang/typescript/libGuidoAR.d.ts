declare enum garErr  {}
declare enum TApplyMode  {}
declare enum chordPitchMode  {}

interface garOut {
    err: garErr;
    str: string;    
}

interface rational {
    num     : number;
    denom   : number;
}

interface GuidoAR {
    guidoarVersion(): any;
    guidoarVersionString(): string;

    gmn2unrolled(gmn: string): garOut;

    gmnVTranpose(gmn: string, interval: number) : garOut;
    gmnGTranpose(gmn: string, gmnSpec: string)  : garOut;

    gmnVTop(gmn: string, nvoices: number): garOut;
    gmnGTop(gmn: string, gmnSpec: string): garOut;

    gmnVBottom(gmn: string, nvoices: number): garOut;
    gmnGBottom(gmn: string, gmnSpec: string): garOut;

    gmnVHead(gmn: string, duration: rational): garOut;
    gmnGHead(gmn: string, gmnSpec: string): garOut;

    gmnVEHead(gmn: string, n: number): garOut;
    gmnGEHead(gmn: string, gmnSpec: string): garOut;

    gmnVTail(gmn: string, duration: rational): garOut;
    gmnGTail(gmn: string, gmnSpec: string): garOut;

    gmnVETail(gmn: string, n: number): garOut;
    gmnGETail(gmn: string, gmnSpec: string): garOut;

    gmnGSeq(gmn1: string, gmn2: string): garOut;

    gmnGPar(gmn1: string, gmn2: string) : garOut;
    gmnGRPar(gmn1: string, gmn2: string): garOut;

    gmnGMirror(gmn1: string, gmn2: string): garOut;

    gmnGSetDuration(gmn: string, gmnSpec: string): garOut;

    gmnApplyRythm(gmn: string, gmnSpec: string, mode: TApplyMode): garOut;
    gmnApplyPitch(gmn: string, gmnSpec: string, mode: TApplyMode, pmode: chordPitchMode): garOut;

    gmnVSetDuration(gmn: string, duration: rational): garOut;
    gmnVMultDuration(gmn: string, mult: number): garOut;

    gmncheck(gmn: string)   : boolean;
    gmnDuration(gmn: string): rational;
    gmnEv2Time(gmn: string, index: number, voice: number): rational;
}