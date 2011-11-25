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
