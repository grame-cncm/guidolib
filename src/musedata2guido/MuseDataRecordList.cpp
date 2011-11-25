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
// MuseDataRecordList.cpp: implementation of class CMuseDataRecordList.
//
//////////////////////////////////////////////////////////////////////

#include "MuseDataRecordList.h"
#include "MuseDataNoteRecord.h"
#include "MuseDataCombineDivideRecord.h"
#include "Errorhandling.h"
#include "GuidoCreator.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// construction/deconstruction
//////////////////////////////////////////////////////////////////////

CMuseDataRecordList::CMuseDataRecordList()
{
	first=NULL;
	last=NULL;
	current=NULL;

	idNotesWithoutTrackID=-1;
	for(int i=0;i<10;i++){idUsesExplicitTrack[i]=-1;}
}

CMuseDataRecordList::~CMuseDataRecordList()
{

}

int CMuseDataRecordList::appendRecord(CMuseDataDataRecord *dr)
{
	CMuseDataRecordListEntry* newEntry=new CMuseDataRecordListEntry();
	newEntry->record=dr;
	newEntry->next=NULL;
	newEntry->before=NULL;

	//case 1: list is empty
	if(first==NULL){
		first=newEntry;
		last=newEntry;
		return 0;
	}

	//case 2: list is not empty
	last->next=newEntry;
	newEntry->before=last;
	last=newEntry;
	return 0;
		
}

void CMuseDataRecordList::printOn(FILE * f)
{
	CMuseDataRecordListEntry* entry=first;
	fprintf(f,"START of LIST\n");
	while(entry!=NULL){
		entry->record->printOn(f);
		fprintf(f,"\n");
		entry=entry->next;
	}
	fprintf(f,"END of LIST\n");
}

void CMuseDataRecordList::gotoFirstRecord()
{
	before=NULL;
	current=first;
}

void CMuseDataRecordList::gotoNextRecord()
{
	if(current!=NULL){
		before=current;
		current=current->next;
	}
}

void CMuseDataRecordList::gotoBeforeRecord()
{
	if(current!=NULL){
		current=current->before;
		if(current!=NULL){
			before=current->before;
		} else {
			before=NULL;
		}
	}
}

CMuseDataDataRecord* CMuseDataRecordList::peekCurrentRecord()
{
	if(current==NULL){
		return NULL;
	} else {
		return current->record;
	}
}

int CMuseDataRecordList::isEndOfList()
{
	return(current==NULL);
}

CMuseDataDataRecord* CMuseDataRecordList::removeCurrentRecord()
{
	if(current==NULL){return NULL;}
	//the record we wish to return
	CMuseDataDataRecord* record=current->record;

	//now remove the entry from the list
	//case 1: only element in list
	if( (current==first) && (current==last)){
		first=NULL;
		last=NULL;
		before=NULL;
		delete current;
		current=NULL;
		return record;
	}

	//case 2: first element in list
	if(current==first){
		first=current->next;
		before=NULL;
		delete current;
		current=first;
		current->before=NULL;
		return record;
	}

	//case 3: last element in list
	if(current==last){
		last=before;
		before->next=NULL;
		delete current;
		current=NULL;
		return record;
	}

	//case 4: somewhere else
	before->next=current->next;
	current->next->before=before;
	delete current;
	current=before->next;
	return record;
}


