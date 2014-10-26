/*
 GUIDO Library
 Copyright (C) 2013 Grame
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 
 */

#ifndef __NoteAndChordParser__
#define __NoteAndChordParser__

#include <istream>
#include <string>
#include <sstream>
#include <locale.h>

#include "GuidoStream.h"
#include "GUIDOFactory.h"
#include "GuidoParser.h"
#include "NoteAndChordFactory.h"

class ARFactory;

/* \brief a class for reading gmn streams
*/
class NoteAndChordParser : public GuidoParser {
		
	
	public:

				 NoteAndChordParser();
		virtual ~NoteAndChordParser();
		
		void parseNoteOrChord();
    
        // We ignore the durations
		virtual void noteEnum		(long int n){}
		virtual void noteDenom		(long int n){}
		virtual void noteDot		(){}
		virtual void noteDdot		(){}
		virtual void noteTdot		(){}
		virtual void noteAbsDur		(long int n){}
    
        void seqInit		(){}
		void seqExit		(){}

		void segmInit		(){}
		void segmExit		(){}
		void segmAppendSeq	(){}

        // We ignore the tags
		void tagStart		(const char* id, long int no){}
		void tagIntArg		(long int n){}
		void tagFloatArg	(double r){}
		void tagArgUnit		(const char* unit){}
		void tagStrArg		(const char *s){}
		void tagAdd			(){}
		void tagAddArg		(const char *s){}
		void tagEnd			(){}
		void tagRange		(){}
    
        void setFactory     (NoteAndChordFactory * factory){fFactory = factory;}

};

#endif
