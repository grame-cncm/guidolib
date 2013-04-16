/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
// MuseDataNoteRecord.cpp: implementation of class CMuseDataNoteRecord.
//
//////////////////////////////////////////////////////////////////////

#include "MuseDataNoteRecord.h"
#include "parseutils.h"

#include "ErrorHandling.h"
#include "GuidoCreator.h"
#include "ConversionOptions.h"
#include "ParseParameters.h"

//////////////////////////////////////////////////////////////////////
// construction/deconstruction
//////////////////////////////////////////////////////////////////////

CMuseDataNoteRecord::CMuseDataNoteRecord()
{
	//set record-type
	rdType=drt_NoteRecord;
	//initialize fields for sound-information represented in record
	rdPitchName=p_a;//just a value, MUST be in record
	rdPitchChromatic=0;
	rdPitchOctave=0;
	rdDuration=0;
	rdNoteType=nt_normal; //MUST be in record!
	rdGraceCueType=gct_8th_with_slash;
	rdFlagTied=0;
	rdFlagChord=0;

	//initialize graphical and interpretativ information
	rdGraphicNoteType=gnt_unknown;
	rdDotsOfProlongation=0;
	rdActualNotatedAccidentals=na_none;
	rdStemDirection=sd_nostem;
	rdStaffAssignment=0;
	int i;
	for(i=0;i<6;i++){rdBeamcode[i]=bc_nobeam;}
	rdTie=0;
	rdBeginTuplet=0;
	rdEndTuplet=0;
	for(i=0;i<4;i++){
		rdBeginSlur[i]=0;
		rdEndSlur[i]=0;
	}
	for(i=0;i<orn_Count;i++){rdOrnaments[i]=0;}


	//initialize implicit information
	idBeamCount=0;
	idBeamChange=0;
	idBeamDrawInfo=0;
	idChord=range_NoPart;
	for(i=0;i<4;i++){idSlurID[i]=0;}
	idTieBegin=0;
	idTieEnd=0;
	idCueNote=range_NoPart;
	idGraceNote=range_NoPart;
	idNextInChord=NULL;
	idChordDuration=0;
	idChord=range_NoPart;
}

CMuseDataNoteRecord::~CMuseDataNoteRecord()
{

}



