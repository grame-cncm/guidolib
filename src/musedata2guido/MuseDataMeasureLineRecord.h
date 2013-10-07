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