int CMuseDataRecordList::convMarkChords(){
	//step one: create sublists for chords:
	CMuseDataDataRecord * last=NULL;
	gotoFirstRecord();
	while(!isEndOfList()){
		if(peekCurrentRecord()->getDataRecordType()==drt_NoteRecord){
			if( ((CMuseDataNoteRecord*)(peekCurrentRecord()))->getrdChordFlag() ){
				if(last==NULL){
					//TODO: ERRORHANLING
				} else {
					if(last->getDataRecordType()!=drt_NoteRecord){
						//TODO: ERRORHANDLING
					} else {
						((CMuseDataNoteRecord*)(last))->setidNextInChord((CMuseDataNoteRecord*)(peekCurrentRecord()));
					}
				}
			}
		}
		last=peekCurrentRecord();
		gotoNextRecord();
	}

	//step two: split chords in every voice, calculate duration of chord
	int chordLabel=1;
	gotoFirstRecord();
	while(!isEndOfList()){
		if(peekCurrentRecord()->getDataRecordType()==drt_NoteRecord){
			if( (!((CMuseDataNoteRecord*)(peekCurrentRecord()))->getrdChordFlag() )
				&&(((CMuseDataNoteRecord*)(peekCurrentRecord()))->getidNextInChord()!=NULL ) ){
				//we have found an initial note-record of a chord!
				//now we have to split the chord into sub-chords in different tracks
				//Create an array for the note-records in the different tracks
				int chordDuration=0;
				pMuseDataNoteRecord records[11];
				pMuseDataNoteRecord first=(CMuseDataNoteRecord*)(peekCurrentRecord());
				pMuseDataNoteRecord current=first;
				int i;
				for(i=0;i<11;i++){records[i]=NULL;}
				do{
					//splitting of tracks:
					if(records[1+current->getTrackNumber()]==NULL){
						//no chord-note in this track!
						current->setidChordInfo(range_Begin);
					} else {
						current->setidChordInfo(range_Part);
					}
					records[1+current->getTrackNumber()]=current;
					//chord duration:
					if(current->getrdDuration()>chordDuration){
						chordDuration=current->getrdDuration();
					}
					//goto next record in list...
					current=current->getidNextInChord();
				} while(current!=NULL);
				//end chord in all voices
				int trackCount=0;
				for(i=0;i<11;i++){
					if(records[i]!=NULL){
						records[i]->setidChordInfo(range_End);
						trackCount++;
					}
				}
				//set chord duration and label in each note of chord
				int label=-1;
				if(trackCount>1){label=chordLabel;chordLabel++;}
				current=first;
				do{
					current->setidChordDuration(chordDuration);
					current->setidChordLabel(label);
					current=current->getidNextInChord();
				}while(current!=NULL);
			}
		}
		gotoNextRecord();
	}

	return 0;
}



int CMuseDataRecordList::convCalculateRecordTimes()
{
	int last=0;
	int time=0;
	gotoFirstRecord();
	while(!isEndOfList()){
		peekCurrentRecord()->setidTime(time);
		switch(peekCurrentRecord()->getDataRecordType()){
		case drt_NoteRecord:
			if(((CMuseDataNoteRecord*)(peekCurrentRecord()))->getrdChordFlag()){
				//note is part of chord, so duration doesn't add to time!
				//but the start time ist the start time of the last note!
				peekCurrentRecord()->setidTime(last);
			} else {
				last=time;
				time=time+((CMuseDataNoteRecord*)(peekCurrentRecord()))->getrdDuration();
			}
			break;
		case drt_CombineDivideRecord:
			time=time+((CMuseDataCombineDivideRecord*)(peekCurrentRecord()))->getChangeOfTime();
			break;
		}
		gotoNextRecord();
	}
	return 0;

}

int CMuseDataRecordList::convRemoveCombineDivideRecords()
{
	gotoFirstRecord();
	while(!isEndOfList()){
		if(peekCurrentRecord()->getDataRecordType()==drt_CombineDivideRecord){
			removeCurrentRecord()->release();
		}else{
			gotoNextRecord();
		}
	}
	return 0;
}

int CMuseDataRecordList::hasOverlappingNotes()
{
	int lastEnd=0;
	gotoFirstRecord();
	while(!isEndOfList()){
		if(peekCurrentRecord()->getDataRecordType()==drt_NoteRecord){
			if( ! ( ((CMuseDataNoteRecord*)(peekCurrentRecord()))->getidChordInfo()==range_Part
					||((CMuseDataNoteRecord*)(peekCurrentRecord()))->getidChordInfo()==range_End   ) ){
				if (peekCurrentRecord()->getidTime() <lastEnd){
					return 1;
				}
				if( ((CMuseDataNoteRecord*)(peekCurrentRecord()))->getidChordInfo()==range_NoPart ){
					lastEnd=peekCurrentRecord()->getidTime()+((CMuseDataNoteRecord*)(peekCurrentRecord()))->getrdDuration();
				} else {
					lastEnd=peekCurrentRecord()->getidTime()+((CMuseDataNoteRecord*)(peekCurrentRecord()))->getidChordDuration();
				}
			}
		}
		gotoNextRecord();
	}
	return 0;
}//hasOverlappingNotes

