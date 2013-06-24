/*
 * MusicNotationHighligher.h
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

#ifndef MNHIGHLIGHTER_H_
#define MNHIGHLIGHTER_H_
#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>
#include <QStack>

typedef enum BlockState {
			OUTOFF_BLOCK=0,
			IN_BLOCK
		} BlockState ;

class MusicNotationHighlighter : public QSyntaxHighlighter
{
	public:
		
		MusicNotationHighlighter(QTextDocument *parent = 0);
		virtual ~MusicNotationHighlighter();
		void highlightBlock ( const QString & text );

	protected:

		void appendLineRule(const QStringList& patterns,
							  const QTextCharFormat& format,
							  bool setMinimal);
		void appendMultilineRule(const QString& startPattern,
							 const QString& endPattern,
								  const QTextCharFormat& format,
								  bool setMinimal);
		void highlightLine(const QString& text,bool useLastBackground);
		bool highlightMultiline(const QString& text);
	
		static QTextCharFormat buildTextCharFormat( const QColor& foreground , const QColor& background , QFont::Weight fontWeight );
	
		class HighlightingLineRule
		{
			public:
				QRegExp pattern;
				QTextCharFormat format;
		};
	
		class HighlightingMultilineRule
		{
		
			public:
				
				HighlightingMultilineRule()
				{
					currentState=OUTOFF_BLOCK;
					previousState=OUTOFF_BLOCK;
				};
				
				QRegExp startPattern;
				QRegExp endPattern;
				QTextCharFormat format;
				BlockState currentState;
				BlockState previousState;
		};

		QTextCharFormat mLastFormat;
		HighlightingLineRule mLineRule;
		HighlightingMultilineRule  mMultiLineRule;
		QVector<HighlightingLineRule> mHighlightingLineRules;
		QVector<HighlightingMultilineRule> mHighlightingMultilineRules;
};

#endif /*MNHIGHLIGHTER_H_*/
