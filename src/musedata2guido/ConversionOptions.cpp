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
