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
