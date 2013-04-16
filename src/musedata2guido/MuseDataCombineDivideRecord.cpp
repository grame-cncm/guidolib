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
// MuseDateCombineDivideRecord.cpp: implementation of class CMuseDateCombineDivideRecord.
//
//////////////////////////////////////////////////////////////////////

#include "MuseDataCombineDivideRecord.h"
#include <string.h>
#include "parseutils.h"
#include "ErrorHandling.h"

//////////////////////////////////////////////////////////////////////
// construction/deconstruction
//////////////////////////////////////////////////////////////////////

CMuseDataCombineDivideRecord::CMuseDataCombineDivideRecord()
{
	rdType=drt_CombineDivideRecord;
}

CMuseDataCombineDivideRecord::~CMuseDataCombineDivideRecord()
{

}

int CMuseDataCombineDivideRecord::parseFromRecord(char *line)
{
	//check type (col 1 to 5)
	if(strncmp(line,"back ",5)==0){
		rdCombineDivideType=cdt_back;
	} else {
		if(strncmp(line,"irest",5)==0){
			rdCombineDivideType=cdt_irest;
		} else {
			MUSEERROR(err_UnknownRecordType,idLineNumber,0);
		}
	}

	//parse skip-value (col 6 to 8)
	if(parseDurationInformation(&line[5],&rdSkip)){
		MUSEERROR(err_ParseDuration,idLineNumber,6);
	}

	//blanks in col 9 to 12
	if(!isBlanks(&line[8],4)){
		MUSEERROR(err_ParseError,idLineNumber,9);
	}

	//parse footnote, level and track in col 13 ro 16
	int rv=parseFootnoteLevelTrack(line);
	if(rv){
//TODO:ERRORHANDLING
	}

	//parse passnr in col 17
	if(!isCharInString(line[16],"0123456789 ")){
		MUSEERROR(err_ParsePassNr,idLineNumber,17);
	} else {
		if(line[16]!=' '){
			rdPassNr=line[16]-'0';
		} else {
			rdPassNr=-1;
		}
	}

	return 0;
}

void CMuseDataCombineDivideRecord::printOn(FILE *f)
{
	CMuseDataDataRecord::printOn(f);
	if(rdCombineDivideType==cdt_back){
		fprintf(f,"back-record");
	} else {
		fprintf(f,"irest-record");
	}
	fprintf(f,", skip %i, level %i, track %i",rdSkip,rdLevelNumber,rdTrackNumber);
}

int CMuseDataCombineDivideRecord::getChangeOfTime()
{
	if(rdCombineDivideType==cdt_back){return -rdSkip;}
	else {return rdSkip;}
}
