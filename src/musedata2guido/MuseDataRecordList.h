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
