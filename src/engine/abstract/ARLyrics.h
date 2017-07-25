#ifndef ARLyrics_H
#define ARLyrics_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARPositionTag.h"
#include "ARFontAble.h"
//#include "GuidoDefs.h"		// for LSPACE

class ARText;

/** \brief Implement the lyrics Tag
*/
class ARLyrics : public ARFontAble, public ARPositionTag
{
	public:

		friend class ARText;
	
					 ARLyrics(bool autopos=false);
		virtual		~ARLyrics() {}

		virtual void setTagParameters (const TagParameterMap& params);
	
		virtual const char* getText() const			{ return fText.c_str(); }
		virtual const char*	getParamsStr() const	{ return kARLyricsParams; };
		virtual const char*	getTagName() const		{ return "ARLyrics"; };
		virtual std::string getGMNName() const		{ return "\\lyrics"; };
		bool		autoPos() const					{ return fAutoPos; }

	protected:
		std::string	fText;		// the corresponding text ...
		bool		fAutoPos;
};

#endif