int CMuseDataRecordList::convCalcSlurIDs(int nextID)
{
	int i, slurID[4];
	pMuseDataNoteRecord slurBeginRecord[4];
	for(i=0;i<4;i++){slurID[i]=0; slurBeginRecord[i]=NULL;}
	gotoFirstRecord();
	while(!isEndOfList()){
		if(peekCurrentRecord()->getDataRecordType()==drt_NoteRecord){
			CMuseDataNoteRecord* r=(CMuseDataNoteRecord*)(peekCurrentRecord());
			for (int i=0;i<4;i++){
				if(r->getrdBeginSlur(i)){
					if(slurID[i]==0){
						r->setidSlurID(i,nextID);
						slurBeginRecord[i]=r;
						slurID[i]=nextID;
						nextID++;
					} else {
						MUSEERROR(err_ReopenSlur,r->getidLineNumber(),0);
					}
				}
				if(r->getrdEndSlur(i)){
					if(slurID[i]==0){
						MUSEERROR(err_SlurNotOpen,r->getidLineNumber(),0);
					} else {
						r->setidSlurID(i,slurID[i]);
						slurBeginRecord[i]=NULL;
						slurID[i]=0;
					}
				}
			}
		}
		gotoNextRecord();
	}
	//check for not-closed slurs
	for(i=0;i<4;i++){
		if(slurID[i]!=0){
			MUSEERROR(err_SlurNotClosed,slurBeginRecord[i]->getidLineNumber(),0);
			slurBeginRecord[i]->setidSlurID(i,0);
		}
	}
	return nextID;
}//convCalcSlurIDs

int CMuseDataRecordList::convCalcTieIDs(int nextID)
{
	int tieID[400];
	for(int i=0;i<400;i++){tieID[i]=0;}
	gotoFirstRecord();
	while(!isEndOfList()){
		if(peekCurrentRecord()->getDataRecordType()==drt_NoteRecord){
			int note=( (CMuseDataNoteRecord*)(peekCurrentRecord()) )->getrdPitchCode();
			int tied=( (CMuseDataNoteRecord*)(peekCurrentRecord()) )->isTiedToNext();
			//begin of tie??
			if (tied){
				if(tieID[note]==0){
					tieID[note]=nextID;
					( (CMuseDataNoteRecord*)(peekCurrentRecord()) )->setidTieBegin(nextID);
					nextID++;
				}
			} 
			//end of tie??
			else {
				if(tieID[note]!=0){
					( (CMuseDataNoteRecord*)(peekCurrentRecord()) )->setidTieEnd(tieID[note]);
					tieID[note]=0;
				}
			}
		}
		gotoNextRecord();
	}
	return nextID;
}//convCalsTieIDs

int CMuseDataRecordList::printGuidoOn(FILE *f,CConversionParameters * c)
{
	int lastEnd=0;
	//guidoCreator.init();
	guidoCreator.sequenceBegin(idTrackNo,idSubtrackNo);
	//fprintf(f,"\\instr<\"piano\",\"MIDI 0\"> ");
	gotoFirstRecord();
	while(!isEndOfList()){
//		int currentTime__=peekCurrentRecord()->getidTime();
		if(peekCurrentRecord()->getidTime()>lastEnd){
			//create an empty-event if neccessary
			//before this, check, if divisions per quarter is set
			if(c->divisionsPerQuarter<=0){
				MUSEERROR(err_NoDivisionPerQuarterInformation,peekCurrentRecord()->getidLineNumber(),0);
			}
			guidoCreator.empty(peekCurrentRecord()->getidTime()-lastEnd,c->divisionsPerQuarter*4,0);
			lastEnd=peekCurrentRecord()->getidTime();
		}
		if(peekCurrentRecord()->printGuidoOn(f,c)){
			return -1;
		}
		if(peekCurrentRecord()->getDataRecordType()==drt_NoteRecord){
			if(!( (((CMuseDataNoteRecord*)peekCurrentRecord())->getidChordInfo()==range_Part)
				||(((CMuseDataNoteRecord*)peekCurrentRecord())->getidChordInfo()==range_End ) ) ){
				//note is not part of a chord-> so we must add duration to lastEnd!
				if( ((CMuseDataNoteRecord*)peekCurrentRecord())->getidChordInfo()==range_NoPart){
					lastEnd+=((CMuseDataNoteRecord*)peekCurrentRecord())->getrdDuration();
				} else {
					lastEnd+=((CMuseDataNoteRecord*)peekCurrentRecord())->getidChordDuration();
				}
			}
		}
		gotoNextRecord();
	}
	guidoCreator.sequenceEnd();
	return 0;
}

