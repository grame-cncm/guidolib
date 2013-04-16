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
// GuidoCreator.cpp: implementation of class CGuidoCreator.
//
//////////////////////////////////////////////////////////////////////

#include "GuidoCreator.h"
#include "ConversionOptions.h"

//global GuidoCreator object:
CGuidoCreator guidoCreator;

//////////////////////////////////////////////////////////////////////
// construction/deconstruction
//////////////////////////////////////////////////////////////////////

CGuidoCreator::CGuidoCreator()
{

}

CGuidoCreator::~CGuidoCreator()
{

}

void CGuidoCreator::setFile(FILE *f)
{
	file=f;
}

void CGuidoCreator::init()
{
}

void CGuidoCreator::staff(int i)
{
	if(i==lastStaff){return;}
	fprintf(file,"\\staff<%i> ",i);
	lastStaff=i;
}

void CGuidoCreator::stemsUp()
{
	if(lastStem==0){return;}
	fprintf(file,"\\stemsUp ");
	lastStem=0;
}

void CGuidoCreator::stemsDown()
{
	if(lastStem==1){return;}
	fprintf(file,"\\stemsDown ");
	lastStem=1;
}

void CGuidoCreator::stemsOff()
{
	if(lastStem==2){return;}
	fprintf(file,"\\stemsOff ");
	lastStem=2;
}


void CGuidoCreator::note(char pitch, int chromatic, int octave, int numerator, int denominator, int dots)
{
	clefS();
	keyS();
	meterS();
	if(inChord){
		notesInChord++;
		if(notesInChord>1){
			fprintf(file,",");
		}
	}

	if(pitch=='_'){
		fprintf(file,"_");
	} else {
		fprintf(file,"%c",pitch);
		switch(chromatic){
		case -2:fprintf(file,"&&");break;
		case -1:fprintf(file,"&"); break;
		case  1:fprintf(file,"#"); break;
		case  2:fprintf(file,"##");break;
		}
		if(lastNoteOctave!=octave){
			fprintf(file,"%i",octave);
			lastNoteOctave=octave;
		}
	}
	if(  (numerator!=lastNoteNumerator) || (denominator!=lastNoteDenominator) || (dots!=lastNoteDots) ){
		fprintf(file,"*%i/%i",numerator,denominator);
		lastNoteDenominator=denominator;
		lastNoteNumerator=numerator;
		lastNoteDots=dots;
	}
	for(int i=0;i<dots;i++){fprintf(file,".");}
	fprintf(file," ");
}//note

void CGuidoCreator::note(char pitch, int chromatic, int octave, int durNumerator, int durDenominator, int dispNumerator, int dispDenominator, int dispDots)
{
	clefS();
	keyS();
	meterS();
	//calculate durations
	float realDuration= (float(durNumerator))/(float(durDenominator));
	float dispDuration= (float(dispNumerator))/(float(dispDenominator));
	if(dispDots==1){dispDuration*=1.5;}
	if(dispDots==2){dispDuration*=1.75;}

	//now we can decide, how the note tag is created
	if(realDuration==dispDuration){
		//the printed duration is the real duration:
		note(pitch,chromatic,octave,dispNumerator,dispDenominator,dispDots);
	} else {
		//the printed duration differs from the real duratioon
		fprintf(file,"\\dispDur<%i,%i,%i>( ",dispNumerator,dispDenominator,dispDots);
		note(pitch,chromatic,octave,durNumerator,durDenominator,0);
		fprintf(file,") ");
	}
}//note

void CGuidoCreator::empty(int numerator, int denominator,int dots)
{
	clefS();
	keyS();
	meterS();
	fprintf(file,"empty");
	if(  (numerator!=lastNoteNumerator) || (denominator!=lastNoteDenominator) || (dots!=lastNoteDots) ){
		fprintf(file,"*%i/%i",numerator,denominator);
		lastNoteDenominator=denominator;
		lastNoteNumerator=numerator;
		lastNoteDots=dots;
	}
	for(int i=0;i<dots;i++){fprintf(file,".");}
	fprintf(file," ");
}


void CGuidoCreator::clef(int clefCode)
{
	if(lastStaff==1){clef1=clefCode;}
	if(lastStaff==2){clef2=clefCode;}
	
	if(!conversionOptions.smartClefsKeysMeters){
		fprintf(file,"\\clef<");
		//print clef-type as in MuseData-spec
		switch((clefCode/10)%3){
		case 0:fprintf(file,"\"g");break;
		case 1:fprintf(file,"\"c");break;
		case 2:fprintf(file,"\"f");break;
		}
		
		//print staffline
		//BEWARE: MuseData counts stafflines from the top,
		//Guido from the bottom!
		fprintf(file,"%i",6-(clefCode%10));

		//print transpositions
		switch(clefCode/30){
		case 0:fprintf(file,"\"> ");break;
		case 1:fprintf(file,"-8\"> ");break;
		case 2:fprintf(file,"+8\"> ");break;
		}
	}
}

void CGuidoCreator::clefS()
{
	if(conversionOptions.smartClefsKeysMeters){
		int clefCode=-100;
		if(lastStaff==1){clefCode=clef1;}
		if(lastStaff==2){clefCode=clef2;}
		if(clefCode==-100){return;}

		fprintf(file,"\\clef<");
		//print clef-type as in MuseData-spec
		switch((clefCode/10)%3){
		case 0:fprintf(file,"\"g");break;
		case 1:fprintf(file,"\"c");break;
		case 2:fprintf(file,"\"f");break;
		}
		
		//print staffline
		//BEWARE: MuseData counts stafflines from the top,
		//Guido from the bottom!
		fprintf(file,"%i",6-(clefCode%10));

		//print transpositions
		switch(clefCode/30){
		case 0:fprintf(file,"\"> ");break;
		case 1:fprintf(file,"-8\"> ");break;
		case 2:fprintf(file,"+8\"> ");break;
		}
		clef1=-100;
		clef2=-100;
	};
}


