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
// MuseDataDataRecord.h: declaration/interface of class CMuseDataDataRecord.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSEDATADATARECORD_H__A5A9C524_8A6E_11D5_9AE7_00600857CEE6__INCLUDED_)
#define AFX_MUSEDATADATARECORD_H__A5A9C524_8A6E_11D5_9AE7_00600857CEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include "MuseDataHeader.h"
#include "ConversionParameters.h"

enum EDataRecordType{
	drt_NoteRecord=0,
	drt_CombineDivideRecord,
	drt_EndRecord,
	drt_MeasureLineRecord,
	drt_MusicalAttributes,
	drt_unknown
};

class CMuseDataDataRecord  
{
public:
	void setTrackID(int id);
	virtual int printGuidoOn(FILE*,CConversionParameters*);
	int getidLineNumber();
	virtual int parseAppendRecord(char*);
	int parsePrintSuggestionRecord(char*line);
	int getidTime();
	void reference();
	void release();
	void setLineNumber(int);
	void setidTime(int);
	virtual void printOn(FILE*);
	virtual int parseFromRecord(char*line);
	int parseFootnoteLevelTrack(char*line);
	int getTrackNumber();
	int getFootNoteFlag();
	int getLevelNumber();
	EDataRecordType getDataRecordType();
	CMuseDataDataRecord();

protected:
	int idLineNumber;
	int idTime;
	int rdTrackNumber;
	int rdLevelNumber;
	int rdFootNoteFlag;
	EDataRecordType rdType;
	virtual ~CMuseDataDataRecord();

private:
	int referenceCounter;
};

#endif // !defined(AFX_MUSEDATADATARECORD_H__A5A9C524_8A6E_11D5_9AE7_00600857CEE6__INCLUDED_)
