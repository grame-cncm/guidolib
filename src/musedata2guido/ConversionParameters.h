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
// ConversionParameters.h: declaration/interface of class CConversionParameters.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVERSIONPARAMETERS_H__E7BE0C62_9611_11D5_9AE7_00600857CEE6__INCLUDED_)
#define AFX_CONVERSIONPARAMETERS_H__E7BE0C62_9611_11D5_9AE7_00600857CEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConversionParameters  
{
public:
	int numberOfStaves;
	int timeDesignationNum;
	int timeDesignationDen;
	int divisionsPerQuarter;
	CConversionParameters();
	virtual ~CConversionParameters();

};

#endif // !defined(AFX_CONVERSIONPARAMETERS_H__E7BE0C62_9611_11D5_9AE7_00600857CEE6__INCLUDED_)