void CGuidoCreator::meter(int num, int den)
{
	meterNum=num;
	meterDen=den;
	if(!conversionOptions.smartClefsKeysMeters){
		fprintf(file,"\\meter<\"%i/%i\"> ",num,den);
	}
}

void CGuidoCreator::meterS()
{
	if(conversionOptions.smartClefsKeysMeters){
		if( (meterNum!=-100) && (meterDen!=-100) ){
			fprintf(file,"\\meter<\"%i/%i\"> ",meterNum,meterDen);
		}
		meterNum=-100;
		meterDen=-100;
	}
}

void CGuidoCreator::key(int i)
{
	key_=i;
	if(!conversionOptions.smartClefsKeysMeters){
		fprintf(file,"\\key<%i> ",i);
	}
}

void CGuidoCreator::keyS()
{
	if(conversionOptions.smartClefsKeysMeters){
		if(key_!=-100){
			fprintf(file,"\\key<%i> ",key_);
		}
		key_=-100;
	}
}

void CGuidoCreator::tieBegin(int id)
{
	fprintf(file,"\\tieBegin:%i ",id);	
}

void CGuidoCreator::tieEnd(int id)
{
	fprintf(file,"\\tieEnd:%i ",id);	
}

void CGuidoCreator::slurBegin(int id)
{
	fprintf(file,"\\slurBegin:%i ",id);		
}

void CGuidoCreator::slurEnd(int id)
{
	fprintf(file,"\\slurEnd:%i ",id);	
}

void CGuidoCreator::chordBegin(int label)
{
	inChord=1;
	notesInChord=0;
	if(label==-1){fprintf(file,"{ "); }
	else{fprintf(file,"\\splitChord <\"label%i\">( { ",label);}

}

void CGuidoCreator::chordEnd(int label)
{
	inChord=0;
	if(label==-1) {fprintf(file,"} "); }
	else {fprintf(file,"} )" );}
}

void CGuidoCreator::cueBegin()
{
	fprintf(file,"\\cueBegin ");
}

void CGuidoCreator::cueEnd()
{
	fprintf(file,"\\cueEnd ");
}

void CGuidoCreator::graceBegin()
{
	fprintf(file,"\\grace( ");
}

void CGuidoCreator::graceEnd()
{
	fprintf(file,") ");
}

void CGuidoCreator::beamBegin(int b)
{
	int i;
	switch(conversionOptions.beamMode){
	case co_beams_SingleTag:
		if(beamsOpen==0){
			fprintf(file,"\\beamBegin ");
		}
		beamsOpen+=b;
		break;
	case co_beams_MultiTag:
		for(i=0;i<b;i++){
			fprintf(file,"\\beamBegin ");
		}
		break;
	case co_beams_AutoBeam:
	case co_beams_NoBeams:
		break;
	}
}

void CGuidoCreator::beamEnd(int b)
{
	int i;
	switch(conversionOptions.beamMode){
	case co_beams_SingleTag:
		beamsOpen-=b;
		if(beamsOpen==0){
			fprintf(file,"\\beamEnd ");
		}
		break;
	case co_beams_MultiTag:
		for(i=0;i<b;i++){
			fprintf(file,"\\beamEnd ");
		}
		break;
	case co_beams_AutoBeam:
	case co_beams_NoBeams:
		break;
	}
}



void CGuidoCreator::sequenceBegin(int track, int subtrack)
{
	//initialize internal structures for Guido-creation
	lastStaff=-1;
	lastStem=-1;
	lastNoteNumerator=-1;
	lastNoteDenominator=-1;
	lastNoteDots=-1;
	lastNoteOctave=-100;
	beamsOpen=0;
	inChord=0;
	
	
	sequenceCount++;


	clef1=-100;
	clef2=-100;
	key_=-100;
	meterDen=-100;
	meterNum=-100;

	//create Guido
	//comma, if it's not the first sequence
	if(sequenceCount>1){fprintf(file,",");}
	//initial linefeed before new sequence...
	fprintf(file,"\n");
	//A nice comment about the next sequence...
	if(conversionOptions.printTrackInfo){
		fprintf(file,"\n%%**************************************************\n");
		if(track==-1){
			fprintf(file,"%% subtrack %i from unmarked track",subtrack);
		} else {
			fprintf(file,"%% subtrack %i from track %i",subtrack,track);
		}
		fprintf(file,"\n%%**************************************************");
	}
	//begin of sequence:
	fprintf(file,"\n[ ");
	
	//initial quido-tags:
	//1. Beam-mode
	switch(conversionOptions.beamMode){
	case co_beams_SingleTag:
	case co_beams_MultiTag:
	case co_beams_NoBeams:
		staff(1);
		fprintf(file,"\\beamsOff ");
		break;
	case co_beams_AutoBeam:
		staff(1);
		fprintf(file,"\\beamsAuto ");
		break;
	}
}


void CGuidoCreator::sequenceEnd()
{
	fprintf(file,"]");
}


void CGuidoCreator::segmentBegin()
{
	sequenceCount=0;
	fprintf(file,"{");
}

void CGuidoCreator::segmentEnd()
{
	fprintf(file,"\n}\n");
}	

void CGuidoCreator::measureLine()
{
	fprintf(file,"|");
	if(conversionOptions.printLineFeedAfterBarline){
		fprintf(file,"\n");
	}
}



