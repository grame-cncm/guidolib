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

// f2guido.cpp
// main source file for the Export to GUIDO Music Notation Finale plugin
// last modified Jan 2003 by Juergen Kilian
// output file: finale2guido.fxt !! fxt is default postfix for finale plugins!

/* Remarks by jk
-The output file must be renamend into f2guido.FXT and cpoied to 
the finale/Plug-In directory.
-ATTENTION: The struct-byte alignment of your compiler MUST be set to 1 byte!
otherwhise the passed parameters from finale will be incorrect
-To create the f2guido plugin some files of the finale-Plugin-SDK are needed
The SDK can be downloaded from the coda music website 
-See also http://www.noteserver.org for news on GUIDO MusicNotation

  History:
  - before 2003: first version implemented by Tobias Mann
  - 12 Jan. 2003: more readable code and 
			     impl. of \lyrics suppor by J. Kilian kilian@salieri.org
				 sometimes there are problems with lyrics at beginning of beams or with empty notes
  - 17  Jan 2003 added lyrics support on emtpy notes. jk

*/

/* Remarks by jk
	-The output file must be renamend into f2guido.FXT and cpoied to 
	 the finale/Plug-In directory.
	-ATTENTION: The struct-byte alignment of your compiler MUST be set to 1 byte!
	 otherwhise the passed parameters from finale will be incorrect
	-To create the f2guido plugin some files of the finale-Plugin-SDK are needed
	 The SDK can be downloaded from the coda music website 
	-See also http://www.noteserver.org for news on GUIDO MusicNotation

*/
#ifdef WIN32
#include "stdafx.h"
#endif
#include "resource.h"

#include "eeddata.h"
#define VERSION "0.9"

#include "lyrics.h"
#include <string.h>
#include <stdio.h>

PUBLIC FX_RETURNVAL FINEXPORT f2gDHandler(EWND hDlg, ufourbyte msg,
										  WPARAM wParam, LPARAM lParam, void *);
PRIVATE void LOCAL readMusic(CONST EWND hWnd, EREGION * reg, const char * fileName);

// set default options
int gmnFormat=111;
int oldGmnFormat=111;


// buffers for lyrics settings
char fSizeStr[12] = "12",
     dxStr[12] ="0";


BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}

tbool FINEXPORT FinaleExtensionEnumerate(CONST EWND /*hWnd*/, CONST twobyte index,
										 FXT_MENUSTRING_PACKET *buf) {
	if (!index) {
		strncpy(buf->menuItem, "Export to GUIDO...", MAX_MSGSTR_LEN);
		strncpy(buf->msgString, "Export to GUIDO", MAX_MSGSTR_LEN);
		strncpy(buf->undoString, "Export to GUIDO", MAX_UNDOSTR_LEN);
		buf->needSelectedRegion=0;
		buf->useGetMenuStatus=0;
		return YES;
	}
	return NO;
}

tbool FINEXPORT FinaleExtensionGetMenuStatus(CONST twobyte /*index*/) {return YES;}

tbool FINEXPORT FinaleExtensionInit(CONST EWND /*hWnd*/, FLAG_32 *flags) {
	*flags=0; return YES;
}

PRIVATE void LOCAL getDefaultGmnFilename(CONST EWND hWnd, char * str, int maxlen) {
	char *l, *r, *d;
	if (GetWindowText(hWnd,str,maxlen)<=0) {
		strcpy(str,"*.gmn"); return;
	}
	l=strchr(str,'['); r=strchr(str,']');
	if ((l==NULL)||(r==NULL)) {
		strcpy(str,"*.gmn"); return;
	}
	for (d=r; (*d!='.')&&(d!=l); --d);
	if (d==l) *r=0; else *d=0;
	++l; memmove(str,l,strlen(l)+1);
	if (strlen(str)<maxlen-4) strcpy(str+strlen(str),".gmn");
}

