#ifndef ARPageFormat_H
#define ARPageFormat_H

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

#include <vector>
#include <string>

#include "ARMTParameter.h"

/*@mkdoc

@group:Layout

@tagname:\pageFormat
@tagalias:
@tagtype:P
@tagnotation:sets the page format
@tagdesc
@tagend

The **\pageFormat** tag indicates the score page format. It should be set at the beginning of the first staff.
When no page format is encountered, the default page format is 21cm x 29.6 (A4) and the margins are set to 2 cm.

@params:
@param:type:string:A4, A3 or letter:*none*:false when w and h are not present
@param:w:unit:the page width:*none*:false when type is not present
@param:h:unit:the page height:*none*:false when type is not present
@param:lm:unit:the left margin size:2cm:true
@param:tm:unit:the top margin size:5cm:true
@param:rm:unit:the right margin size:2cm:true
@param:bm:unit:the bottom margin size:3cm:true
@paramdesc
@paramend

*/

//class GRPage;

/** \brief not yet documented
*/
class ARPageFormat :  public ARMTParameter
{
//    friend class GRPage;

	public:
	//				 ARPageFormat( float px, float py, float ml, float mt, float mr, float mb );
	//				 ARPageFormat( char * px, char * py, char * ml, char * mt, char * mr, char * mb );
	//				 ARPageFormat( const ARPageFormat & arp );
					 ARPageFormat();
		virtual 	~ARPageFormat() {}

		void		 getPageFormat(float * sx, float * sy, float * ml, float * mt, float * mr, float * mb ) const;
		void		 setPageFormat(float sx, float sy, float ml, float mt, float mr, float mb );

		void operator=(const ARPageFormat& format);
		virtual bool IsStateTag() const { return true; }
	
		virtual TagParameterMap checkTagParameters (TagParametersList& params, const std::string pTemplate);
		virtual void setTagParameters (const TagParameterMap& params);

		virtual const char*	getParamsStr() const	{ return kARPageFormatParams; };
		virtual const char*	getTagName() const		{ return "ARPageFormat"; };
		virtual std::string getGMNName() const		{ return "\\pageFormat"; };

	protected:
		void  ClipSize();
		void  AdjustMargins();
	
		float mSizeX;		// All sizes are in internal (logical) units.
		float mSizeY;
		float mLeft;		
		float mTop;
		float mRight;
		float mBottom;
		std::string mFormat;

	private:
		void getMargins (const TagParameterMap& params);
};

#endif
