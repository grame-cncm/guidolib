/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "MuseDataFile.h"
#include "MuseDataCombineDivideRecord.h"
#include "MuseDataDataRecord.h"
#include "MuseDataEndRecord.h"
#include "MuseDataMeasureLineRecord.h"
#include "MuseDataNoteRecord.h"
#include "MuseDataMusicalAttributesRecord.h"

#include "ErrorHandling.h"

#define MuseData2GuidoExport
#include "MuseData2Guido.h"

//////////////////////////////////////////////////////////////////////
// construction/deconstruction
//////////////////////////////////////////////////////////////////////

CMuseDataFile::CMuseDataFile()
{
	//Create data for Handling of subtracks
	subtrackCollection= new pMuseDataRecordList[4];
	subtrackMax=4;
	subtrackCount=0;
}

CMuseDataFile::~CMuseDataFile()
{

}

CMuseDataRecordList* CMuseDataFile::peekRecordList()
{
	return &recordList;
}




void CMuseDataFile::printOn(FILE *f)
{
	fprintf(f,"recordList:\n");
	recordList.printOn(f);
	for(int i=0;i<subtrackCount;i++){
		getchar();
		fprintf(f,"subtrack %i:\n",i);
		subtrackCollection[i]->printOn(f);
	}
	fprintf(stderr, "End of file\n");
	getchar();
}

int CMuseDataFile::readFromFile(char *filename)
{
	FILE * file=fopen(filename,"rt");
	if(file==NULL){
		MUSEERROR(err_OpenInputFile,0,0);
		return -1;
	}
	if(parseInit()!=0){
		fclose(file);
		MUSEERROR(err_ParsingAbborted,0,0);
		return -1;
	} else {
		if(parseLoop(file)!=0){
			fclose(file);
			MUSEERROR(err_ParsingAbborted,0,0);
			return -1;
		}
	}
	fclose(file);
	MUSEERROR(err_ParsingCompleted,0,0);
	return 0;
}

int CMuseDataFile::parseLoop(FILE * f)
{
	char line[81];
	while(!feof(f)){
		//read one line from file
		parseLineNumber++;
		if(fgets(line,80,f)==NULL){
			if(feof(f)){
				continue;
			} else {
				MUSEERROR(err_ReadInputFile,parseLineNumber,0);
			}
		}

		//beautify read line: fill end with blanks and remove eol-sign
		for(int i=0;i<80;i++){
			if( (line[i]=='\n') || (line[i]==0) ){
				for(i;i<80;i++){line[i]=' ';}
			}
		}
		line[80]=0;
		//parse the line
		if(parseLine(line)!=0){return -1;}
	}
	return 0;
}

int CMuseDataFile::parseInit()
{
	parseLineNumber=0;
	parseCommentFlag=0;
	parseHeaderFlag=1;
	parseLastParsedDataRecord=NULL;

	parseParameters.lastNoteDuration=-1;
	return 0;
}
 
int CMuseDataFile::parseLine(char * line)
{
	//begin or end of a multi-line-comment??
	if(line[0]=='&'){
		parseCommentFlag=1-parseCommentFlag;
		return 0;
	}
	//are we in a comment??
	if(parseCommentFlag){return 0;}
	//a single-line comment??
	if(line[0]=='@'){return 0;}

	//are we still parsing the header??
	if(parseHeaderFlag){
		int rv=parseHeaderRecord(line);
		if(rv==1){
			//this is the first line AFTER the header
			parseHeaderFlag=0;
		} else {
			return rv;
		}
	}
	//otherwise we are parsing data-records
	return parseDataRecord(line);
}

int CMuseDataFile::parseHeaderRecord(char * line)
{
	return fileHeader.parseFromRecord(line,parseLineNumber);
}

int CMuseDataFile::parseDataRecord(char * line)
{
	CMuseDataDataRecord* newRecord=NULL;

	switch(line[0]){
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'r':
	case ' ':
	case 'c':
	case 'g':
		newRecord=new CMuseDataNoteRecord();break;
	case 'f':
		MUSEERROR(err_UnsupportedRecord,parseLineNumber,0);
		return 0;
	case 'b':
	case 'i':
		newRecord=new CMuseDataCombineDivideRecord();break;
	case 'm':
		newRecord=new CMuseDataMeasureLineRecord(); break;
	case '*':
		MUSEERROR(err_UnsupportedRecord,parseLineNumber,0);
		return 0;
	//case '@':break; //handled in parseLine
	//case '&':break; //handled in parseLine
	case '/':
		newRecord=new CMuseDataEndRecord();break;
	case 'P':
		//for printing-suggestions we can't simply create a new record,
		//instead we send them to the last read record, if there was any
		if(parseLastParsedDataRecord==NULL){
			MUSEERROR(err_PrintSuggestion,parseLineNumber,0);
			return 0; //no record before, sniff
		} else {
			return parseLastParsedDataRecord->parsePrintSuggestionRecord(line);
		}
		break;
	case 'a':
		//Append-Records are handled like P-records!
		if(parseLastParsedDataRecord==NULL){
			MUSEERROR(err_PrintSuggestion,parseLineNumber,0);
			return 0; //no record before
		} else {
			return parseLastParsedDataRecord->parseAppendRecord(line);
		}

		break;
	case 'S':
		MUSEERROR(err_UnsupportedRecord,parseLineNumber,0);
		return 0;
	case '$':
		newRecord=new CMuseDataMusicalAttributesRecord();
		break;
	default:
		MUSEERROR(err_UnknownRecordType,parseLineNumber,0);
		return 0;
	}

	if(newRecord==NULL){
		MUSEERROR(err_InternalMemNoRecordObject,parseLineNumber,0);
		return 0;
	}

	//set new records line-number from file
	newRecord->setLineNumber(parseLineNumber);
	//parse the record
	int retVal=newRecord->parseFromRecord(line);
	if(retVal!=0){
		return -1;
	}

	//store new record
	recordList.appendRecord(newRecord);
	//maybe the next record is an Addition to this one, so we store it here
	parseLastParsedDataRecord=newRecord;
	return 0;
}

