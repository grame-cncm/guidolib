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
// ErrorHandling.cpp: implementation of class CErrorHandling.
//
//////////////////////////////////////////////////////////////////////

#include "ErrorHandling.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// construction/deconstruction
//////////////////////////////////////////////////////////////////////

EErrorType CErrorHandling::errorType[err_NumberOfErrors];

CErrorHandling::CErrorHandling()
{

}

CErrorHandling::~CErrorHandling()
{

}

#define LINE fprintf(stderr, " near line %i",line);
#define COLUMN fprintf(stderr, " column %i",col);
#define TEXT(t) fprintf(stderr, ":"); fprintf(stderr, t); fprintf(stderr, "\n");break;

EErrorType CErrorHandling::error(EErrorCode e,int line,int col)
{
	EErrorType et=errorType[e];
	switch(et){
	case errtype_Error:fprintf(stderr, "ERROR");break;
	case errtype_Warning:fprintf(stderr, "WARNING");break;
	case errtype_Information:fprintf(stderr, "INFORMATION");break;
	}

	switch(e){
	case err_ParseError:LINE TEXT("parse error")
	case err_NoInputFile:TEXT("no input file specified")							
	case err_OpenInputFile:TEXT("couldn't open input file")
	case err_ReadInputFile:LINE TEXT("couldn't read input file")
	case err_ParsingAbborted:TEXT("parsing of input file abborted")
	case err_ParsingCompleted:TEXT("parsing of input file completed")
	case err_UnknownRecordType:LINE TEXT("unknown record type")
	case err_UnsupportedRecord:LINE TEXT("record type not supported")					
	case err_ParseDuration:LINE COLUMN TEXT("while parsing duration")
	case err_ParsePassNr:LINE COLUMN TEXT("while parsing pass number")
	case err_ParseTrack:LINE COLUMN TEXT("while parsing track number")
	case err_ParseLevel:LINE COLUMN TEXT("while parsing level number")
	case err_ParseBarNumber:LINE COLUMN TEXT("while parsing bar number")
	case err_ParsePitch:LINE COLUMN TEXT("while parsing pitch information")
	case err_ParseTieFlag:LINE COLUMN TEXT("while parsing tie flag")
	case err_ParseNoteType:LINE COLUMN TEXT("while parsing note type")
	case err_ParseStaffAssignment:LINE COLUMN TEXT("while parsing staff assignment")
	case err_ParseBeamCode:LINE COLUMN TEXT("while parsing beam code")
	case err_ParseDots:LINE COLUMN TEXT("while parsing dots of prolongation")
	case err_ParseNotatedAccidentals:LINE COLUMN TEXT("while parsing notated accidentals")
	case err_ParseStemDirection:LINE COLUMN TEXT("while parsing stem direction")
	case err_ParseAttribute:LINE COLUMN TEXT("while parsing attibute")

	case err_SyntaxBeam:LINE COLUMN TEXT("unclear beam syntax")
	case err_EditorialLevelNotSupported:LINE COLUMN TEXT("level feature not supported!")
	case err_FootnoteNotSupported:LINE COLUMN TEXT("footnote feature not supported!")

	case err_PrintSuggestion:LINE TEXT("append-record has no target")
	case err_AppendRecord:LINE TEXT("print-suggestion has no target")

	case err_ReopenSlur:LINE TEXT("slur already open")
	case err_SlurNotOpen:LINE TEXT("slur not open")
	case err_SlurNotClosed:LINE TEXT("slur not closed")
	case err_ReopenTie:LINE TEXT("tie already open")
	case err_TieNotOpen:LINE TEXT("tie not open")
	case err_InconsistenBeaming:LINE TEXT("inconsistent beming")
	case err_NoDivisionPerQuarterInformation:LINE TEXT("divisions per quarter not defined")


	case err_InternalMemNoRecordObject:LINE TEXT("internal error: couldn't create record-object")
	}
	return et;
}

#define DET(e,t) errorType[e]=t;
#define E errtype_Error
#define W errtype_Warning
#define I errtype_Information

void CErrorHandling::init()
{
	DET(err_NoError,I)
	DET(err_NoInputFile,E)
	DET(err_OpenInputFile,E)
	DET(err_ReadInputFile,E)
	DET(err_ParsingAbborted,I)
	DET(err_ParsingCompleted,I)
	DET(err_UnknownRecordType,W)
	DET(err_UnsupportedRecord,W)
	DET(err_ParseError,E)
	DET(err_ParseDuration,E)
	DET(err_ParsePassNr,W)
	DET(err_ParseTrack,E)
	DET(err_ParseLevel,E)
	DET(err_ParseBarNumber,W)
	DET(err_ParsePitch,E)
	DET(err_ParseTieFlag,E)
	DET(err_ParseNoteType,W);
	DET(err_ParseStaffAssignment,E);
	DET(err_ParseBeamCode,E);
	DET(err_ParseDots,E);
	DET(err_ParseNotatedAccidentals,W);
	DET(err_ParseStemDirection,W);
	DET(err_ParseAttribute,E);

	DET(err_SyntaxBeam,W);
	DET(err_EditorialLevelNotSupported,W);
	DET(err_FootnoteNotSupported,W);

	DET(err_PrintSuggestion,E);
	DET(err_AppendRecord,E);

	DET(err_ReopenSlur,W);
	DET(err_SlurNotOpen,W);
	DET(err_SlurNotClosed,W);
	DET(err_ReopenTie,W);
	DET(err_TieNotOpen,W);
	DET(err_InconsistenBeaming,W);
	DET(err_NoDivisionPerQuarterInformation,E);

	DET(err_InternalMemNoRecordObject,E);

}