int CMuseDataNoteRecord::parseFromRecord(char *line)
{
	//information retrievalble from  col 13-43

	//1.Parse the sound-information from colums 1 to 9
	//1.1. normal notes or chords
	if(isCharInString(line[0],"ABCDEFGr ")){
		//set note-type
		rdNoteType=nt_normal;
		//chord or not??
		if(line[0]==' '){rdFlagChord=1;} else {rdFlagChord=0;}
		//parse pitch
		if(parsePitchInformation(&line[rdFlagChord],&rdPitchName,&rdPitchChromatic,&rdPitchOctave)){
			MUSEERROR(err_ParsePitch,idLineNumber,rdFlagChord);
		}
		//parse duration
		if(parseDurationInformation(&line[5],&rdDuration)){
			if( (rdFlagChord) && (parseParameters.lastNoteDuration!=-1) && (isBlanks(line+5,3)) ){
				rdDuration=parseParameters.lastNoteDuration;
			} else {
				MUSEERROR(err_ParseDuration,idLineNumber,6);
			}
		}
		parseParameters.lastNoteDuration=rdDuration;
		//parse tie-flag
		if(!isCharInString(line[8],"- ")){
			MUSEERROR(err_ParseTieFlag,idLineNumber,9)
		}
		if(line[8]=='-'){rdFlagTied=1;}
	} 

	//1.2 cue- or gracenotes
	else {
		//cue or grace?
		if(line[0]=='c'){rdNoteType=nt_cue;} else {rdNoteType=nt_grace;}
		//in chord?
		if(line[1]==' '){rdFlagChord=1;}
		//parse pitch
		if(parsePitchInformation(&line[1+rdFlagChord],&rdPitchName,&rdPitchChromatic,&rdPitchOctave)){
			MUSEERROR(err_ParsePitch,idLineNumber,1+rdFlagChord)
		}
		//check for blank(s)
		for(int i=(5+rdFlagChord);i<7;i++){
			if(line[i]!=' '){
				MUSEERROR(err_ParseError,idLineNumber,i+1)
			}
		}
		//parse notetype
		if(isCharInString(line[7],"0123456789A")){
			rdGraceCueType=EGraceCueType(line[7]);
		} else {
			MUSEERROR(err_ParseNoteType,idLineNumber,8)
		}
		//check for final blank
		if(line[8]!=' '){
			MUSEERROR(err_ParseError,idLineNumber,9)
		}
		//one thing is shure: cue and gracenotes don't have a duration
		rdDuration=0;
	}
	//***********************END OF SOUND INFORMATION ****************************

	//2.: Parse graphical and interpretative information from col 10-80

	//check for blanks in col 10-12
	if(!isBlanks(&line[9],3)){
		MUSEERROR(err_ParseError,idLineNumber,10)
	}

	//2.1. footnote, level and track (done in superclass)
	int rv=parseFootnoteLevelTrack(line);
	if(rv){
//TODO:ERRORHANDLING
	}

	//2.2 graphical representation col 17-19
	//2.2.1 graphic note type
	if(!isCharInString(line[16]," LbwhqestxyzBA987654321")){
		MUSEERROR(err_ParseNoteType,idLineNumber,17)
	} else {
		rdGraphicNoteType=EGraphicNoteType(line[16]);
	}
	//2.2.2 dots of prolongation
	switch(line[17]){
	case' ':rdDotsOfProlongation=0;break;
	case'.':rdDotsOfProlongation=1;break;
	case':':rdDotsOfProlongation=2;break;
	default:
		MUSEERROR(err_ParseDots,idLineNumber,18);
		rdDotsOfProlongation=0;
		break;
	}
	//2.2.3 actual notated accidentals
	if(!isCharInString(line[18]," #nfxX&SF")){
		MUSEERROR(err_ParseNotatedAccidentals,idLineNumber,19);
		rdActualNotatedAccidentals=ENotatedAccidental(' ');
	} else {
		rdActualNotatedAccidentals=ENotatedAccidental(line[18]);
	}
	//2.2.4
//TODO:TIMEMODIFICATION
	//2.2.5 stem direction (col 23)
	if(!isCharInString(line[22],"du ")){
		MUSEERROR(err_ParseStemDirection,idLineNumber,24);
		rdStemDirection=EStemDirection(' ');
	} else {
		rdStemDirection=EStemDirection(line[22]);
	}
	//2.2.6 staff assignment (col 24)
	switch(line[23]){
	case' ':rdStaffAssignment=0;break;
	case'1':rdStaffAssignment=1;break;
	case'2':rdStaffAssignment=2;break;
	default:
		MUSEERROR(err_ParseStaffAssignment,idLineNumber,25)
		rdStaffAssignment=0;
	}
	//check for blank in col 25
	if(line[24]!=' '){
		MUSEERROR(err_ParseError,idLineNumber,26)
	}
	//2.2.7 beamcodes col 26-31
	//parsing line
	int i;
	for(i=25;i<31;i++){
		if(!isCharInString(line[i]," [=]/\\")){
			MUSEERROR(err_ParseBeamCode,idLineNumber,i+1)
			rdBeamcode[i-25]=EBeamCode(' ');
		}else{
			rdBeamcode[i-25]=EBeamCode(line[i]);
		}
	}
	//checking beam-level
	idBeamCount=0;
	idBeamChange=0;
	if( (conversionOptions.beamMode!=co_beams_AutoBeam) && (conversionOptions.beamMode!=co_beams_NoBeams) ){
		int depthCount=0;
		for(i=0;i<6;i++){
			if(isCharInString(rdBeamcode[i],"[=]")){
				if(depthCount==i){
					depthCount++;
					if(rdBeamcode[i]=='['){
						if(idBeamChange<0){ 
							MUSEERROR(err_SyntaxBeam,idLineNumber,26+i);
						} else {
							idBeamChange++;
							idBeamCount++;
						}
					}
					if(rdBeamcode[i]==']'){
						if(idBeamChange>0){ 
							MUSEERROR(err_SyntaxBeam,idLineNumber,26+i);
						} else {
							idBeamChange--;
						}
					}
					if(rdBeamcode[i]=='='){
						idBeamCount++;
						if(idBeamChange!=0){ 
							MUSEERROR(err_SyntaxBeam,idLineNumber,26+i);
						}
					}
				} else {
					MUSEERROR(err_SyntaxBeam,idLineNumber,26+i);
				}
			}
		}//for
	}//if

	//2.2.8 additional notations
	for(i=31;i<43;i++){
		switch(line[i]){
		//editorial level
		case '&':
			MUSEERROR(err_EditorialLevelNotSupported,idLineNumber,i);
			i++;
			break;
		//ties:
		case '-': rdTie=1;break;
		//slurs
		case '(': rdBeginSlur[0]=1; break;
		case ')': rdEndSlur[0]=1; break;
		case '[': rdBeginSlur[1]=1; break;
		case ']': rdEndSlur[1]=1; break;
		case '{': rdBeginSlur[2]=1; break;
		case '}': rdEndSlur[2]=1; break;
		case 'z': rdBeginSlur[3]=1; break;
		case 'x': rdEndSlur[3]=1; break;
		//tuplets
		case '*': rdBeginTuplet=1;break;
		case '!': rdEndTuplet=1;break;
		//ornaments
		case 't': rdOrnaments[orn_tr]=1;break;
		case 'r': rdOrnaments[orn_turn]=1;break;
		case 'k': rdOrnaments[orn_delayed_turn]=1;break;
		case 'w': rdOrnaments[orn_shake]=1;break;
		case '~': rdOrnaments[orn_wavy_line]=1;break;
		case 'c': rdOrnaments[orn_continue_wavy_line]=1;break;
		case 'M': rdOrnaments[orn_mordent]=1;break;
		case 'j': rdOrnaments[orn_slide]=1;break;
			
		}
	}

	return 0;
}

