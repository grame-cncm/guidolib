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
