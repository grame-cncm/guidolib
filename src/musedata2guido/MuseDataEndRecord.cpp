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
// MuseDataEndRecord.cpp: implementation of class CMuseDataEndRecord.
//
//////////////////////////////////////////////////////////////////////

#include "MuseDataEndRecord.h"
#include <string.h>
#include "ErrorHandling.h"

//////////////////////////////////////////////////////////////////////
// construction/deconstruction
//////////////////////////////////////////////////////////////////////

CMuseDataEndRecord::CMuseDataEndRecord()
{
	rdType=drt_EndRecord;
}

CMuseDataEndRecord::~CMuseDataEndRecord()
{

}

int CMuseDataEndRecord::parseFromRecord(char *line)
{
	if(strncmp(line,"/FINE",5)){
		rdEndRecordType=ert_fine;
	} else {
		if(strncmp(line,"/END",4)){
			rdEndRecordType=ert_end;
		} else {
			MUSEERROR(err_UnknownRecordType,idLineNumber,0);
		}
	}
	return 0;
}

void CMuseDataEndRecord::printOn(FILE *f)
{
	CMuseDataDataRecord::printOn(f);
	if(rdEndRecordType==ert_end){
		fprintf(f,"/END-record");
	}else{
		fprintf(f,"/FINE-record");
	}
}
