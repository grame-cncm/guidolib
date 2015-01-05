/*
 * GuidoHighlighter.h
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

#ifndef GUIDOHIGHLIGHTER_H_
#define GUIDOHIGHLIGHTER_H_

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>
#include <QStack>
#include <QMap>

#include "MusicNotationHighlighter.h"

class GuidoHighlighter:public MusicNotationHighlighter
{

	QMap<int , QColor>	mColors;
	QMap<int , int>		mWeights;

public:

	enum { 
		VOICE_SEPARATOR_ELT = 0,
		SCORE_SEPARATOR_ELT,
		DURATION_ELT,
		NOTE_ELT,
		TAG_PARAM_ELT,
		TAG_ELT,
		COMMENT_ELT,
		SIZE
	 } SyntaxElement;

	GuidoHighlighter(QTextDocument *parent = 0) : MusicNotationHighlighter(parent) {}
	
	void addRule( int syntaxElement , const QColor& color , const QFont::Weight& weight );
	
	const QColor&	color(int syntaxElement)	{ return mColors[syntaxElement]; }
	int				weight(int syntaxElement)	{ return mWeights[syntaxElement]; }
	
};

#endif /*GUIDOHIGHLIGHTER_H_*/
