/*
 * GFontQt.h
 *
 * Created by Christophe Daudin on 12/05/09.
 * Copyright 2009 Grame. All rights reserved.
 *
 * GNU Lesser General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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
