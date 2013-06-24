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
// MuseDataMusicalAttributesRecord.cpp: implementation of class CMuseDataMusicalAttributesRecord.
//
//////////////////////////////////////////////////////////////////////

#include "MuseDataMusicalAttributesRecord.h"
#include <string.h>

//#include "secureio.h"
#include "GuidoCreator.h"
#include "ErrorHandling.h"
#include <stdio.h>
#define snprintf _snprintf

//////////////////////////////////////////////////////////////////////
// construction/deconstruction
//////////////////////////////////////////////////////////////////////

CMuseDataMusicalAttributesRecord::CMuseDataMusicalAttributesRecord()
{
	rdType=drt_MusicalAttributes;
	
	rdClef[0]=-1;
	rdClef[1]=-1;
	rdDivisionsPerQuarter=-1;
	rdKey=-100;
	rdNumberOfInstruments=-1;
	rdNumberOfStaves=1;
	rdTimeDesignationDen=-1;
	rdTimeDesignationNum=-1;
	rdTransposingPart=-1;
}

CMuseDataMusicalAttributesRecord::~CMuseDataMusicalAttributesRecord()
{

}

int CMuseDataMusicalAttributesRecord::parseFromRecord(char * line)
{
	if(line[1]!=' '){
		MUSEERROR(err_EditorialLevelNotSupported,idLineNumber,2);
	}
	if(line[2]!=' '){
		MUSEERROR(err_FootnoteNotSupported,idLineNumber,3);
	}

	if(line[1]==0){return 0;}
	if(line[2]==0){return 0;}

	int col=3;
	while(line[col]!=0){
		int rv=0;
		switch(line[col]){
		//single-integer-attributes:
		case 'K':rv=parseAttribute(&line[col+1],&rdKey);break;
		case 'Q':rv=parseAttribute(&line[col+1],&rdDivisionsPerQuarter);break;
		case 'X':rv=parseAttribute(&line[col+1],&rdTransposingPart);break;
		case 'S':rv=parseAttribute(&line[col+1],&rdNumberOfStaves);break;
		case 'I':rv=parseAttribute(&line[col+1],&rdNumberOfInstruments);break;
		case 'T':rv=parseTimeDesignation(&line[col+1]);break;
		//clefs:
		case 'C':
			if(line[col+1]==':'){rv=parseAttribute(&line[col+1],&rdClef[0]);break;}
			if(line[col+1]=='1'){rv=parseAttribute(&line[col+2],&rdClef[0]);break;}
			if(line[col+1]=='2'){rv=parseAttribute(&line[col+2],&rdClef[1]);break;}
			rv=-1;break;
		//directives:
		case 'D':
			if(line[col+1]==':'){strncpy(rdDirective[0],&line[col+2],80);break;}
			if(line[col+1]=='1'){
				if(line[col+2]==':') {strncpy(rdDirective[0],&line[col+3],80);break;}
				else {rv=-1;break;}
			}
			if(line[col+1]=='2'){
				if(line[col+2]==':') {strncpy(rdDirective[1],&line[col+3],80);break;}
				else {rv=-1;break;}
			}
		}

		if(rv){
			MUSEERROR(err_ParseAttribute,idLineNumber,col);
		}

		//find next blank
		while( (line[col]!=0) && (line[col]!=' ') ){col++;}
		if(line[col]==' '){col++;}
	}
	return 0;
}

int CMuseDataMusicalAttributesRecord::parseAttribute(char *s, int *v)
{
	if(s[0]!=':') return -1;
	if(sscanf(&s[1],"%i",v)!=1){return -1;}
	return 0;
}

int CMuseDataMusicalAttributesRecord::printGuidoOn(FILE * f, CConversionParameters * p)
{
	//update conversion-parameters according to record!
	if(rdDivisionsPerQuarter!=-1){
		p->divisionsPerQuarter=rdDivisionsPerQuarter;
	}
	if(rdTimeDesignationDen!=-1){
		p->timeDesignationDen=rdTimeDesignationDen;
	}
	if(rdTimeDesignationNum!=-1){
		p->timeDesignationNum=rdTimeDesignationNum;
	}
	if(rdNumberOfStaves>1){
		p->numberOfStaves=rdNumberOfStaves;
	}
	if(rdClef[1]!=-1){
		if(p->numberOfStaves<2){
			p->numberOfStaves=2;
		}
	}

	//print stuff...
	//clefs
	if(rdClef[0]!=-1){
		guidoCreator.staff(1);
		guidoCreator.clef(rdClef[0]);
	}
	if(rdClef[1]!=-1){
		guidoCreator.staff(2);
		guidoCreator.clef(rdClef[1]);
	}
	//key
	if(rdKey!=-100){
		guidoCreator.staff(1);
		guidoCreator.key(rdKey);
		if(p->numberOfStaves>1){
			guidoCreator.staff(2);
			guidoCreator.key(rdKey);
		}
	}
	//meter
	if( (rdTimeDesignationNum!=-1) && (rdTimeDesignationDen>0) ){
		guidoCreator.staff(1);
		guidoCreator.meter(rdTimeDesignationNum,rdTimeDesignationDen);
		if( p->numberOfStaves>1 ){
			guidoCreator.staff(2);
			guidoCreator.meter(rdTimeDesignationNum,rdTimeDesignationDen);
		}
	}

	return 0;
}

int CMuseDataMusicalAttributesRecord::parseTimeDesignation(char *s)
{
	if(s[0]!=':'){return -1;}
	if(sscanf(&s[1],"%i/%i",&rdTimeDesignationNum,&rdTimeDesignationDen)!=2){return -1;}
	return 0;
}