tbool FINEXPORT FinaleExtensionInvoke(CONST EWND hWnd, CONST twobyte index,
                                      EREGION *pSelRegion, EREGION *pDocRegion) {

	// set defualt fsize, dy
	strcpy(fSizeStr,"12");
	strcpy(dxStr,"0");


	FF_DialogHandlerUPP dh = NewFF_DialogHandlerProc(f2gDHandler);
	HWND dlg=FX_Dialog(IDD_DIALOG1, dh, hWnd, DIALOG_MODAL, NULL);
	if ((gmnFormat&1)==0) return NO;
	
	OPENFILENAME fnstruct;
	char str[260];
	
	fnstruct.hwndOwner=hWnd;
	getDefaultGmnFilename(hWnd,str,255);
	fnstruct.lpstrFilter="*.gmn";
	fnstruct.lpstrCustomFilter=NULL;
	fnstruct.nFilterIndex=0;
	fnstruct.lpstrFile=str;
	fnstruct.nMaxFile=256;
	fnstruct.lpstrFileTitle=NULL;
	fnstruct.lpstrInitialDir=NULL;
	fnstruct.lpstrTitle="Export to GUIDO Music Notation";
	fnstruct.Flags=OFN_HIDEREADONLY|OFN_LONGNAMES|OFN_NOREADONLYRETURN
		|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST;
	fnstruct.lpstrDefExt="gmn";
	fnstruct.lStructSize=sizeof(fnstruct);
	
	if (!GetSaveFileName(&fnstruct)) return NO;
	
	if (pSelRegion) readMusic(hWnd, pSelRegion, str); else readMusic(hWnd, pDocRegion, str);
	return NO; // no refresh
}

void FINEXPORT FinaleExtensionTerminate(CONST EWND /*hWnd*/) {;}

ELONG FINEXPORT FinaleExtensionVersion(CONST ELONG /*hiVersion*/) {return FXT_VERSION;}

////////////////////////////////////////////////////////////////////////////////////////

class writeBufferClass {
	static int count; // of set marks
public:
	char * s;
	writeBufferClass * n;
	int m; // mark
	
	writeBufferClass(void) {s=NULL; n=NULL; m=-1;}
	~writeBufferClass(void) {if (n) delete n; if (s) delete s;}
	
	void add(const char * str,int len) {
		if (s&&!n) {n=new writeBufferClass;}
		if (n) {n->add(str,len); return;}
		s=new char[len+1]; strcpy(s,str);
	}
	void removeMark(int mark) {
		if (mark<0) {
			m=-1; count=0; if (n) n->removeMark(mark);
		} else {
			if (m==mark) {
				m=-1; --count;
			} else {
				if (n) n->removeMark(mark);
			}
		}
	}
	
	void flush(FILE * f, int mark=-1) {
		writeBufferClass * i;
		removeMark(mark);
		while (m<0) {
			if (s) {fwrite(s,strlen(s),1,f); delete s; s=NULL;}
			if (n) {i=n; s=n->s; m=n->m; n=n->n; i->n=NULL; i->s=NULL; delete i;}
			else break;
		}
	}
	void write(FILE * f, const char * str,int len) {
		if (!count) {
			if (s||n) this->flush(f,-1);
			fwrite(str,len,1,f);
		} else add(str,len);
	}
	void mark(int mrk) {
		if (s&&!n) {n=new writeBufferClass;}
		if (n) {n->mark(mrk); return;}
		if (m>=0) {n=new writeBufferClass; n->mark(mrk); return;}
		if (mrk<0) mrk=0;
		m=mrk; ++count;
	}
	void insert(int mark, const char * str, int len) {
		writeBufferClass * i;
		if (m!=mark) {
			if (n) n->insert(mark,str,len);
			return;
		}
		i=n; n=new writeBufferClass; n->n=i; n->m=m; n->s=s;
		s=new char[len+1]; strcpy(s,str); m=-1;
	}
} w2g;
int writeBufferClass::count=0;

////////////////////////////////////////////////////////////////////////////////////////

class Storage {
public:
	twobyte meas, inx;
	ENTNUM en;
	Storage * next;
	int usedFlag;
	
	Storage(void) {next=NULL; usedFlag=0;}
	~Storage(void) {if (next) delete next;}
	int stored(ENTNUM n, twobyte m, twobyte i) {
		if ((n==en)&&(meas==m)&&(inx==i)&&(!usedFlag)) {usedFlag=1; return 1;}
		if (!next) return 0;
		return next->stored(n,m,i);
	}
	int storageDone(void) {
		if (!usedFlag) return 0;
		if (!next) return 1;
		return next->storageDone();
	}
};