void CMuseDataNoteRecord::printOn(FILE *f)
{
	CMuseDataDataRecord::printOn(f);
	switch(rdNoteType){
	case nt_normal: fprintf(f,"normal ");break;
	case nt_cue:fprintf(f,"cue ");break;
	case nt_grace:fprintf(f,"grace ");break;
	}

	if (rdPitchName==p_rest){
		fprintf(f,"rest, duration %i",rdDuration);
	}else{
		fprintf(f,"note, pitch %c,chromatic %i, octave %i, duration %i",rdPitchName,rdPitchChromatic,rdPitchOctave,rdDuration);
	};

	switch(idChord){
	case ci_Begin:fprintf(f,",begin of chord");break;
	case ci_Part:fprintf(f,",part of chord");break;
	case ci_End:fprintf(f,",end of chord");break;
	}

	if(idBeamCount!=0){
		fprintf(f,",%i beams",idBeamCount);
		if(idBeamChange<0){ fprintf(f," (%i end)",-idBeamChange);}
		if(idBeamChange>0){ fprintf(f," (%i new)",idBeamChange);}
	}
	
	int slur=0;
	for(int i=0;i<4;i++){
		if(rdBeginSlur[i]){slur=1;}
		if(rdEndSlur[i]){slur=1;}
	}
	if(slur){
		fprintf(f,"slurs: ");
		for(int i=0;i<4;i++){
			if(rdBeginSlur[i]){fprintf(f,"+%i (id:%i)",i,idSlurID[i]);}
			if(rdEndSlur[i]){fprintf(f,"-%i (id:%i)",i,idSlurID[i]);}
		}
	}

	if(idTieBegin){fprintf(f,"begin tie %i",idTieBegin);}
	if(idTieEnd){fprintf(f,"end tie %i",idTieEnd);}
}
			

int CMuseDataNoteRecord::getrdDuration()
{
	return rdDuration;
}

void CMuseDataNoteRecord::setidChordInfo(ERange c)
{
	if((c==range_Begin) && (idChord==range_End) ){idChord=range_Single;return;}
	if((c==range_End) && (idChord==range_Begin) ){idChord=range_Single;return;}
	idChord=c;
}

int CMuseDataNoteRecord::getrdChordFlag()
{
	return rdFlagChord;
}

int CMuseDataNoteRecord::getrdBeginSlur(int i)
{
	return rdBeginSlur[i];
}

int CMuseDataNoteRecord::getrdEndSlur(int i)
{
	return rdEndSlur[i];
}

