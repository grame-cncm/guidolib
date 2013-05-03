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
// MuseDataRecordList.h: declaration/interface of class CMuseDataRecordList.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSEDATARECORDLIST_H__3E3618A3_8B43_11D5_9AE7_00600857CEE6__INCLUDED_)
#define AFX_MUSEDATARECORDLIST_H__3E3618A3_8B43_11D5_9AE7_00600857CEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MuseDataDataRecord.h"
#include "MuseDataHeader.h"
#include "ConversionParameters.h"

class CMuseDataRecordListEntry{
public:
	CMuseDataRecordListEntry* before;
	CMuseDataRecordListEntry* next;
	CMuseDataDataRecord* record;
};

class CMuseDataRecordList  
{
public:
	void gotoBeforeRecord();
	int convCalcSortListByRecordTime();
	void setTrackNo(int track, int subtrack);
	int convMakeBeams();
	CMuseDataRecordList* stripImplicitSubtrack();
	CMuseDataRecordList* createExplicitSubtrack(int id);
	int usesNotesWithoutExplicitTrack();
	int usesExplicitTrack(int i);
	int convCalcTrackUsage();
	int convMarkCueRanges();
	int convMarkGraceRanges();
	int printGuidoOn(FILE*,CConversionParameters*);
	int convCalcTieIDs(int);
	int convCalcSlurIDs(int);
	int hasOverlappingNotes();
	int convRemoveCombineDivideRecords();
	int convCalculateRecordTimes();
	int convMarkChords();
	CMuseDataDataRecord* removeCurrentRecord();
	int isEndOfList();
	CMuseDataDataRecord* peekCurrentRecord();
	void gotoNextRecord();
	void gotoFirstRecord();
	void printOn(FILE*);
	int appendRecord(CMuseDataDataRecord*dr);
	CMuseDataRecordList();
	virtual ~CMuseDataRecordList();

protected:
	int idTrackNo;
	int idSubtrackNo;
	CMuseDataRecordListEntry * first;
	CMuseDataRecordListEntry * last;
	CMuseDataRecordListEntry * before;
	CMuseDataRecordListEntry * current;
	int idUsesExplicitTrack[10];
	int idNotesWithoutTrackID;
};

typedef CMuseDataRecordList* pMuseDataRecordList;

#endif // !defined(AFX_MUSEDATARECORDLIST_H__3E3618A3_8B43_11D5_9AE7_00600857CEE6__INCLUDED_)
