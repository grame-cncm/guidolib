/*
 * MusicNotationHighligher.cpp
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
		rx.setMinimal(setMinimal);
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
		int index = text.indexOf(expression);
		while (index >= 0) 
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = text.indexOf(expression, index + length);
		}
	}
}

//-------------------------------------------------------------------------
bool MusicNotationHighlighter::highlightMultiline(const QString& text )
{
	bool result = false;
	for(int i=0;i<mHighlightingMultilineRules.size();i++)
	{
		setCurrentBlockState(OUTOFF_BLOCK);

		int startIndex = 0;
		if (previousBlockState() != IN_BLOCK)
		{
			startIndex = text.indexOf(mHighlightingMultilineRules[i].startPattern);
		}
		else
		{
			result = true;
		}
		
		while (startIndex >= 0) 
		{
			result = true;
			int endIndex = text.indexOf(mHighlightingMultilineRules[i].endPattern, startIndex);

			int commentLength;
			if (endIndex == -1) 
			{
				setCurrentBlockState(IN_BLOCK);
				commentLength = text.length() - startIndex;
			} else 
			{
				commentLength = endIndex - startIndex
						+ mHighlightingMultilineRules[i].endPattern.matchedLength();
			}
			setFormat( startIndex, commentLength, mHighlightingMultilineRules[i].format );
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
