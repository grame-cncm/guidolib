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
// ConversionOptions.cpp: implementation of class CConversionOptions.
//
//////////////////////////////////////////////////////////////////////

#include "ConversionOptions.h"
#include "ErrorHandling.h"
#include <string.h>

//global ConversionOptions object
CConversionOptions conversionOptions;

//////////////////////////////////////////////////////////////////////
// construction/deconstruction
//////////////////////////////////////////////////////////////////////


CConversionOptions::CConversionOptions()
{
	sourceFile=NULL;
	destinationFile=NULL;
	beamMode=co_beams_SingleTag;
	printTrackInfo=1;
	printLineFeedAfterBarline=1;
	waitOnEnd=0;
	smartClefsKeysMeters=1;
}

CConversionOptions::~CConversionOptions()
{

}

int CConversionOptions::parseCommandLine(int argc, char **argv)
{
	if(argc<2){
		MUSEERROR(err_NoInputFile,0,0)
	}

	//Last option is sourcefile!
	sourceFile=argv[argc-1];

	//parse the other options
	for(int i=1;i<argc-1;i++){
		parseOption(argv[i]);	
	}
	return 0;
}

void CConversionOptions::parseOption(char *o)
{
	int rv=0;
	switch(o[0]){
	case 'b': //beam-mode
		switch(o[1]){
		case 'a': beamMode=co_beams_AutoBeam;break;
		case 'm': beamMode=co_beams_MultiTag;break;
		case 'n': beamMode=co_beams_NoBeams;break;
		case 's': beamMode=co_beams_SingleTag;break;
		default: OPERROR(o);
		}
		break;
	case 'o': //output-file
		destinationFile=o+1;break;
	case 'p': //printing-options
		switch(o[1]){
		case 'm': rv=parseSwitch(&printLineFeedAfterBarline,o+2);break;
		case 't': rv=parseSwitch(&printTrackInfo,o+2);break;
		}
		break;
	case 's': //smart keys, meters and clefs
		rv=parseSwitch(&smartClefsKeysMeters,o+1);break;
	case 'w': //waiting on end
		rv=parseSwitch(&waitOnEnd,o+1);break;
	default:
		OPERROR(o);
	}
	if(rv){
		OPERROR(o);
	};
}

int CConversionOptions::parseSwitch(int * s,char * value)
{
	switch(*value){
	case '+':*s=1;break;
	case '-':*s=0;break;
	default: return -1;
	}
	return 0;
}
