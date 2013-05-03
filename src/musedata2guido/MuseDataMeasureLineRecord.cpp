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
// MuseDataMeasureLineRecord.cpp: implementation of class CMuseDataMeasureLineRecord.
//
//////////////////////////////////////////////////////////////////////

#include "MuseDataMeasureLineRecord.h"
#include <string.h>
#include "parseutils.h"
#include "ErrorHandling.h"
#include "GuidoCreator.h"

//////////////////////////////////////////////////////////////////////
// construction/deconstruction
//////////////////////////////////////////////////////////////////////

CMuseDataMeasureLineRecord::CMuseDataMeasureLineRecord()
{
	rdType=drt_MeasureLineRecord;
}

CMuseDataMeasureLineRecord::~CMuseDataMeasureLineRecord()
{

}

int CMuseDataMeasureLineRecord::parseFromRecord(char *line)
{
	//check type of barline: (col 1 to 8)
	rdBarLineType=blt_none;
	if(strncmp(line,"measure ",8)==0){rdBarLineType=blt_measure;}
	if(strncmp(line,"mdotted ",8)==0){rdBarLineType=blt_dotted;}
	if(strncmp(line,"mdouble ",8)==0){rdBarLineType=blt_double;}
	if(strncmp(line,"mheavy1 ",8)==0){rdBarLineType=blt_heavy1;}
	if(strncmp(line,"mheavy2 ",8)==0){rdBarLineType=blt_heavy2;}
	if(strncmp(line,"mheavy3 ",8)==0){rdBarLineType=blt_heavy3;}
	if(strncmp(line,"mheavy4 ",8)==0){rdBarLineType=blt_heavy4;}
	if(rdBarLineType==blt_none){
		MUSEERROR(err_UnknownRecordType,idLineNumber,0)
	}

	//parse bar-number (col 9 to 12)
	if(parseBarNumber(&line[8],&rdBarNumber)){
		MUSEERROR(err_ParseBarNumber,idLineNumber,9);
	}

	//parse footnote, level and track in superclass (col 13 to 16)
	int rv=parseFootnoteLevelTrack(line);
	if(rv){
//TODO:ERRORHANDLING
	}

	return 0;
}

void CMuseDataMeasureLineRecord::printOn(FILE *f)
{
	CMuseDataDataRecord::printOn(f);
	fprintf(f,"m..-record, bar number %i, level %i, track %i",rdBarNumber,rdLevelNumber,rdTrackNumber);
}

int CMuseDataMeasureLineRecord::printGuidoOn(FILE *f, CConversionParameters *)
{
	guidoCreator.measureLine();
	return 0;
}