int CMuseDataNoteRecord::getrdPitchCode()
{
	int rv;
	if(rdPitchName==p_rest){return 0;}
	rv=rdPitchName-'A'+1;
	rv+=8*rdPitchOctave;
	rv+=80*(rdPitchChromatic+2);
	return rv;
}

int CMuseDataNoteRecord::isTiedToNext()
{
	return (rdTie||rdFlagTied);
}

void CMuseDataNoteRecord::setidTieBegin(int i)
{
	idTieBegin=i;
}

void CMuseDataNoteRecord::setidTieEnd(int i)
{
	idTieEnd=i;
}

void CMuseDataNoteRecord::setidSlurID(int no, int id)
{
	idSlurID[no]=id;
}

int CMuseDataNoteRecord::printGuidoOn(FILE * f,CConversionParameters* c)
{
	int i;
	//1. create opening tags, if neccessary

	//order of chord, tie and slur tags changed, MF 25-09-01
	//1.3 chord
	if( (idChord==range_Begin) || (idChord==range_Single) ) {
		//if(idChordLabel==-1){fprintf(f,"{ "); }
		//else{fprintf(f,"\\chord <\"label%i\">( { ",idChordLabel);}
		guidoCreator.chordBegin(idChordLabel);
	}
	//1.1 slurs
	for(i=0;i<4;i++){
		if( (rdBeginSlur[i]) && (idSlurID[i]) ){ guidoCreator.slurBegin(idSlurID[i]); }
	}
	//1.2 tie
	//if(idTieBegin){	fprintf(f,"\\tieBegin:%i ",idTieBegin); }
	if(idTieBegin){guidoCreator.tieBegin(idTieBegin);}


	//1.4 grace notes
	if( (idGraceNote==range_Begin) || (idGraceNote==range_Single) ) {
		//fprintf(f,"\\graceBegin ");
		//fprintf(f,"\\grace( ");
		guidoCreator.graceBegin();
	}

	//1.5 cue notes
	if( (idCueNote==range_Begin) || (idCueNote==range_Single) ){
		//fprintf(f,"\\cueBegin ");
		guidoCreator.cueBegin();
	}

	//1.6 beams
	if(idBeamDrawInfo>0){
		guidoCreator.beamBegin(idBeamDrawInfo);
	}
	//2. create note-tags
	//2.0 comma, if we are in a chord
	//MOVED TO GUDIOCREATOR


	//2.05 staff asignment
	switch(rdStaffAssignment){
	case 0: case 1: 
		guidoCreator.staff(1);break;
	case 2: 
		guidoCreator.staff(2);break;
	}

	//2.1 rest or normal note??
	char pitch;
	if(rdPitchName==p_rest){
		//2.1.1 a rest:
		pitch='_';
	} else {
		//2.1.2 a note:
		//2.1.2.0 stem
		switch(rdStemDirection){
		case sd_down: 
			guidoCreator.stemsDown();break;
		case sd_up: 
			guidoCreator.stemsUp();break;
		case sd_nostem: 
			guidoCreator.stemsOff();
		}
		//2.1.2.1 pitch-name
		switch(rdPitchName){
		case p_a:pitch='a';break;
		case p_b:pitch='b';break;
		case p_c:pitch='c';break;
		case p_d:pitch='d';break;
		case p_e:pitch='e';break;
		case p_f:pitch='f';break;
		case p_g:pitch='g';break;
		}
		//2.1.2.2 accidentals:
		//2.1.2.3 octave:
	}
	//2.2 duration
	int denom;
	int num;
	if(rdGraphicNoteType!=gnt_unknown){
		//2.2.1. explicit given print-types for notes
		//2.2.1.1 print notes
		switch(rdGraphicNoteType){
		case gnt_fs_long:case gnt_cs_long:
			num=2;denom=1;break;
//TODO:BREVIS
//TODO:WHOLE!
		case gnt_fs_whole:case gnt_cs_whole:
			num=1;denom=1;break;
		case gnt_fs_half:case gnt_cs_half:
			num=1;denom=2;break;
		case gnt_fs_quarter:case gnt_cs_quarter:
			num=1;denom=4;break;
		case gnt_fs_eighth:case gnt_cs_eighth:
			num=1;denom=8;break;
		case gnt_fs_sixteenth:case gnt_cs_sixteenth:
			num=1;denom=16;break;
		case gnt_fs_32nd:case gnt_cs_32nd:
			num=1;denom=32;break;
		case gnt_fs_64th:case gnt_cs_64th:
			num=1;denom=64;break;
		case gnt_fs_128th:case gnt_cs_128th:
			num=1;denom=128;break;
		case gnt_fs_256th:case gnt_cs_256th:
			num=1;denom=256;break;
		default:
	//TODO:calculate note-length from duration
			break;
		}
	} else {
		//we have to calculate the note-type from the duration
		//fprintf(f,"%i/%i",rdDuration,h->getrdDivisionsPerQuarter());
		num=rdDuration;denom=c->divisionsPerQuarter*4;
	}
	//2.2.1.2 print dots


	//new, improved creation of note-events..
	//check,if divisions per Quarter is set
	if(c->divisionsPerQuarter<=0){
		MUSEERROR(err_NoDivisionPerQuarterInformation,getidLineNumber(),0);
	}
	guidoCreator.note(pitch,rdPitchChromatic,rdPitchOctave-3,rdDuration,c->divisionsPerQuarter*4,num,denom,rdDotsOfProlongation);

	//3. create closing tags:
	//n-5 beams
	if(idBeamDrawInfo<0){
		guidoCreator.beamEnd(-idBeamDrawInfo);
	}
	
	//1.n-4 cue notes
	if( (idCueNote==range_End) || (idCueNote==range_Single) ){
		//fprintf(f,"\\cueEnd ");
		guidoCreator.cueEnd();
	}

	//3.n-3 grace notes
	if( (idGraceNote==range_End) || (idGraceNote==range_Single) ) {
		//fprintf(f,"\\graceEnd ");
		//fprintf(f,") ");
		guidoCreator.graceEnd();
	}

	//3.n-1 tie
	if(idTieEnd){guidoCreator.tieEnd(idTieEnd);}

	//3.n slurs
	for(i=0;i<4;i++){
		if( (rdEndSlur[i]) && (idSlurID[i]) ){ guidoCreator.slurEnd(idSlurID[i]); }
	}

	//3.n-2 chord
	if( (idChord==range_End) || (idChord==range_Single) ) {
		guidoCreator.chordEnd(idChordLabel);
	}
	
	return 0;
}

