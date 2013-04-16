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
// MuseDataFile.h: declaration/interface of class CMuseDataFile.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSEDATAFILE_H__3E3618A2_8B43_11D5_9AE7_00600857CEE6__INCLUDED_)
#define AFX_MUSEDATAFILE_H__3E3618A2_8B43_11D5_9AE7_00600857CEE6__INCLUDED_

#include "ConversionParameters.h"	// Added by the ClassView
#include "MuseDataHeader.h"	// Added by the ClassView
#include "MuseDataRecordList.h"	// Added by the ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuidoCreator.h"
#include "ParseParameters.h"

class CMuseDataFile  
{
public:
	int convBeforeSplit();
	CConversionParameters conversionParameters;
	int convConvertSubtracks();
	int printGuidoOn(FILE*);
	int convSplitSubtracks();
	int addSubtrack(CMuseDataRecordList*);
	CMuseDataHeader* peekHeader();
	int readFromFile(char* filename);
	void printOn(FILE*f);
	CMuseDataRecordList* peekRecordList();
	CMuseDataFile();
	virtual ~CMuseDataFile();

protected:
	pMuseDataRecordList * subtrackCollection;
	int subtrackCount;
	int subtrackMax;
	CMuseDataDataRecord* parseLastParsedDataRecord;
	int parseHeaderFlag;
	int parseCommentFlag;
	int parseDataRecord(char *);
	int parseHeaderRecord(char*);
	int parseLine(char*);
	int parseLineNumber;
	int parseLoop(FILE *);
	int parseInit();
	CMuseDataHeader fileHeader;
	CMuseDataRecordList recordList;
};

#endif // !defined(AFX_MUSEDATAFILE_H__3E3618A2_8B43_11D5_9AE7_00600857CEE6__INCLUDED_)
