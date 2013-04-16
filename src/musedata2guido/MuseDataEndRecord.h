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
// MuseDataEndRecord.h: declaration/interface of class CMuseDataEndRecord.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSEDATAENDRECORD_H__3E3618A1_8B43_11D5_9AE7_00600857CEE6__INCLUDED_)
#define AFX_MUSEDATAENDRECORD_H__3E3618A1_8B43_11D5_9AE7_00600857CEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MuseDataDataRecord.h"

enum EEndRecordType{
	ert_end=0,
	ert_fine
};

class CMuseDataEndRecord : public CMuseDataDataRecord  
{
public:
	virtual void printOn(FILE*);
	virtual int parseFromRecord(char * line);
	CMuseDataEndRecord();
	virtual ~CMuseDataEndRecord();

protected:
	EEndRecordType rdEndRecordType;
};

#endif // !defined(AFX_MUSEDATAENDRECORD_H__3E3618A1_8B43_11D5_9AE7_00600857CEE6__INCLUDED_)
