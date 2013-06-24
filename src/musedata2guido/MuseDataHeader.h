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
// MuseDataHeader.h: declaration/interface of class CMuseDataHeader.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSEDATAHEADER_H__A5A9C523_8A6E_11D5_9AE7_00600857CEE6__INCLUDED_)
#define AFX_MUSEDATAHEADER_H__A5A9C523_8A6E_11D5_9AE7_00600857CEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMuseDataHeader  
{
public:
	int getrdDivisionsPerQuarter();
	int parseFromRecord(char * line,int);
	CMuseDataHeader();
	virtual ~CMuseDataHeader();

protected:
	int parseAttribute(char*,int*);
	int rdKey;
	int rdDivisionsPerQuarter;
	int rdTimeDesignationDen;
	int rdTimeDesignationNum;
	int rdClef[2];
	char rdDirective[2][80];
	int rdTransposingPart;
	int rdNumberOfStaves;
	int rdNumberOfInstruments;
	int attributesRead;
	int parseGroupPart(char*);
	int parseMusicalAttributes(char*,int);
	int parseGroupMemberships(char*);
	int parseMiscDest(char*);
	int parseLineNr;
	char rec1[81];
	char rec2[81];
	char rec3[81];
	char date_name[81];
	char work_movement[81];
	char source[81];
	char work_title[81];
	char movement_title[81];
	char name_of_part[81];


};

#endif // !defined(AFX_MUSEDATAHEADER_H__A5A9C523_8A6E_11D5_9AE7_00600857CEE6__INCLUDED_)
