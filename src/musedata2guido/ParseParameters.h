/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
// ParseParameters.h: declaration/interface of class CParseParameters.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSEPARAMETERS_H__015ADE40_A05E_11D5_9AE7_00600857CEE6__INCLUDED_)
#define AFX_PARSEPARAMETERS_H__015ADE40_A05E_11D5_9AE7_00600857CEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CParseParameters  
{
public:
	CParseParameters();
	virtual ~CParseParameters();
	int lastNoteDuration;
};

extern CParseParameters parseParameters;

#endif // !defined(AFX_PARSEPARAMETERS_H__015ADE40_A05E_11D5_9AE7_00600857CEE6__INCLUDED_)
