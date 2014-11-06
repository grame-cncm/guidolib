/*
 GUIDO Library
 Copyright (C) 2013 Grame
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 
 */

#ifndef __GuidoParser__
#define __GuidoParser__

#include <istream>
#include <string>
#include <sstream>
#include <locale.h>

#include "GuidoStream.h"
#include "GUIDOFactory.h"

class ARFactory;

/* \brief a class for reading gmn streams
*/
class GuidoParser {
	
    protected:
    
	virtual void initScanner();
	virtual void destroyScanner();

	int fzaehlerSet;
	int faccidentals;
	int fndots;

	bool fnt_enumSet;		/* enum of rel.dur explicitely specified? */
	long int fnt_enum;		/* rel. dur. of current / last note: */
	long int fnt_denom;

	int fErrorLine;			//< filled in case of syntax error
    int fErrorColumn;		//< filled in case of syntax error
	std::string fErrorMsg;	//< filled in case of syntax error
	
	virtual int	_yyparse();

	public:
        void              *fScanner;   // the flex scanner
		std::istream      *fStream;    // input stream
		ARFactory         *fFactory;
		std::string	       fText;      // the current text

				 GuidoParser();
		virtual ~GuidoParser();
		
		virtual ARHandler           parse();
        virtual const ARFactory    *getFactory() const  { return fFactory; }

        virtual void                setStream(std::istream *stream);

		virtual void noteInit		(const char *id);
		virtual void noteAcc		(int n);
		virtual void noteOct		(int n);
		virtual void noteEnum		(long int n);
		virtual void noteDenom		(long int n);
		virtual void noteDot		();
		virtual void noteDdot		();
		virtual void noteTdot		();
		virtual void noteAbsDur		(long int n);
		virtual void appendNote	();

		virtual void chordInit		();
		virtual void chordInitNote	();
		virtual void seqAppendChord	();

		virtual void seqInit		();
		virtual void seqExit		();

		virtual void segmInit		();
		virtual void segmExit		();
		virtual void segmAppendSeq	();

		virtual void tagStart		(const char* id, long int no);
		virtual void tagIntArg		(long int n);
		virtual void tagFloatArg	(double r);
		virtual void tagArgUnit		(const char* unit);
		virtual void tagStrArg		(const char *s);
		virtual void tagAdd			();
		virtual void tagAddArg		(const char *s);
		virtual void tagEnd			();
		virtual void tagRange		();

		virtual void setError(int line, int column, const char *msg)
					{ fErrorLine = line; fErrorColumn = column; fErrorMsg = msg; }

		virtual int getErrorLine() const		{ return fErrorLine; }
		virtual int getErrorColumn() const		{ return fErrorColumn; }
		virtual const char* getErrorMsg() const { return fErrorMsg.c_str(); }
};

#endif