PRIVATE void LOCAL store(EXTGF * p, twobyte m, twobyte i, Storage ** storage) {
	Storage * st=new Storage;
	
	st->meas=m; st->en=p->pent[i].entnum; st->inx=i; st->next=*storage;
	*storage=st;
}

PRIVATE int LOCAL stored(EXTGF * p, twobyte m, twobyte i, Storage * storage) {
	if (!storage) return 0;
	return storage->stored(p->pent[i].entnum,m,i);
}

PRIVATE int LOCAL storageDone(Storage * storage) {
	if (!storage) return 1;
	return storage->storageDone();
}

////////////////////////////////////////////////////////////////////////////////////////

PRIVATE int LOCAL keyOffset(int sg) {
	int type=(sg>>8), r;
	if ((type!=0)&&(type!=1)) {/*dbgOut("Unknown Key Signature");*/ return 0;}
	
	sg&=0xff; if (sg&0x80) sg=sg-256;
	r=4*sg; r+=700;
	if (type) r-=2;
	r%=7;
	
	return r;
}

PRIVATE void LOCAL toneName(int v, int sg, int mv, FLAG_32 flags, char * s) {
	int o,l,exm; char c;
	
	if ((flags&0x40000000)==0) {
		strcpy(s,"_"); return;
	}
	
	v+=keyOffset(sg);
	o=(v+2100)/7, l=(v+2100)%7;
	
	switch (l) {
	case 0: c='c'; exm=60; break;
	case 1: c='d'; exm=62; break;
	case 2: c='e'; exm=64; break;
	case 3: c='f'; exm=65; break;
	case 4: c='g'; exm=67; break;
	case 5: c='a'; exm=69; break;
	case 6: c='b'; exm=71; break;
		//default: dbgOut("internal error 1");
	}
	
	o-=299;
	sprintf(s,"%c",c);
	
	exm+=12*(o-1);
	while (mv<exm) {sprintf(s+strlen(s),"&"); --exm;}
	while (mv>exm) {sprintf(s+strlen(s),"#"); ++exm;}
	
	sprintf(s+strlen(s),"%i",o);
}

PRIVATE void LOCAL ksName(int sg, char * s) {
	
	strcpy(s,"\\key<\"");
	
	switch (sg) {
	case 0x00: strcpy(s+strlen(s),"C"); break; case 0x01: strcpy(s+strlen(s),"G"); break;
	case 0x02: strcpy(s+strlen(s),"D"); break; case 0x03: strcpy(s+strlen(s),"A"); break;
	case 0x04: strcpy(s+strlen(s),"E"); break; case 0x05: strcpy(s+strlen(s),"B"); break;
	case 0x06: strcpy(s+strlen(s),"F#"); break; case 0x07: strcpy(s+strlen(s),"C#"); break;
	case 0x08: strcpy(s+strlen(s),"G#"); break; case 0x09: strcpy(s+strlen(s),"D#"); break;
	case 0x0A: strcpy(s+strlen(s),"A#"); break; case 0x0B: strcpy(s+strlen(s),"E#"); break;
	case 0x0C: strcpy(s+strlen(s),"B#"); break; case 0xFF: strcpy(s+strlen(s),"F"); break;
	case 0xFE: strcpy(s+strlen(s),"B&"); break; case 0xFD: strcpy(s+strlen(s),"E&"); break;
	case 0xFC: strcpy(s+strlen(s),"A&"); break; case 0xFB: strcpy(s+strlen(s),"D&"); break;
	case 0xFA: strcpy(s+strlen(s),"G&"); break; case 0xF9: strcpy(s+strlen(s),"C&"); break;
	case 0xF8: strcpy(s+strlen(s),"F&"); break; case 0xF7: strcpy(s+strlen(s),"B&&"); break;
	case 0xF6: strcpy(s+strlen(s),"E&&"); break; case 0xF5: strcpy(s+strlen(s),"A&&"); break;
	case 0xF4: strcpy(s+strlen(s),"D&&"); break;
		
	case 0x100: strcpy(s+strlen(s),"a"); break; case 0x101: strcpy(s+strlen(s),"e"); break;
	case 0x102: strcpy(s+strlen(s),"b"); break; case 0x103: strcpy(s+strlen(s),"f#"); break;
	case 0x104: strcpy(s+strlen(s),"c#"); break; case 0x105: strcpy(s+strlen(s),"g#"); break;
	case 0x106: strcpy(s+strlen(s),"d#"); break; case 0x107: strcpy(s+strlen(s),"a#"); break;
	case 0x108: strcpy(s+strlen(s),"e#"); break; case 0x109: strcpy(s+strlen(s),"b#"); break;
	case 0x10A: strcpy(s+strlen(s),"f##"); break; case 0x10B: strcpy(s+strlen(s),"c##"); break;
	case 0x10C: strcpy(s+strlen(s),"g##"); break; case 0x1FF: strcpy(s+strlen(s),"d"); break;
	case 0x1FE: strcpy(s+strlen(s),"g"); break; case 0x1FD: strcpy(s+strlen(s),"c"); break;
	case 0x1FC: strcpy(s+strlen(s),"f"); break; case 0x1FB: strcpy(s+strlen(s),"b&"); break;
	case 0x1FA: strcpy(s+strlen(s),"e&"); break; case 0x1F9: strcpy(s+strlen(s),"a&"); break;
	case 0x1F8: strcpy(s+strlen(s),"d&"); break; case 0x1F7: strcpy(s+strlen(s),"g&"); break;
	case 0x1F6: strcpy(s+strlen(s),"c&"); break; case 0x1F5: strcpy(s+strlen(s),"f&"); break;
	case 0x1F4: strcpy(s+strlen(s),"b&&"); break;
		
	default: *s=0; return;
	}
	
	strcpy(s+strlen(s),"\"> ");
}

