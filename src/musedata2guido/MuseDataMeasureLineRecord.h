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
// MuseDataMeasureLineRecord.h: declaration/interface of class CMuseDataMeasureLineRecord.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSEDATAMEASURELINERECORD_H__34531FC3_8B13_11D5_9AE7_00600857CEE6__INCLUDED_)
#define AFX_MUSEDATAMEASURELINERECORD_H__34531FC3_8B13_11D5_9AE7_00600857CEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MuseDataDataRecord.h"

enum EBarLineType{
	blt_none=0,
	blt_measure,
	blt_dotted,
	blt_double,
	blt_heavy1,
	blt_heavy2,
	blt_heavy3,
	blt_heavy4
};

class CMuseDataMeasureLineRecord : public CMuseDataDataRecord  
{
public:
	int printGuidoOn(FILE*,CConversionParameters*);
	virtual void printOn(FILE*);
	virtual int parseFromRecord(char*line);
	CMuseDataMeasureLineRecord();
	virtual ~CMuseDataMeasureLineRecord();

protected:
	EBarLineType rdBarLineType;
	int rdBarNumber;

};

#endif // !defined(AFX_MUSEDATAMEASURELINERECORD_H__34531FC3_8B13_11D5_9AE7_00600857CEE6__INCLUDED_)
