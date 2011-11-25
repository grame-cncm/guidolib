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
// ErrorHandling.h: declaration/interface of class CErrorHandling.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERRORHANDLING_H__59728460_8C05_11D5_9AE7_00600857CEE6__INCLUDED_)
#define AFX_ERRORHANDLING_H__59728460_8C05_11D5_9AE7_00600857CEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>

enum EErrorCode{
	err_NoError=0,
	err_NoInputFile,
	err_OpenInputFile,
	err_ReadInputFile,
	err_ParsingAbborted,
	err_ParsingCompleted,
	err_UnknownRecordType,
	err_UnsupportedRecord,
	err_ParseError,
	err_ParseDuration,
	err_ParsePassNr,
	err_ParseLevel,
	err_ParseTrack,
	err_ParseBarNumber,
	err_ParsePitch,
	err_ParseTieFlag,
	err_ParseNoteType,
	err_ParseStaffAssignment,
	err_ParseBeamCode,
	err_ParseDots,
	err_ParseNotatedAccidentals,
	err_ParseStemDirection,
	err_ParseAttribute,

	err_SyntaxBeam,
	err_EditorialLevelNotSupported,
	err_FootnoteNotSupported,

	err_PrintSuggestion,
	err_AppendRecord,

	err_ReopenSlur,
	err_SlurNotOpen,
	err_SlurNotClosed,
	err_ReopenTie,
	err_TieNotOpen,
	err_InconsistenBeaming,
	err_NoDivisionPerQuarterInformation,

	err_InternalMemNoRecordObject,

	err_NumberOfErrors
};

enum EErrorType{
	errtype_Error=0,
	errtype_Warning,
	errtype_Information
};

#define MUSEERROR(e,l,c)  if(CErrorHandling::error(e,l,c)==errtype_Error){return -1;}

#define OPERROR(o)  fprintf(stderr, "Unknown option %s\n",o);

class CErrorHandling  
{
public:
	static void init();
	static EErrorType error(EErrorCode e,int line,int col);
	CErrorHandling();
	virtual ~CErrorHandling();

protected:
	static int ignoreError[err_NumberOfErrors];
	static EErrorType errorType[err_NumberOfErrors];
};

#endif // !defined(AFX_ERRORHANDLING_H__59728460_8C05_11D5_9AE7_00600857CEE6__INCLUDED_)