PRIVATE void LOCAL durName(int sd, int ad, char * s) {
	int a,b;
	
	for (b=1; (sd%(2*b)==0)&&(b<4096); b*=2);
	a=sd/b;
	b=4096/b;
	
	if (ad!=sd) {
		if ((abs(2*sd/3-ad)<2)&&(b%2==0)) {
			b=3*b/2;
		} else { if ((abs(4*sd/5-ad)<2)&&(b%4==0)) {
			b=5*b/4;
		} else {
			a=ad; b=4096;
		}}
	}
	
	if (b==1) {
		sprintf(s,"*%i",a);
	} else { if (a==1) {
		sprintf(s,"/%i",b);
	} else {
		sprintf(s,"*%i/%i",a,b);
	}}
	
}

PRIVATE void LOCAL getTimeSig(int meas, int * a, int * b) {
	EDataID dataID;
	EDTMeasureSpec mspec;
	long mspecSize=sizeof(EDTMeasureSpec);
	int i;
	
	dataID.other.cmper=meas;
	dataID.other.inci=0;
	FX_LoadEData(ot_MeasureSpec, &dataID, &mspec, &mspecSize);
	
	i=1; while ((mspec.divbeat%(2*i)==0)&&(i<4096)) i*=2;
	*b=4096/i;
	*a=mspec.beats*mspec.divbeat/i;
}

PRIVATE void LOCAL accumulateTS(int ts_a, int ts_b, int * acctsa, int * acctsb) {
	if (ts_b==*acctsb) 
	{
		*acctsa+=ts_a;
	} 
	else 
	{ 
		if (*acctsb%ts_b==0) 
		{
			*acctsa+=ts_a*(*acctsb)/ts_b;
		} 
		else 
		{
			ts_a*=*acctsb;
			*acctsb*=ts_b;
			*acctsa*=ts_b;
			*acctsa+=ts_a;
		}
	}
}

