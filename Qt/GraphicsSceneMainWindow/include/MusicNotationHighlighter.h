/*
 * MusicNotationHighligher.h
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
