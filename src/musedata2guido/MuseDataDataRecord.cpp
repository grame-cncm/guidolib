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
// MuseDataDataRecord.cpp: implementation of class CMuseDataDataRecord.
//
//////////////////////////////////////////////////////////////////////

#include "MuseDataDataRecord.h"
#include "parseutils.h"
#include "ErrorHandling.h"

//////////////////////////////////////////////////////////////////////
// construction/deconstruction
//////////////////////////////////////////////////////////////////////

CMuseDataDataRecord::CMuseDataDataRecord()
{
	idTime=-1;
	idLineNumber=-1;
	rdType=drt_unknown;
	rdFootNoteFlag=0;
	rdLevelNumber=-1;
	rdTrackNumber=-1;
	referenceCounter=1;
}

CMuseDataDataRecord::~CMuseDataDataRecord()
{

}

EDataRecordType CMuseDataDataRecord::getDataRecordType()
{
	return rdType;
}

int CMuseDataDataRecord::getFootNoteFlag()
{
	return rdFootNoteFlag;
}

int CMuseDataDataRecord::getTrackNumber()
{
	return rdTrackNumber;
}

int CMuseDataDataRecord::getLevelNumber()
{
	return rdLevelNumber;
}

int CMuseDataDataRecord::parseFootnoteLevelTrack(char *line)
{
	//2.1.1 footnote
	if(line[12]!=' '){
		MUSEERROR(err_FootnoteNotSupported,idLineNumber,13);
	}
	//2.1.2 level
	if(isCharInString(line[13],"0123456789")){
		rdLevelNumber=line[13]-'0';
		MUSEERROR(err_EditorialLevelNotSupported,idLineNumber,14);
	} else {
		if(line[13]!=' '){
			MUSEERROR(err_EditorialLevelNotSupported,idLineNumber,14);
			//ERROR(err_ParseLevel,idLineNumber,14);
		}
	}
	//2.1.3 track
	if(isCharInString(line[14],"0123456789")){
		rdTrackNumber=line[14]-'0';
	} else {
		if(line[14]!=' '){
			MUSEERROR(err_ParseTrack,idLineNumber,15)
		}
	}
	//check for blank in col 16
	if(line[15]!=' '){
//TODO:ERRORHANDLING
	}
	return 0;
}

int CMuseDataDataRecord::parseFromRecord(char*line){
//TODO:ERRORHANDLING
	return 0;
}



void CMuseDataDataRecord::printOn(FILE * f)
{
	fprintf(f,"at time %i ",idTime);
	if(rdType==drt_unknown){
		fprintf(f,"an unknown record");
	}
}

void CMuseDataDataRecord::setidTime(int t)
{
	idTime=t;
}

void CMuseDataDataRecord::setLineNumber(int l)
{
	idLineNumber=l;
}

void CMuseDataDataRecord::release()
{
	referenceCounter--;
	if(referenceCounter==0){delete this;}
}

void CMuseDataDataRecord::reference()
{
	referenceCounter++;
}

int CMuseDataDataRecord::getidTime()
{
	return idTime;
}

int CMuseDataDataRecord::parsePrintSuggestionRecord(char *line)
{
//TODO:WARNING, not yet supported!
	return 0;
}

int CMuseDataDataRecord::parseAppendRecord(char *)
{
//TODO:WARNING, not yet supported!
	return 0;
}

int CMuseDataDataRecord::getidLineNumber()
{
	return idLineNumber;
}

int CMuseDataDataRecord::printGuidoOn(FILE *,CConversionParameters *)
{
	return 0;
}

void CMuseDataDataRecord::setTrackID(int id)
{
	rdTrackNumber=id;
}
