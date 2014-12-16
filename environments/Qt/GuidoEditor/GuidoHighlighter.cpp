/*
 * GuidoHighlighter.cpp
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


#include "GuidoHighlighter.h"

#include <QtDebug>
//#include <execinfo.h> only on unix
#include <iostream>
using namespace std;

#define WHITE				Qt::white
#define TRANSPARENT			Qt::transparent

#define SQUARE_BRACKETS_BACK	TRANSPARENT
#define CURLY_BRACKETS_BACK		TRANSPARENT
#define NOTE_NAMES_BACK		TRANSPARENT
#define ACCIDENTALS_BACK	TRANSPARENT
#define DURATIONS_BACK		TRANSPARENT
#define TAGS_BACK			TRANSPARENT
#define FULL_TAGS_BACK		TRANSPARENT
#define COMMENTS_BACK		TRANSPARENT


#define NOTE QString( "(c|cis|d|dis|e|f|fis|g|gis|a|ais|h|b|do|re|mi|fa|sol|la|si|ti|empty)" )
#define NOTE_PATTERN QString( "((_|" + NOTE + ")\\s*[#|&]*\\s*(-\\d)?\\d*)" )

#define TAG QString("\\\\\\w+(:\\d)?")


//-------------------------------------------------------------------------
void GuidoHighlighter::addRule( int syntaxElement , const QColor& color , const QFont::Weight& weight )
{
	mColors	[syntaxElement] = color;
	mWeights[syntaxElement] = weight;

	switch ( syntaxElement )
	{
		case VOICE_SEPARATOR_ELT:
		{
			//					SPECIAL CHARS
			QStringList squareBracketPattern;
			squareBracketPattern
			<<   "\\["
			<<   "\\]"
			;
			appendLineRule(squareBracketPattern, buildTextCharFormat( color,SQUARE_BRACKETS_BACK,weight ),false);
			break;
		}
		case SCORE_SEPARATOR_ELT:
		{
			QStringList curlyBracketPattern;
			curlyBracketPattern
			<<   "\\{"
			<<   "\\}"
			;
			appendLineRule(curlyBracketPattern, buildTextCharFormat( color,CURLY_BRACKETS_BACK,weight ),false);
			break;
		}
		case DURATION_ELT:
		{
			//                           DURATION
			QStringList durationPatterns;
			durationPatterns
			<<  NOTE_PATTERN + "(\\s*\\*\\s*\\d+)?(\\s*/\\s*\\d+)?"
			;
			appendLineRule(durationPatterns, buildTextCharFormat( color,DURATIONS_BACK,weight ),false);
			break;
		}
		case NOTE_ELT:
		{
			//                              NOTE NAMES
			QStringList noteNamesPatterns;
			noteNamesPatterns
			<< NOTE_PATTERN;
			appendLineRule(noteNamesPatterns, buildTextCharFormat( color,NOTE_NAMES_BACK,weight ),false);
			break;
		}
		case TAG_PARAM_ELT:
		{
			//							TAGS
			QStringList fullTagPattern;
			fullTagPattern << TAG + "(<.*>)";
			appendLineRule( fullTagPattern , buildTextCharFormat( color,FULL_TAGS_BACK,weight ) , true );
			break;
		}
		case TAG_ELT:
		{
			//the tag itself
			QStringList tagsPatterns;
			tagsPatterns << TAG;
			appendLineRule(tagsPatterns, buildTextCharFormat( color,TAGS_BACK,weight ),false);
			break;
		}
		case COMMENT_ELT:
		{
			QStringList commentsPatterns;
			commentsPatterns <<"\\%.*";
			appendLineRule(commentsPatterns, buildTextCharFormat( color,COMMENTS_BACK,weight ),false);

			QString startCommentsPatterns("\\(\\*");
			QString endCommentsPatterns("\\*\\)");
			appendMultilineRule(startCommentsPatterns,endCommentsPatterns,
					buildTextCharFormat( color,COMMENTS_BACK,weight ),true);
			break;
		}
	}
}

/*
//-------------------------------------------------------------------------
void GuidoHighlighter::addRule( int syntaxElement , const QColor& color , const QFont::Weight& weight )
{
	switch ( syntaxElement )
	{
		case VOICE_SEPARATOR_ELT:
			//					SPECIAL CHARS
			QStringList squareBracketPattern;
			squareBracketPattern
			<<   "\\["
			<<   "\\]"
			;
			appendLineRule(squareBracketPattern, buildTextCharFormat( SQUARE_BRACKETS_FONT,SQUARE_BRACKETS_BACK,SQUARE_BRACKETS_WEIGHT ),false);
		break;
		case SCORE_SEPARATOR_ELT:
			QStringList curlyBracketPattern;
			curlyBracketPattern
			<<   "\\{"
			<<   "\\}"
			;
			appendLineRule(curlyBracketPattern, buildTextCharFormat( CURLY_BRACKETS_FONT,CURLY_BRACKETS_BACK,CURLY_BRACKETS_WEIGHT ),false);
		break;
		case DURATION_ELT:
			//                           DURATION
			QStringList durationPatterns;
			durationPatterns
*/
//			<<  NOTE_PATTERN + "(\\s*\\*\\s*\\d+)?(\\s*/\\s*\\d+)?"
/*
			;
			appendLineRule(durationPatterns, buildTextCharFormat( DURATIONS_FONT,DURATIONS_BACK,DURATIONS_WEIGHT ),false);
		break;
		case NOTE_ELT:
			//                              NOTE NAMES
			QStringList noteNamesPatterns;
			noteNamesPatterns
			<< NOTE_PATTERN;
			appendLineRule(noteNamesPatterns, buildTextCharFormat( NOTE_NAMES_FONT,NOTE_NAMES_BACK,NOTE_NAMES_WEIGHT ),false);
		break;
		case TAG_PARAM_ELT:
			//							TAGS
			QStringList fullTagPattern;
			fullTagPattern << TAG + "(<.*>)";
			appendLineRule( fullTagPattern , buildTextCharFormat( FULL_TAGS_FONT,FULL_TAGS_BACK,FULL_TAGS_WEIGHT ) , true );
		break;
		case TAG_ELT:
			//the tag itself
			QStringList tagsPatterns;
			tagsPatterns << TAG;
			appendLineRule(tagsPatterns, buildTextCharFormat( TAGS_FONT,TAGS_BACK,TAGS_WEIGHT ),false);
		break;
		case COMMENT_ELT:
			QStringList commentsPatterns;
			commentsPatterns <<"\\%.*";
			appendLineRule(commentsPatterns, buildTextCharFormat( COMMENTS_FONT,COMMENTS_BACK,COMMENTS_WEIGHT ),false);

			QString startCommentsPatterns("\\(\\*");
			QString endCommentsPatterns("\\*\\)");
			appendMultilineRule(startCommentsPatterns,endCommentsPatterns,
					buildTextCharFormat( COMMENTS_FONT,COMMENTS_BACK,COMMENTS_WEIGHT ),true);
		break;
	}
}
*/

