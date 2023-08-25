/*
 * MusicNotationHighligher.cpp
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

#include "MusicNotationHighlighter.h"

//-------------------------------------------------------------------------
void MusicNotationHighlighter::appendLineRule(const QStringList& patterns,
		  const QTextCharFormat& format,
		  bool setMinimal=false)
{
	mLastFormat = format;
	foreach (QString pattern, patterns) 
	{
		QRegExp rx=QRegExp(pattern);
#if Qt6
		if (setMinimal) rx.setPatternOptions(QRegularExpression::InvertedGreedinessOption);
#else
		rx.setMinimal(setMinimal);
#endif
		mLineRule.pattern = rx;
		mLineRule.format = format;
		mHighlightingLineRules.append(mLineRule);
	}
}

//-------------------------------------------------------------------------
void MusicNotationHighlighter::appendMultilineRule(const QString& startPattern,
	const QString& endPattern, const QTextCharFormat& format, 
	bool)
{
	mLastFormat = format;
	
	QRegExp srx=QRegExp(startPattern);
	QRegExp erx=QRegExp(endPattern);
	mMultiLineRule.startPattern = srx;
	mMultiLineRule.endPattern=erx;
	mMultiLineRule.format = format;
	mHighlightingMultilineRules.append(mMultiLineRule);						  	
}

//-------------------------------------------------------------------------
MusicNotationHighlighter::MusicNotationHighlighter(QTextDocument *parent)
	:QSyntaxHighlighter(parent)
{}

//-------------------------------------------------------------------------
MusicNotationHighlighter::~MusicNotationHighlighter()
{}

//-------------------------------------------------------------------------
void MusicNotationHighlighter::highlightLine(const QString& text,bool )
{
	foreach (HighlightingLineRule rule, mHighlightingLineRules)
	{
		QRegExp expression(rule.pattern);
#ifdef Qt6
		QRegularExpressionMatch match;
		qsizetype index = text.indexOf(expression, 0, &match);
		while (index >= 0)
		{
			qsizetype length = match.capturedLength();
			setFormat((int)index, (int)length, rule.format);
			index = text.indexOf(expression, index + length, &match);
		}
#else
		int index = text.indexOf(expression);
		while (index >= 0) 
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = text.indexOf(expression, index + length);
		}
#endif
	}
}

//-------------------------------------------------------------------------
bool MusicNotationHighlighter::highlightMultiline(const QString& text )
{
	bool result = false;
	for(int i=0;i<mHighlightingMultilineRules.size();i++)
	{
		setCurrentBlockState(OUTOFF_BLOCK);

		qsizetype startIndex = 0;
		if (previousBlockState() != IN_BLOCK)
			startIndex = text.indexOf(mHighlightingMultilineRules[i].startPattern);
		else result = true;
		
		while (startIndex >= 0) 
		{
			result = true;
#ifdef Qt6
			QRegularExpressionMatch endMatch;
			qsizetype endIndex = text.indexOf(mHighlightingMultilineRules[i].endPattern, startIndex, &endMatch);
#else
			int endIndex = text.indexOf(mHighlightingMultilineRules[i].endPattern, startIndex);
#endif
			qsizetype commentLength;
			if (endIndex == -1) {
				setCurrentBlockState(IN_BLOCK);
				commentLength = text.length() - startIndex;
			}
			else {
#ifdef Qt6
				commentLength = endIndex - startIndex + endMatch.capturedLength();
#else
				commentLength = endIndex - startIndex + mHighlightingMultilineRules[i].endPattern.matchedLength();
#endif
			}
			setFormat( (int)startIndex, (int)commentLength, mHighlightingMultilineRules[i].format );
			startIndex = text.indexOf(mHighlightingMultilineRules[i].startPattern, startIndex + commentLength);
		}
	}
	return result;
}

#include <QtDebug>
//-------------------------------------------------------------------------
void MusicNotationHighlighter::highlightBlock ( const QString & text )
{
	highlightLine(text,false);
	highlightMultiline(text);
}

//-------------------------------------------------------------------------
QTextCharFormat MusicNotationHighlighter::buildTextCharFormat( const QColor& foreground , const QColor& background , QFont::Weight fontWeight )
{
	QTextCharFormat result;
	result.setForeground( foreground );
	result.setBackground( background );
	result.setFontWeight( fontWeight );
	return result;
}
