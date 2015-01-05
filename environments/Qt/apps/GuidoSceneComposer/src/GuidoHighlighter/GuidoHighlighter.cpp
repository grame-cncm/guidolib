/*
 * GuidoHighlighter.cpp
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


#include "GuidoHighlighter.h"
//#include <execinfo.h> only on unix
#include <iostream>
using namespace std;

#define WHITE				Qt::white
#define TRANSPARENT			Qt::transparent

#define VIOLET				QColor(72,66,139)
#define LIGHT_YELLOW		QColor(250,250,210)
#define LIGHT_GREEN			QColor(240,255,240)
#define FLORAL_GREEN		QColor(255,250,240)
#define DARK_OLIVE_GREEN	QColor(85,107,47)
#define DODGER_BLUE_4		QColor(16,78,139)
#define INDIAN_RED_1		QColor(255,106,106)
#define INDIAN_RED_2		QColor(190,84,83)
#define DARK_SEA_GREEN_1	QColor(193,255,193)
#define DARK_SEA_BLUE_1		QColor(193,193,255)
#define LAVENDER_BLUSH		QColor(255,240,245)
#define GREEN_4				QColor(0,139,0)
#define ORANGE_1			QColor(240,110,36)
#define ORANGE_2			QColor(209,110,36)
#define DEEP_BLUE			QColor(31,68,110)
#define DEEP_TURQUOISE		QColor(31,109,110)
#define DEEP_PURPLE			QColor( 70 , 0 , 83 )

#define SQUARE_BRACKETS_FONT	DEEP_PURPLE
#define SQUARE_BRACKETS_BACK	TRANSPARENT
#define SQUARE_BRACKETS_WEIGHT	QFont::Black

#define CURLY_BRACKETS_FONT		DEEP_PURPLE
#define CURLY_BRACKETS_BACK		TRANSPARENT
#define CURLY_BRACKETS_WEIGHT	QFont::Black

#define NOTE_NAMES_FONT		DEEP_BLUE
#define NOTE_NAMES_BACK		TRANSPARENT
#define NOTE_NAMES_WEIGHT	QFont::Bold

#define ACCIDENTALS_FONT	DEEP_BLUE
#define ACCIDENTALS_BACK	TRANSPARENT
#define ACCIDENTALS_WEIGHT	QFont::Bold

#define DURATIONS_FONT		INDIAN_RED_2
#define DURATIONS_BACK		TRANSPARENT
#define DURATIONS_WEIGHT	QFont::Bold

#define TAGS_FONT			ORANGE_2
#define TAGS_BACK			TRANSPARENT
#define TAGS_WEIGHT			QFont::Bold

#define FULL_TAGS_FONT		Qt::black
#define FULL_TAGS_BACK		TRANSPARENT
#define FULL_TAGS_WEIGHT	QFont::Bold

#define COMMENTS_FONT		GREEN_4
#define COMMENTS_BACK		TRANSPARENT
#define COMMENTS_WEIGHT		QFont::Normal

#define NOTE QString( "(c|cis|d|dis|e|f|fis|g|gis|a|ais|h|b|do|re|mi|fa|sol|la|si|ti|empty)" )
#define NOTE_PATTERN QString( "((_|" + NOTE + ")\\s*[#|&]*\\s*(-\\d)?\\d*)" )

#define TAG QString("\\\\\\w+(:\\d)?")
//-------------------------------------------------------------------------
GuidoHighlighter::GuidoHighlighter(QTextDocument *parent)
	:MusicNotationHighlighter(parent)
{	
	//					SPECIAL CHARS
	QStringList squareBracketPattern;
	squareBracketPattern
	<<   "\\["
	<<   "\\]"
	;
	appendLineRule(squareBracketPattern, buildTextCharFormat( SQUARE_BRACKETS_FONT,SQUARE_BRACKETS_BACK,SQUARE_BRACKETS_WEIGHT ),false);

	QStringList curlyBracketPattern;
	curlyBracketPattern
	<<   "\\{"
	<<   "\\}"
	;
	appendLineRule(curlyBracketPattern, buildTextCharFormat( CURLY_BRACKETS_FONT,CURLY_BRACKETS_BACK,CURLY_BRACKETS_WEIGHT ),false);

	//                           DURATION
	QStringList durationPatterns;
	durationPatterns
	<<  NOTE_PATTERN + "(\\s*\\*\\s*\\d+)?(\\s*/\\s*\\d+)?"
	;
	appendLineRule(durationPatterns, buildTextCharFormat( DURATIONS_FONT,DURATIONS_BACK,DURATIONS_WEIGHT ),false);

	//                              NOTE NAMES
	QStringList noteNamesPatterns;
	noteNamesPatterns
	<< NOTE_PATTERN;
	appendLineRule(noteNamesPatterns, buildTextCharFormat( NOTE_NAMES_FONT,NOTE_NAMES_BACK,NOTE_NAMES_WEIGHT ),false);

	//							TAGS
	QStringList fullTagPattern;
	fullTagPattern << TAG + "(<.*>)";
	appendLineRule( fullTagPattern , buildTextCharFormat( FULL_TAGS_FONT,FULL_TAGS_BACK,FULL_TAGS_WEIGHT ) , true );

	//the tag itself
	QStringList tagsPatterns;
	tagsPatterns << TAG;
	appendLineRule(tagsPatterns, buildTextCharFormat( TAGS_FONT,TAGS_BACK,TAGS_WEIGHT ),false);

	QStringList commentsPatterns;
	commentsPatterns <<"\\%.*";
	appendLineRule(commentsPatterns, buildTextCharFormat( COMMENTS_FONT,COMMENTS_BACK,COMMENTS_WEIGHT ),false);

	QString startCommentsPatterns("\\(\\*");
	QString endCommentsPatterns("\\*\\)");
	appendMultilineRule(startCommentsPatterns,endCommentsPatterns,
			buildTextCharFormat( COMMENTS_FONT,COMMENTS_BACK,COMMENTS_WEIGHT ),true);
}
