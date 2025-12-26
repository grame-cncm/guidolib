/*
  GuidoAR Library
  Copyright (C) 2008  Grame

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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __exceptions__
#define __exceptions__

//______________________________________________________________________________
typedef struct TException {

	TException(int n, const char* s, const char* f, int ln) 
		: num(n), msg(s), file(f), line(ln) {}

	const int	num;		// the exception number
	const char* msg;		// the message associated to the exception
	const char* file;		// the file where the exception occured
	const int	line;		// the line number within the file
}TException;

//______________________________________________________________________________
// a macro to automatically catch the file name and line number
#define eMsg(n)				kExceptionMsg[n]	
#define newMsgException(n,msg)	TException(n,msg,__FILE__,__LINE__)	
#define newException(n)			TException(n,eMsg(n),__FILE__,__LINE__)	

//______________________________________________________________________________
// exceptions definitions
enum ExceptionNum {
	kNoException,				// no exception - shouldn't be triggered
	kUndefinedCompOperation,	// undefined composition operator called
	kNullValue,					// got an unexpected null value
	kNullEnvironment,			// got a null environment
	kNullIdent,					// got a null identificator within a lambda abstraction
	kNullBody,					// got a null body within a lambda abstraction
	kMissingArgument			// an expression argument is missing
};

extern const char* kExceptionMsg[];	// and the associates messages

#endif
