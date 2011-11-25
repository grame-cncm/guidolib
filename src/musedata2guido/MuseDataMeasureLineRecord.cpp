/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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


