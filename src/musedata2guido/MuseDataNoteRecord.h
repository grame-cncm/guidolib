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
// MuseDataNoteRecord.h: declaration/interface of class CMuseDataNoteRecord.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSEDATANOTERECORD_H__A5A9C525_8A6E_11D5_9AE7_00600857CEE6__INCLUDED_)
#define AFX_MUSEDATANOTERECORD_H__A5A9C525_8A6E_11D5_9AE7_00600857CEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MuseDataDataRecord.h"

enum ENoteType{
	nt_normal=0,
	nt_cue,
	nt_grace
};

enum EGraceCueType{
	gct_8th_with_slash='0',
	gct_256th='1',
	gct_128th='2',
	gct_64th='3',
	gct_32th='4',
	gct_16th='5',
	gct_8th='6',
	gct_quarter='7',
	gct_half='8',
	gct_whole='9',
	gct_breve='A'
};

enum EPitch{
	p_a='A',
	p_b='B',
	p_c='C',
	p_d='D',
	p_e='E',
	p_f='F',
	p_g='G',
	p_rest='r'
};

enum EGraphicNoteType{
	gnt_unknown=' ',
	gnt_fs_long='L',
	gnt_fs_breve='b',
	gnt_fs_whole='w',
	gnt_fs_half='h',
	gnt_fs_quarter='q',
	gnt_fs_eighth='e',
	gnt_fs_sixteenth='s',
	gnt_fs_32nd='t',
	gnt_fs_64th='x',
	gnt_fs_128th='y',
	gnt_fs_256th='z',
	gnt_cs_long='B',
	gnt_cs_breve='A',
	gnt_cs_whole='9',
	gnt_cs_half='8',
	gnt_cs_quarter='7',
	gnt_cs_eighth='6',
	gnt_cs_sixteenth='5',
	gnt_cs_32nd='4',
	gnt_cs_64th='3',
	gnt_cs_128th='2',
	gnt_cs_256th='1'
};

enum ENotatedAccidental{
	na_none=' ',
	na_sharp='#',
	na_natural='n',
	na_flat='f',
	na_doublesharp='x',
	na_sharpsharp='X',
	na_flatflat='&',
	na_naturalsharp='S',
	na_naturalflat='F'
};

enum EStemDirection{
	sd_down='d',
	sd_up='u',
	sd_nostem=' '
};

enum EBeamCode{
	bc_nobeam=' ',
	bc_start='[',
	bc_continue='=',
	bc_end=']',
	bc_forwardhook='/',
	bc_backwardhook='\\'
};

enum EChordInfo{
	ci_NoChord=0,
	ci_Begin,
	ci_Part,
	ci_End
};

enum EOrnaments{
	orn_tr=0,
	orn_turn,
	orn_delayed_turn,
	orn_shake,
	orn_wavy_line,
	orn_continue_wavy_line,
	orn_mordent,
	orn_slide,
	orn_Count
};

enum ERange{
	range_NoPart=0,
	range_Begin,
	range_Part,
	range_End,
	range_Single
};


class CMuseDataNoteRecord : public CMuseDataDataRecord  
{
public:
	int getidBeamDrawwInfo();
	void setidBeamDrawInfo(int d);
	int getidBeamChange();
	int getidBeamCount();
	void setidChordLabel(int l);
	int getidChordDuration();
	void setidChordDuration(int d);
	int idChordDuration;
	ERange getidChordInfo();
	CMuseDataNoteRecord* getidNextInChord();
	void setidNextInChord(CMuseDataNoteRecord *);
	ENoteType getrdNoteType();
	void setidGraceNote(ERange);
	void setidCueNote(ERange);
	int printGuidoOn(FILE *,CConversionParameters*);
	void setidSlurID(int no,int id);
	void setidTieEnd(int i);
	void setidTieBegin(int i);
	int isTiedToNext();
	int getrdPitchCode();
	int getrdEndSlur(int i);
	int getrdBeginSlur(int i);
	int getrdChordFlag();
	void setidChordInfo(ERange c);
	int getrdDuration();
	virtual void printOn(FILE*);
	virtual int parseFromRecord(char *line);
	CMuseDataNoteRecord();
	virtual ~CMuseDataNoteRecord();

protected:
	int idBeamDrawInfo;
	int idChordLabel;
	//sound-information (from col 1 to 9 of file)
	EPitch rdPitchName;
	int rdPitchChromatic;
	int rdPitchOctave;
	int rdDuration;
	ENoteType rdNoteType;
	EGraceCueType rdGraceCueType;
	int rdFlagTied;
	int rdFlagChord;

	//graphical and interpretativ information (from col 13 to 42)
	EGraphicNoteType rdGraphicNoteType;
	int rdDotsOfProlongation;
	ENotatedAccidental rdActualNotatedAccidentals;
	EStemDirection rdStemDirection;
	int rdStaffAssignment;
	EBeamCode rdBeamcode[6];
 	int rdTie;
	int rdBeginTuplet;
	int rdEndTuplet;
	int rdBeginSlur[4];
	int rdEndSlur[4];
	int rdOrnaments[orn_Count];

	//implicit information computed from file
	ERange idChord;
	CMuseDataNoteRecord* idNextInChord;


	int idBeamCount;
	int idBeamChange;
	int idSlurID[4];
	int idTieBegin;
	int idTieEnd;
	ERange idCueNote;
	ERange idGraceNote;
};

typedef CMuseDataNoteRecord* pMuseDataNoteRecord;

#endif // !defined(AFX_MUSEDATANOTERECORD_H__A5A9C525_8A6E_11D5_9AE7_00600857CEE6__INCLUDED_)