PRIVATE int LOCAL underfullMeasure(int eed, int tsa, int tsb, int lps, char * str) {
	int md;
	
	if (4096%tsb!=0) return 0; // unknown time signature
	md=tsa*(4096/tsb);
	
	if (eed<md) 
	{
		if (lps==1) 
			strcpy(str,"_"); 
		else strcpy(str,"empty");
		durName(md-eed,md-eed,str+strlen(str));
		sprintf(str+strlen(str)," ");
		return 1;
	}
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////

#define LIFSET(a) if ((gmnFormat&(a))!=0) {
#define LIFNOT(a) if ((gmnFormat&(a))==0) {
#define LEND      }

// marks
#define M_BEAM 1

// options
#define F_BEAM 2
#define F_COMMENT 4
#define F_STAFF 8
#define F_DEBUG 16
#define F_CR 32
#define F_LYRICS 64

PRIVATE void LOCAL readMusic(CONST EWND hWnd, EREGION * reg, const char * fileName) {
	TIME_EDU selStart, selEnd;
	twobyte meas, slot, layer, i, j;
	EXTGF * p;
	char str[1023];
	int chFlag, openSeq, newOpenSeq, keySig=0, oldKeySig, showKsFlag, seqCounter;
	int dirtyFlag, staffNumber, layerPerStaff, ts_a=0, ts_b=0, otsa, otsb;
	int showTsFlag, expEllDur, storageFlag=0, acctsa, acctsb, graceFlag=0;
	int midiv, possibleBeam, beam2;
	Storage * storage=NULL;
	FILE * f;
	
	f=fopen(fileName,"wb");
	if (!f) 
	{
		MessageBox(hWnd, "Couldn't open file for writing.", "Error",
			MB_ICONERROR|MB_OK);
		return;
	}
	
	oldGmnFormat=gmnFormat;
	
	LIFSET(F_COMMENT)
		sprintf(str,"(* Created by Export to GUIDO Music Notation plugin, v%4s *)\r\n", VERSION);
		w2g.write(f,str,strlen(str));
		sprintf(str,"(* For more information see http://www.salieri.org         *)\r\n");
		w2g.write(f,str,strlen(str));
		LIFSET(F_DEBUG)
			sprintf(str,"(* Option code for export is: %2i                           *)\r\n", gmnFormat);
			w2g.write(f,str,strlen(str));
		LEND
		w2g.write(f,"\r\n",2);
	LEND
		
		
	w2g.write(f,"{",1);
	seqCounter=0; staffNumber=1; layerPerStaff=0;
	
	
	
	// for each slot and layer...
	for (slot=reg->start.IUslot; slot<=reg->end.IUslot; ++slot) 
	{
		for (layer=0; layer<MAXLAYERS; ++layer) 
		{
			
			openSeq=0; acctsa=0; acctsb=1;
			
			// for each selected measure...
			for (meas=reg->start.meas; meas<=reg->end.meas; ++meas) 
			{
				
				// determine selected part of measure...
				selStart=((meas==reg->start.meas)?reg->start.duraPos:0);
				selEnd=  ((meas==reg->end.meas)?reg->end.duraPos:FOURBYTE_MAX);
				
				dirtyFlag=0;
				
				// has the time signature changed?
				// (new staffs are handled later)
				otsa=ts_a; otsb=ts_b;
				getTimeSig(meas, &ts_a, &ts_b);
				showTsFlag=((otsa!=ts_a)||(otsb!=ts_b));
				
				// no cross-measure beaming
				w2g.flush(f,M_BEAM);
				possibleBeam=0; beam2=0;
				
				
				// iterate through measure	
				p=FX_LoadTGF(reg->start.IUlist, slot, meas, layer, YES);
				if (p) 
				{
					for (i=0; i<p->numEntries; ++i) 
					{
						if ((p->pent[i].elldur<selStart)||(p->pent[i].elldur>selEnd)) continue;
						if ((storageFlag)&&(!stored(p,meas,i,storage))) continue;
						
						// has the key signature changed?
						oldKeySig=keySig;
						keySig=FX_GetKeySig(meas,FX_SlotToInst(slot,0),1);
						showKsFlag=(keySig!=oldKeySig);
						
						newOpenSeq=0;
						if (!openSeq) 
						{ // new sequence
							showKsFlag=1; showTsFlag=1; newOpenSeq=1;
							if (seqCounter) {
								LIFSET(F_CR)
									w2g.write(f,",\r\n\r\n",5);
								LEND
									LIFNOT(F_CR)
									w2g.write(f,",\r\n",3);
								LEND
							}
							++seqCounter; 
							openSeq=1; 
							w2g.write(f,"[ ",2);
							
							// beams (0)
							LIFSET(F_BEAM)
								w2g.write(f,"\\beamsOff ",10);
							LEND
								
								LIFSET(F_STAFF)
								sprintf(str,"\\staff<%i> ",staffNumber); 
							w2g.write(f,str,strlen(str));
							LEND
								++layerPerStaff; expEllDur=0;
						} // if ! open seq
						
						if (showKsFlag) 
						{ // write key signature if necessary
							ksName(keySig,str); 
							w2g.write(f,str,strlen(str));
						}
						if (showTsFlag) 
						{ // write time signature if necessary
							sprintf(str,"\\meter<\"%i/%i\"> ", ts_a, ts_b);
							w2g.write(f,str,strlen(str)); showTsFlag=0;
						}
						
						// write leading rest/empty if necessary
						if (newOpenSeq&&acctsa) 
						{
							if (layerPerStaff==1)
								sprintf(str,"_*%i/%i ", acctsa, acctsb);
							else
								sprintf(str,"empty*%i/%i ", acctsa, acctsb);
							w2g.write(f,str,strlen(str));
						}
						
								
						char addLyrics = 0; // flag is 1 if lyrics found
								
								
						dirtyFlag=1;
						
						if ((expEllDur>p->pent[i].elldur)&&(!storageFlag)) 
						{
								// can't handle this note now,
								// must be dealt with later
								store(p,meas,i,&storage);
						} 
						else 
						{
								// create gap-rests
								if (expEllDur<p->pent[i].elldur) 
								{
									if (layerPerStaff==1) 
										strcpy(str,"_"); 
									else strcpy(str,"empty");
									w2g.write(f,str,strlen(str));
									durName(p->pent[i].elldur-expEllDur,p->pent[i].elldur-expEllDur,str);
									w2g.write(f,str,strlen(str)); w2g.write(f," ",1);
									expEllDur=p->pent[i].elldur;
								}
						expEllDur+=p->pent[i].actdur;
								
						// write debug info if requested
						LIFSET(F_DEBUG)
							sprintf(str,
								"\r\n(* EXENTRY: dura %i ef %X xef %X numNotes %i elldur %i actdur %i *) ",
									p->pent[i].dura, p->pent[i].ef, p->pent[i].xef,
									p->pent[i].numNotes, p->pent[i].elldur, p->pent[i].actdur);
							w2g.write(f,str,strlen(str));
						LEND

							

							if (p->pent[i].numNotes) 
							{
								// Is it a grace note? (a)
								if (!((p->pent[i].actdur==0)&&(p->pent[i].dura!=0))) 
								{
									if (graceFlag) 
									{
										graceFlag=0; w2g.write(f,") ",2);
									}	
								}
								
								// beams (1)
								LIFSET(F_BEAM)
									if ((p->pent[i].dura<1024)&&
										((p->pent[i].ef&0x40000000)!=0))  // this is a note
									{
										if (p->pent[i].ef&0x800) 
										{
											if (possibleBeam==2) 
											{
												w2g.insert(M_BEAM,"\\beam(",6);
												w2g.flush(f,M_BEAM);
												if (beam2) 
												{
													w2g.write(f,")",1);
													beam2=0;
												}
												w2g.write(f,") ",2);
											}
											w2g.mark(M_BEAM);
											possibleBeam=1;
										} 
										else 
										{
											if (possibleBeam==1) possibleBeam=2;
										}
									} 
									else 
									{
										if (possibleBeam==2) 
										{
											w2g.insert(M_BEAM,"\\beam(",6);
											w2g.flush(f,M_BEAM);
											if (beam2) 
											{
												w2g.write(f,")",1);
												beam2=0;
											}
											w2g.write(f,") ",2);
										}
										possibleBeam=0; w2g.flush(f,M_BEAM);
									}
									if ((p->pent[i].ef&0x400)&&(possibleBeam!=0)) 
									{
										if ((possibleBeam==2)&&(!beam2)) 
										{
											w2g.insert(M_BEAM,"\\beam(\\beam(",12);
											w2g.flush(f,M_BEAM);
											w2g.write(f,") ",2);
											w2g.mark(M_BEAM);
											beam2=1;
										} 
										else 
										{
											if (beam2) 
											{
												w2g.write(f,") ",2);
											}
											w2g.write(f,"\\beam(",6);
											beam2=1;
										}
									}
									LEND
										
										// Is it a grace note? (b)
										if ((p->pent[i].actdur==0)&&(p->pent[i].dura!=0)) 
										{
											if (graceFlag==0) 
											{
												graceFlag=1; w2g.write(f,"\\grace(",7);
											}
										}
// DO here the lyrics --------------
								LIFSET(F_LYRICS)
			 addLyrics = lyricsTag(p->pent[i],
								dxStr,
								fSizeStr,
								str);
							if( addLyrics )
							{
								// brackte must be closed after note || chord
								w2g.write(f,str,strlen(str));								

							} // if lyrics
						LEND
						//---------------------						
			
										
												// Is it a chord?
												chFlag=(p->pent[i].numNotes > 1);
												if (chFlag) 
													w2g.write(f,"{",1);
												
												// for each note...
												for (j=0; j<p->pent[i].numNotes; ++j) 
												{
													
													// write debug info if requested
													LIFSET(F_DEBUG)
														sprintf(str,
														"\r\n(* EXNOTE: tcd %i alteration %i noteID %i flag %X *) ",
														p->pent[i].note[j].tcd, p->pent[i].note[j].alteration,
														p->pent[i].note[j].noteID, p->pent[i].note[j].flag);
													w2g.write(f,str,strlen(str));
													LEND
														
														// tie notes (1)
														midiv=FX_NoteToMidi(&(p->pent[i].note[j]),keySig);
													if ((p->pent[i].note[j].flag&0x60000000)!=0x60000000)
													{
														if (p->pent[i].note[j].flag&0x40000000) 
														{
															sprintf(str,"\\tieBegin:%i ",midiv); 
															w2g.write(f,str,strlen(str));
														}
													}
								

													
													// tone
													toneName(p->pent[i].note[j].tcd, keySig, midiv,
														p->pent[i].ef, str);
													w2g.write(f,str,strlen(str));
													
													// duration
													if (graceFlag)
														durName(p->pent[i].dura, p->pent[i].dura, str);
													else
														durName(p->pent[i].dura, p->pent[i].actdur, str);
													w2g.write(f,str,strlen(str));
													
													// tie notes (2)
													if ((p->pent[i].note[j].flag&0x60000000)!=0x60000000)
													{
														if (p->pent[i].note[j].flag&0x20000000) 
														{
															sprintf(str," \\tieEnd:%i",midiv); w2g.write(f,str,strlen(str));
														}
													}
													
													// write separator (',' for chords, ' ' else)
													if ((!chFlag)||(j<p->pent[i].numNotes-1)) 
														w2g.write(f,(chFlag?",":" "),1);
												} // for
												
												if (chFlag) 
													w2g.write(f,"} ",2);											
							} 
							else  // numnotes == 0
							{ // chord-rest
					// Here lyrics ? -----------
// DO here the lyrics --------------
								LIFSET(F_LYRICS)
			 addLyrics = lyricsTag(p->pent[i],
								dxStr,
								fSizeStr,
								str);
							if( addLyrics )
							{
								// brackte must be closed after note || chord
								w2g.write(f,str,strlen(str));								

							} // if lyrics
						LEND
						//---------------------						



								
								
								// as chord-rests are explicit, they're never transformed to empties
								w2g.write(f,"_",1);
								durName(p->pent[i].dura, p->pent[i].actdur, str);
								w2g.write(f,str,strlen(str));
								w2g.write(f," ",1);
								
							} // if numNotes
						} // if ellDur<expEllDur
						
						if( addLyrics )
						{
							// close lyric tag
							w2g.write(f,") ", 2);
							addLyrics = 0;
						}
					} // end: for numEntries
				} // if p
				
				// beams (2)
				LIFSET(F_BEAM)
					if (beam2) 
					{
						w2g.write(f,")",1); beam2=0;
					}
					if (possibleBeam==2) 
					{
						w2g.insert(M_BEAM,"\\beam(",6);
						w2g.flush(f,M_BEAM);
						w2g.write(f,") ",2);
						possibleBeam=0;
					}
					w2g.flush(f,M_BEAM);
					LEND
						
						if ((!dirtyFlag)&&(openSeq)) 
						{
							// empty measure but running sequence
							
							if (showTsFlag) 
							{
								sprintf(str,"\\meter<\"%i/%i\"> ", ts_a, ts_b);
								w2g.write(f,str,strlen(str)); showTsFlag=0;
							}
							
							// (key signature is not being checked for empty measures)
							
							if (layerPerStaff==1) 
							{
								w2g.write(f,"_",1);
							} 
							else 
							{
								w2g.write(f,"empty",5);
							}
							sprintf(str,"*%i/%i ", ts_a, ts_b); w2g.write(f,str,strlen(str));
							
						} 
						else 
						{ 
							if (openSeq&&underfullMeasure(expEllDur,ts_a,ts_b,layerPerStaff,str)) 
							{
								// trailing gap-rests
								w2g.write(f,str,strlen(str));
							}
						}
						
						p=FX_FreeTGF(p);
						
						// cosmetics and resetting expEllDur
						if ((meas<reg->end.meas)&&(openSeq)) 
						{
							LIFSET(F_STAFF)
								w2g.write(f,"| ",2);
							LEND
								if (meas%3==0) 
								{
									LIFSET(F_CR)
										w2g.write(f,"\r\n",2);
									LEND
									LIFSET(F_COMMENT)
										sprintf(str,"(* M.%i *) ", meas+1);
										w2g.write(f,str,strlen(str));
									LEND
								}
								expEllDur=0;
						}
						
						// accumulate time signatures
						accumulateTS(ts_a, ts_b, &acctsa, &acctsb);
			} // end: for measure
			if (openSeq) 
			{
				if (graceFlag) 
				{
					graceFlag=0; w2g.write(f,") ",2);
				}	
				w2g.write(f,"]",1);
			}
			// If there are unwritten notes, do it again!
			if (storage) 
			{
				if (!storageDone(storage)) 
				{
					--layer; storageFlag=1;
				} 
				else 
				{
					storageFlag=0; delete storage; storage=NULL;
				}
			}
		} // end: for layer
		++staffNumber; layerPerStaff=0;
	} // end: for slot
	
	w2g.write(f,"}",1);
	w2g.flush(f);
	
	fclose(f);
}

////////////////////////////////////////////////////////////////////////////////////////

PUBLIC FX_RETURNVAL FINEXPORT f2gDHandler(EWND hDlg, ufourbyte msg,
										  WPARAM wParam, LPARAM lParam, void *) {
	tbool handled=YES;
	char strBuf[1023];
	
	switch (msg) {
	case WM_INITDIALOG:	
		gmnFormat=oldGmnFormat;
		CheckDlgButton(hDlg,IDC_CHECK1,(gmnFormat&2)?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,IDC_CHECK2,(gmnFormat&4)?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,IDC_CHECK3,(gmnFormat&8)?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,IDC_CHECK4,(gmnFormat&16)?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,IDC_CHECK5,(gmnFormat&32)?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,IDC_LYRICS,(gmnFormat&64)?BST_CHECKED:BST_UNCHECKED);
		SetDlgItemText(hDlg, IDC_FSIZE, fSizeStr);
		SetDlgItemText(hDlg, IDC_DX, dxStr);

		break;
	case WM_COMMAND:
		{
			WPARAM cmdID = GET_WM_COMMAND_ID(wParam, lParam);
			
			switch (cmdID) 
			{
			case IDCANCEL:
				gmnFormat=0; // no break
			case IDOK:
				// get value for dx and fsize
				GetDlgItemText(hDlg, IDC_FSIZE, fSizeStr, 10 );
				GetDlgItemText(hDlg, IDC_DX, dxStr, 10 );

				FX_EndDialog(hDlg);
				break;
			case IDABOUT1:
				sprintf(strBuf,"Version %4s.\r\nWritten by T. Mann & J. Kilian.\r\nFor more information see http://www.salieri.org",VERSION);
				MessageBox(hDlg, strBuf,
					"About Export to GUIDO Music Notation",
					MB_ICONINFORMATION|MB_OK);
				break;
			case IDC_CHECK1: gmnFormat^=2; break; // cosntants are defined somewhre else!
			case IDC_CHECK2: gmnFormat^=4; break;
			case IDC_CHECK3: gmnFormat^=8; break;
			case IDC_CHECK4: gmnFormat^=16; break;
			case IDC_CHECK5: gmnFormat^=32; break;
			case IDC_LYRICS: gmnFormat^=64; break;
				
			default:
				handled = NO;
			} // switch
			break;
		}
	case WM_LBUTTONDOWN:
	default:
		handled = NO;
	}
	
	return handled;
}
