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
// MuseDataMusicalAttributesRecord.h: declaration/interface of class CMuseDataMusicalAttributesRecord.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSEDATAMUSICALATTRIBUTESRECORD_H__E7BE0C61_9611_11D5_9AE7_00600857CEE6__INCLUDED_)
#define AFX_MUSEDATAMUSICALATTRIBUTESRECORD_H__E7BE0C61_9611_11D5_9AE7_00600857CEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MuseDataDataRecord.h"

class CMuseDataMusicalAttributesRecord : public CMuseDataDataRecord  
{
public:
	int printGuidoOn(FILE*, CConversionParameters *);
	virtual int parseFromRecord(char *);
	CMuseDataMusicalAttributesRecord();
	virtual ~CMuseDataMusicalAttributesRecord();

protected:
	int parseTimeDesignation(char * s);
	int parseAttribute(char * s,int *v);
	int rdKey;
	int rdDivisionsPerQuarter;
	int rdTimeDesignationDen;
	int rdTimeDesignationNum;
	int rdClef[2];
	char rdDirective[2][80];
	int rdTransposingPart;
	int rdNumberOfStaves;
	int rdNumberOfInstruments;


};

#endif // !defined(AFX_MUSEDATAMUSICALATTRIBUTESRECORD_H__E7BE0C61_9611_11D5_9AE7_00600857CEE6__INCLUDED_)
