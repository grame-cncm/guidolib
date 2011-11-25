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
