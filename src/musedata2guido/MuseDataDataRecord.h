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
