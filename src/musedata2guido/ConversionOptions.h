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
