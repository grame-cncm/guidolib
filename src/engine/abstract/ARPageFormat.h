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
