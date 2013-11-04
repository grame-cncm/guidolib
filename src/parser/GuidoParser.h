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
#include "GuidoFactory.h"

class ARFactory;

/* \brief a class for reading gmn streams
*/
class GuidoParser {
		
	void initScanner();
	void destroyScanner();

	int fzaehlerSet;
	int faccidentals;
	int fndots;

	bool fnt_enumSet;		/* enum of rel.dur explicitely specified? */
	long int fnt_enum;		/* rel. dur. of current / last note: */
	long int fnt_denom;

	int fErrorLine;			//< filled in case of syntax error
	int fErrorColumn;		//< filled in case of syntax error
	std::string fErrorMsg;	//< filled in case of syntax error
	
	int	_yyparse();
	
	public:
		void              *fScanner;   // the flex scanner
		std::istream      *fStream;    // input stream
		ARFactory         *fFactory;
		std::string	       fText;      // the current text

				 GuidoParser();
		virtual ~GuidoParser();
		
		ARHandler           parse();
        ARHandler           parseSynchronousParser();
        const ARFactory    *getFactory() const  { return fFactory; }
        ARFactory          *getFactory()        { return fFactory; }

        void                setStream(std::istream *stream);

        GuidoStream        *getGuidoStream()    { return (GuidoStream *)fStream; }
        std::istream       *getStream()         { return fStream; }

		void noteInit		(const char *id);
		void noteAcc		(int n);
		void noteOct		(int n);
		void noteEnum		(long int n);
		void noteDenom		(long int n);
		void noteDot		();
		void noteDdot		();
		void noteTdot		();
		void noteAbsDur		(long int n);
		void appendNote	();

		void chordInit		();
		void chordInitNote	();
		void seqAppendChord	();

		void seqInit		();
		void seqExit		();

		void segmInit		();
		void segmExit		();
		void segmAppendSeq	();

		void tagStart		(const char* id, long int no);
		void tagIntArg		(long int n);
		void tagFloatArg	(double r);
		void tagArgUnit		(const char* unit);
		void tagStrArg		(const char *s);
		void tagAdd			();
		void tagAddArg		(const char *s);
		void tagEnd			();
		void tagRange		();

		void setError(int line, int column, const char *msg)
					{ fErrorLine = line; fErrorColumn = column; fErrorMsg = msg; }

		int getErrorLine() const				{ return fErrorLine; }
		int getErrorColumn() const				{ return fErrorColumn; }
		const char* getErrorMsg() const			{ return fErrorMsg.c_str(); }
};

#endif