void CMuseDataNoteRecord::setidCueNote(ERange v)
{
	if((v==range_End) && (idCueNote==range_Begin)){idCueNote=range_Single;return;}
	if((v==range_Begin) && (idCueNote==range_End)){idCueNote=range_Single;return;}
	idCueNote=v;
}

void CMuseDataNoteRecord::setidGraceNote(ERange v)
{
	if((v==range_End) && (idGraceNote==range_Begin)){idGraceNote=range_Single;return;}
	if((v==range_Begin) && (idGraceNote==range_End)){idGraceNote=range_Single;return;}
	idGraceNote=v;
}

ENoteType CMuseDataNoteRecord::getrdNoteType()
{
	return rdNoteType;
}

void CMuseDataNoteRecord::setidNextInChord(CMuseDataNoteRecord * r)
{
	idNextInChord=r;
}

CMuseDataNoteRecord* CMuseDataNoteRecord::getidNextInChord()
{
	return idNextInChord;
}

ERange CMuseDataNoteRecord::getidChordInfo()
{
	return idChord;
}

void CMuseDataNoteRecord::setidChordDuration(int d)
{
	idChordDuration=d;
}

int CMuseDataNoteRecord::getidChordDuration()
{
	return idChordDuration;
}

void CMuseDataNoteRecord::setidChordLabel(int l)
{
	idChordLabel=l;
}

int CMuseDataNoteRecord::getidBeamCount()
{
	return idBeamCount;
}

int CMuseDataNoteRecord::getidBeamChange()
{
	return idBeamChange;
}

void CMuseDataNoteRecord::setidBeamDrawInfo(int d)
{
	idBeamDrawInfo=d;
}

int CMuseDataNoteRecord::getidBeamDrawwInfo()
{
	return idBeamDrawInfo;
}
