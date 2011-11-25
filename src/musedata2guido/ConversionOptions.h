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
// ConversionOptions.h: declaration/interface of class CConversionOptions.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVERSIONOPTIONS_H__60963740_9C7D_11D5_9AE7_00600857CEE6__INCLUDED_)
#define AFX_CONVERSIONOPTIONS_H__60963740_9C7D_11D5_9AE7_00600857CEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum co_beams{
	co_beams_SingleTag=0,
	co_beams_MultiTag,
	co_beams_AutoBeam,
	co_beams_NoBeams
};


class CConversionOptions  
{
public:
	int parseSwitch(int * s, char* value);
	void parseOption(char * o);
	int parseCommandLine(int argc, char ** argv);
	CConversionOptions();
	virtual ~CConversionOptions();
	char * sourceFile;
	char * destinationFile;
	co_beams beamMode;
	int printTrackInfo;
	int printLineFeedAfterBarline;
	int waitOnEnd;
	int smartClefsKeysMeters;
};

extern CConversionOptions conversionOptions;

#endif // !defined(AFX_CONVERSIONOPTIONS_H__60963740_9C7D_11D5_9AE7_00600857CEE6__INCLUDED_)
