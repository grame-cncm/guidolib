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
// MuseDateCombineDivideRecord.h: declaration/interface of class CMuseDateCombineDivideRecord.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSEDATECOMBINEDIVIDERECORD_H__3E3618A0_8B43_11D5_9AE7_00600857CEE6__INCLUDED_)
#define AFX_MUSEDATECOMBINEDIVIDERECORD_H__3E3618A0_8B43_11D5_9AE7_00600857CEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MuseDataDataRecord.h"

enum ECombineDivideType{
	cdt_back=0,
	cdt_irest
};

class CMuseDataCombineDivideRecord : public CMuseDataDataRecord  
{
public:
	int getChangeOfTime();
	virtual void printOn(FILE*);
	virtual int parseFromRecord(char*line);
	CMuseDataCombineDivideRecord();
	virtual ~CMuseDataCombineDivideRecord();

protected:
	ECombineDivideType rdCombineDivideType;
	int	rdSkip;
	int rdPassNr;

};

#endif // !defined(AFX_MUSEDATECOMBINEDIVIDERECORD_H__3E3618A0_8B43_11D5_9AE7_00600857CEE6__INCLUDED_)