int CMuseDataRecordList::convMarkGraceRanges()
{
	int graceOn=0;
	CMuseDataNoteRecord* last=NULL;
	CMuseDataNoteRecord* r=NULL;
	gotoFirstRecord();
	while(!isEndOfList()){
		if(peekCurrentRecord()->getDataRecordType()==drt_NoteRecord){
			last=r;
			r=(CMuseDataNoteRecord*)(peekCurrentRecord());
			if(r->getrdNoteType()==nt_grace){
				if (graceOn) {r->setidGraceNote(range_Part);}
				else {r->setidGraceNote(range_Begin);graceOn=1;}
			}
			else {
				if (graceOn) {last->setidGraceNote(range_End);graceOn=0;}
			}
		}
		gotoNextRecord();
	}
	if(graceOn){
		r->setidGraceNote(range_End);
	}
	return 0;
}

int CMuseDataRecordList::convMarkCueRanges()
{
	int cueOn=0;
	CMuseDataNoteRecord* last=NULL;
	CMuseDataNoteRecord* r=NULL;
	gotoFirstRecord();
	while(!isEndOfList()){
		if(peekCurrentRecord()->getDataRecordType()==drt_NoteRecord){
			last=r;
			r=(CMuseDataNoteRecord*)(peekCurrentRecord());
			if(r->getrdNoteType()==nt_cue){
				if (cueOn) {r->setidCueNote(range_Part);}
				else {r->setidCueNote(range_Begin);cueOn=1;}
			}
			else {
				if (cueOn) {last->setidCueNote(range_End);cueOn=0;}
			}
		}
		gotoNextRecord();
	}
	if(cueOn){
		r->setidCueNote(range_End);
	}
	return 0;}

int CMuseDataRecordList::convCalcTrackUsage()
{
	idNotesWithoutTrackID=0;
	for(int i=0;i<10;i++){idUsesExplicitTrack[i]=0;}
	gotoFirstRecord();
	while(!isEndOfList()){
		if(peekCurrentRecord()->getDataRecordType()==drt_NoteRecord){
			int id=peekCurrentRecord()->getTrackNumber();
			if(id==-1){
				idNotesWithoutTrackID=1;
			}else{
				idUsesExplicitTrack[id]=1;
			}
		}
		gotoNextRecord();
	}
	return 0;
}

int CMuseDataRecordList::usesExplicitTrack(int i)
{
	if(i==-1){return idNotesWithoutTrackID;}
	return idUsesExplicitTrack[i];
}

int CMuseDataRecordList::usesNotesWithoutExplicitTrack()
{
	return idNotesWithoutTrackID;
}

CMuseDataRecordList* CMuseDataRecordList::createExplicitSubtrack(int id)
{
	CMuseDataRecordList* newList=new CMuseDataRecordList();
	gotoFirstRecord();
	while(!isEndOfList()){
		if(peekCurrentRecord()->getTrackNumber()==id){
			peekCurrentRecord()->reference();
			newList->appendRecord(peekCurrentRecord());
		} else {
			if(peekCurrentRecord()->getDataRecordType()!=drt_NoteRecord){
				peekCurrentRecord()->reference();
				newList->appendRecord(peekCurrentRecord());
			}
		}
		gotoNextRecord();
	}
	return newList;
}

