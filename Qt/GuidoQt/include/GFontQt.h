/*
 * GFontQt.h
 *
 * Created by Christophe Daudin on 12/05/09.
 * Copyright 2009 Grame. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.

 * Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 * research@grame.fr
 */
#ifndef GFONTQT_H_
#define GFONTQT_H_

#include "VGFont.h"

#include <QFontInfo>
#include <QString>
#include <QChar>

#include <string>

/**
*	\brief Qt implementation of the VGFont interface.
*
*	More precisely : a wrapper between the VGFont interface and the QtFont object.
*/
class GFontQt : public VGFont
{
public:
			 GFontQt(QFont * nativeFont, const char* faceName, int size, int properties);
	virtual ~GFontQt();
  
	// - VGFont services ---------------------------------------------
	virtual const char* GetName() const			{ return mName.c_str(); }
	virtual int			GetSize() const			{ return mSize; }
	virtual int			GetProperties() const	{ return mFontProp; }

	// - Symbol services ---------------------------------------------
	virtual void GetExtent(const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context = 0) const;
	virtual void GetExtent(unsigned char c, float * outWidth, float * outHeight, VGDevice * context = 0) const;
	  
	/// Returns the  font associated with the current object.
	QFont * GetNativeFont() const { return mNativeFont; }

	/// Returns the symbol corresponding to the input index.
	QChar Symbol(unsigned int sym) const	{ return sym; }

protected:
	QFont * mNativeFont;
	std::string	mName;
	int		mSize;
	int		mFontProp;
};

#endif /*GFONTQT_H_*/
