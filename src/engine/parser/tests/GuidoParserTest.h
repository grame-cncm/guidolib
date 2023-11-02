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

class TagParameter;
class ARMusicalTag;

/* \brief a class for reading gmn streams
*/
class GuidoParser {
	
    protected:
    
	virtual void initScanner();
	virtual void destroyScanner();

	int fErrorLine = 0;			//< filled in case of syntax error
    int fErrorColumn = 0;		//< filled in case of syntax error
	std::string fErrorMsg;		//< filled in case of syntax error
	
	virtual int	_yyparse();

	public:
		enum vartype { kString, kInt, kFloat };

        void              *fScanner;   // the flex scanner
		std::istream      *fStream;    // input stream
		std::string	       fText;      // the current text

				 GuidoParser(std::istream *stream)	: fStream(stream) {}
		virtual ~GuidoParser()	{}
		
		virtual int parse() {
			setlocale(LC_NUMERIC, "C");
			initScanner();
			int ret =_yyparse ();
		    destroyScanner();
			setlocale(LC_NUMERIC, 0);
			return ret;
		}
		// return the next char in stream or in expanded variable
		virtual bool get(char & c) {
			fStream->get(c);
			return !fStream->eof();
		}

		virtual void noteInit		(const char *id) {}
		virtual void tabInit		(int n, const char *fret) {}
		virtual void noteAcc		(int n) {}
		virtual void noteOct		(int n) {}
		virtual void noteEnum		(long int n) {}
		virtual void noteDenom		(long int n) {}
		virtual void noteDot		() {}
		virtual void noteDdot		() {}
		virtual void noteTdot		() {}
		virtual void noteAbsDur		(long int n) {}
		virtual void appendNote		() {}

		virtual void chordInit		() {}
		virtual void chordInitNote	() {}
		virtual void seqAppendChord	() {}

		virtual void seqInit		() {}
		virtual void seqExit		() {}

		virtual void segmInit		() {}
		virtual void segmExit		() {}
		virtual void segmAppendSeq	() {}

		virtual void tagStart		(const char* id, long int no) {}
		virtual void tagIntArg		(long int n) {}
		virtual void tagFloatArg	(double r) {}
		virtual void tagArgUnit		(const char* unit) {}
		virtual void tagStrArg		(const char *s) {}
		virtual void tagAdd			() {}
		virtual void tagAddArg		(const char *s) {}
		virtual void tagEnd			() {}
		virtual void tagRange		() {}

		virtual void tagParameter (TagParameter* param) {}
		virtual TagParameter* floatParam (float val) { return 0; }
		virtual TagParameter* floatParam (float val, const char* unit) { return 0; }
		virtual TagParameter* intParam (int val) { return 0; }
		virtual TagParameter* intParam (int val, const char* unit) { return 0; }
		virtual TagParameter* strParam (const char*) { return 0; }
		virtual TagParameter* varParam (const char* name) { std::cerr << "var param " << name << std::endl; return 0; }
		virtual void setParamName (TagParameter*, const char*) {}

		virtual void variableDecl (const char* name, const char* value, vartype type) { std::cerr << "declare variable type " << type << " " << name << ": " << value << std::endl; }


		virtual bool variableSymbols (const char* name) { std::cerr << "variableSym " << name << std::endl; return true; }

		virtual void setError(int line, int column, const char *msg)
					{ fErrorLine = line; fErrorColumn = column; fErrorMsg = msg; }

		virtual int getErrorLine() const		{ return fErrorLine; }
		virtual int getErrorColumn() const		{ return fErrorColumn; }
		virtual const char* getErrorMsg() const { return fErrorMsg.c_str(); }

		virtual void parseError(int line, int column, const char* msg)		{
			setError (line, column, msg);
			std::cerr << "error line: " << line << " col: " << column << ": " << msg << std::endl;
		}

};

#endif
