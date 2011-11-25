/*
GUIDO Library
Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.
	
	  You should have received a copy of the GNU Lesser General Public
	  License along with this library; if not, write to the Free Software
	  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
	  
*/
/*
	lyric-tag support for finale2guido Plug-In
	created 2003 by Juergen Kilian
	ToDo:
	- add measureText and pageText support
*/

#include "lyrics.h"
#include "finextnd.h"
#include <stdio.h>

char lyricsTag( EXENTRY pent_i,	// current finale object
			   char *dxStr,		// str for dx in \lyric
			   char *fSizeStr,	// str for fontsize in \lyric
			   char *buffer)	// buffer for return str
/*
  check for lyrics linked to pent_i and copy text to buffer!
  result:
	1 : if lyrics found
	0 : otherwhise
  The range of the \lyric-tag needs to closed later!

  Todo:
  check lentgh(lyrics) and buffer length!!
			   
				 */
{
	EDataID id;
	EDTLyric	lyric;
	fourbyte	lyricSize = sizeof( EDTLyric );
	
	id.entryDetail.entryNumber = pent_i.entnum;
	id.entryDetail.inci = 0;
	
	EXTAG curExTag = 0;
	
	char addLyrics = 0;
	char syllStr[256];
	
	if (pent_i.ef & EF_LYRICDETAIL) // exist any  lyrics for current entry?
	{								
		addLyrics = 1;

		// look for lyric type
		if (FX_LoadEData(ed_VerseLyric,&id,&lyric,&lyricSize))
		{
			curExTag = ed_VerseLyric;
		}
		else if(FX_LoadEData(ed_ChorusLyric,&id,&lyric,&lyricSize))
		{
			curExTag = ed_ChorusLyric;
		}
		else if(FX_LoadEData(ed_SectionLyric,&id,&lyric,&lyricSize))
		{
			curExTag = ed_SectionLyric;
		}
		else
		{
			sprintf(buffer," (* unknown lyrictype*) \\lyrics<\"\">( " );
			/*				
				pageText: ot_PageText, EDTMeasureText
				measureText: dt_MeasureTextBlock; EDTPageText


			*/
			
		}
								
								
		if( curExTag ) // anything found?
		{
			// now retrieve lyric text
			twobyte actLen = 0;
			tbool gotSyll = FX_SyllableInfo( curExTag, lyric.rawTextNum, lyric.syll,
				syllStr, sizeof(syllStr), &actLen, NULL, NULL, NULL );
			
			// write GUIDO lyric tag			
			//{ [ \lyrics<"This is it: Lyr-ics!",dy=4hs,fsize=20pt>(c  d e f g ) a  ] ,
			sprintf(buffer,"\\lyrics<\"%s\", dy=%shs, fsize=%spt>( ",
					syllStr, dxStr, fSizeStr); 
		}				
								
	} // if lyrics attached to pent_i
	
	return addLyrics;
} //lyricsTag 