CMuseDataRecordList* CMuseDataRecordList::stripImplicitSubtrack()
{
	CMuseDataRecordList* newList=new CMuseDataRecordList();
	int currentEnd=0;
	gotoFirstRecord();
	if(!isEndOfList()){currentEnd=peekCurrentRecord()->getidTime();}
	while(!isEndOfList()){
		if(peekCurrentRecord()->getidTime()>=currentEnd){
			currentEnd=peekCurrentRecord()->getidTime();
			if(peekCurrentRecord()->getDataRecordType()==drt_NoteRecord){
				if( ((CMuseDataNoteRecord*)(peekCurrentRecord()))->getidChordInfo()==range_NoPart ){
					currentEnd+=((CMuseDataNoteRecord*)peekCurrentRecord())->getrdDuration();
				} else {
					currentEnd+=((CMuseDataNoteRecord*)peekCurrentRecord())->getidChordDuration();
				}
				newList->appendRecord(removeCurrentRecord());
			} else {
				peekCurrentRecord()->reference();
				newList->appendRecord(peekCurrentRecord());
				gotoNextRecord();
			}
		} else {
			//Changed 09/04/01 MF
			//check, if we are part of a chord:
			if(peekCurrentRecord()->getDataRecordType()==drt_NoteRecord){
				if( ( ((CMuseDataNoteRecord*)(peekCurrentRecord()))->getidChordInfo() == range_Part)
				 || ( ((CMuseDataNoteRecord*)(peekCurrentRecord()))->getidChordInfo() == range_End) ){
					newList->appendRecord(removeCurrentRecord());
				} else {
					gotoNextRecord();
				}
			} else {
				gotoNextRecord();
			}
			//gotoNextRecord(); removed
			//End of Change 04/09/01 MF
		}
	}
	return newList;
}

int CMuseDataRecordList::convMakeBeams()
{
	int lastBeamCount=0;
	CMuseDataNoteRecord* lastNoteRecord=NULL;
	CMuseDataNoteRecord* currentNoteRecord=NULL;
	
	gotoFirstRecord();
	while(!isEndOfList()){
		if(peekCurrentRecord()->getDataRecordType()==drt_NoteRecord){
			currentNoteRecord=(CMuseDataNoteRecord*)(peekCurrentRecord());
			//check beam-consistency
			if(lastBeamCount+currentNoteRecord->getidBeamChange()==currentNoteRecord->getidBeamCount()){
				//this is fine...
				currentNoteRecord->setidBeamDrawInfo(currentNoteRecord->getidBeamChange());
			} else {
				//inconsistent beaming: change beamChange...
				currentNoteRecord->setidBeamDrawInfo(currentNoteRecord->getidBeamCount()-lastBeamCount);
				MUSEERROR(err_InconsistenBeaming,currentNoteRecord->getidLineNumber(),0);
			}
			lastBeamCount=currentNoteRecord->getidBeamCount();
			lastNoteRecord=currentNoteRecord;
		}
		gotoNextRecord();
	}
	if(lastBeamCount!=0){
		//we have left open beams...
		MUSEERROR(err_InconsistenBeaming,lastNoteRecord->getidLineNumber(),0);
		lastNoteRecord->setidBeamDrawInfo(lastNoteRecord->getidBeamDrawwInfo()-lastBeamCount);
	}
	return 0;
}



void CMuseDataRecordList::setTrackNo(int track, int subtrack)
{
	idTrackNo=track;
	idSubtrackNo=subtrack;
}

int CMuseDataRecordList::convCalcSortListByRecordTime()
{
	CMuseDataRecordListEntry* ptrEntry=first;
	int i=0;
	while(ptrEntry!=NULL){
		i++;
		//if we are at the beginning of the list, we goto the next entry...
		if(ptrEntry->before==NULL){
			ptrEntry=ptrEntry->next;
			continue;
		}
		//if the time of the record before the current one is less or equal, we go to the next entry
		if(ptrEntry->before->record->getidTime()<=ptrEntry->record->getidTime()){
			ptrEntry=ptrEntry->next;
			continue;
		}
		CMuseDataRecordListEntry* newBefore;
		CMuseDataRecordListEntry* newNext;
		CMuseDataRecordListEntry* oldBefore=ptrEntry->before;;
		CMuseDataRecordListEntry* oldNext=ptrEntry->next;
		if(oldBefore->before==NULL){
			//we move to the head of the list
			newBefore=NULL;
			first=ptrEntry;
		} else {
			newBefore=oldBefore->before;
		}
		newNext=oldBefore;
		//link newBefore with current
		if(newBefore!=NULL){
			newBefore->next=ptrEntry;
		}
		ptrEntry->before=newBefore;
		//link current with newNext
		ptrEntry->next=newNext;
		newNext->before=ptrEntry;
		//link newNext with old next
		newNext->next=oldNext;
		if(oldNext!=NULL){
			oldNext->before=newNext;
		} else {
			//we have a new tail of the list
			last=newNext;
		}
	}
	gotoFirstRecord();
	return 0;
}