CMuseDataHeader* CMuseDataFile::peekHeader()
{
	return &fileHeader;
}


int CMuseDataFile::addSubtrack(CMuseDataRecordList * l)
{
	if(subtrackCount<subtrackMax){
		subtrackCollection[subtrackCount]=l;
		subtrackCount++;
		return 0;
	} else {
		pMuseDataRecordList* newList=new pMuseDataRecordList[subtrackMax+4];
		for(int i=0;i<subtrackMax;i++){newList[i]=subtrackCollection[i];}
		delete [] subtrackCollection;
		subtrackCollection=newList;
		subtrackMax+=4;
		return addSubtrack(l);
	}
}

int CMuseDataFile::convSplitSubtracks()
{
	CMuseDataRecordList* currentList;
	CMuseDataRecordList* newList;
	int st;
	//initialize splitting of recordlist
	recordList.convCalcTrackUsage();
	//get all explicit tracks and split them
	for(int i=-1;i<10;i++){
		st=1;
		if(recordList.usesExplicitTrack(i)){
			//create list for explicit subtrack
			currentList=recordList.createExplicitSubtrack(i);
			//split this further into implicit subtracks
			while(currentList->hasOverlappingNotes()){
				//add each implicit subtrack to the collection
				newList=currentList->stripImplicitSubtrack();
				newList->setTrackNo(i,st);
				addSubtrack(newList);
				st++;
			}
			//and also add the remaining track
			currentList->setTrackNo(i,st);
			addSubtrack(currentList);
		}
	}
	return 0;
}

int CMuseDataFile::printGuidoOn(FILE * f)
{
	guidoCreator.setFile(f);
	guidoCreator.segmentBegin();
	for(int i=0;i<subtrackCount;i++){
		if(subtrackCollection[i]->printGuidoOn(f,&conversionParameters)){
			return -1;
		};
	}
	guidoCreator.segmentEnd();
	return 0;
}


int CMuseDataFile::convConvertSubtracks()
{
	int nextID=1;
	for(int i=0;i<subtrackCount;i++){
		nextID=subtrackCollection[i]->convCalcTieIDs(nextID);
		nextID=subtrackCollection[i]->convCalcSlurIDs(nextID);
		subtrackCollection[i]->convMarkCueRanges();
		subtrackCollection[i]->convMarkGraceRanges();
		subtrackCollection[i]->convMakeBeams();
	}
	return 0;
}

int CMuseDataFile::convBeforeSplit()
{
	fprintf(stderr, "calculating times:\n");
	if(peekRecordList()->convCalculateRecordTimes()){
		return -1;
	}
	if(peekRecordList()->convRemoveCombineDivideRecords()){
		return -1;
	}
	fprintf(stderr, "marking chords:\n");	
	if(peekRecordList()->convMarkChords()){
		return -1;
	}
	peekRecordList()->convCalcSortListByRecordTime();	
	return 0;
}

#include "ConversionOptions.h"

int mainmain(int argc,char**argv){
	CMuseDataFile f;

	CErrorHandling::init();
	int nextID=1;
	
	if(conversionOptions.parseCommandLine(argc,argv)){
		return -1;
	};

	fprintf(stderr, "reading file: %s\n",conversionOptions.sourceFile);
	if(f.readFromFile(conversionOptions.sourceFile)){
		return -1;
	};



	if(f.convBeforeSplit()){
		return -1;
	};


	fprintf(stderr, "splittig subtracks:\n");
	if(f.convSplitSubtracks()){
		return -1;
	}

	fprintf(stderr, "converting subtracks\n");
	if(f.convConvertSubtracks()){
		return -1;
	}

	FILE * o;
	fprintf(stderr, "generating gmn-file ");
	if(conversionOptions.destinationFile==NULL){
//		o=fopen("a.out.gmn","wt");
		o=stdout;
		fprintf(stderr, "a.out.gmn");
	} else {
		o=fopen(conversionOptions.destinationFile,"wt");
		fprintf(stderr, "%s\n",conversionOptions.destinationFile);
	}

	int rv=f.printGuidoOn(o);
	fclose(o);
	
	return rv;
}//mainmain


#ifdef WIN32
#include <windows.h>

int __stdcall DllMain(HINSTANCE hInstance, DWORD fdwReason,
					PVOID pvReserverd)
{

	if (fdwReason == DLL_PROCESS_ATTACH 
		|| fdwReason == DLL_THREAD_ATTACH)
	{
	}
	else if (fdwReason == DLL_PROCESS_DETACH
		|| fdwReason == DLL_THREAD_DETACH)
	{
	}

	return TRUE;
}
#endif

MuseData2Guido_API int md2gmn(int argc,char**argv, const char * logfilename)
{
	FILE * stream = freopen(logfilename,"w",stdout);
	int retval = mainmain(argc,argv);
	fclose(stream);
	return retval;
}
