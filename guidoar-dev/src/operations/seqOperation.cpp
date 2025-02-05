/*
  GUIDO Library
  Copyright (C) 2006  Grame

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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifdef WIN32
# pragma warning (disable : 4786)
#endif

#include <iostream>

#include "clonevisitor.h"
#include "countvisitor.h"
#include "seqOperation.h"
#include "ARFactory.h"
#include "ARChord.h"
#include "ARNote.h"
#include "AROthers.h"
#include "markers.h"
#include "tree_browser.h"

#include "ARTag.h"

using namespace std;

namespace guido 
{


//______________________________________________________________________________
/*!
\brief	A visitor to remove closed tags
*/
class gar_export closedRemover : public clonevisitor
{
	protected:
		void visitStart ( Sguidotag& elt )	{ if (markers::opened (elt) != markers::kClosed) clonevisitor::visitStart(elt); }
		void visitEnd	( Sguidotag& elt )	{ if (markers::opened (elt) != markers::kClosed) clonevisitor::visitEnd(elt); }

    public:
				 closedRemover() {}
		virtual ~closedRemover() {}
};

//______________________________________________________________________________
/*!
\brief	A visitor to clean opened tags
*/
//______________________________________________________________________________
class gar_export seqCleaner : public clonevisitor
{
	protected:
		SARNote			fFirstTied;			// used for merging tied notes carrying begin-end markers
		Sguidoelement	fFirstCTied;		// used for merging chords notes carrying begin-end markers
		std::map<std::string,Sguidoelement>	fEndTags; // used for cancelling xxxEnd|Begin sequence carrying end-begin markers
		rational		fCurrentDuration;
		bool	fInTie;		
		bool	fTieChord;		

		void visitStart ( SARVoice& elt )	{ fCurrentDuration.set(1,4); fInTie = false; fTieChord=false; clonevisitor::visitStart(elt); }
		void visitStart ( SARNote& elt );
		void visitStart ( SARChord& elt );
		void visitStart ( Sguidotag& elt );

		void visitEnd	( SARChord& elt );
		void visitEnd	( Sguidotag& elt );

    public:
				 seqCleaner() : 	fCurrentDuration(1,4), fInTie(false) {}
		virtual ~seqCleaner() {}
};

//_______________________________________________________________________________
void seqCleaner::visitStart ( Sguidotag& elt )
{
	int status = markers::opened (elt);
	bool done = false;
	if (status == markers::kClosed) {
		if (elt->getType() == kTTie) fInTie = true;
		markers::delMark (elt);
	}
	if (elt->endTag() && (status == markers::kOpenedEnd)) {
		Sguidoelement end = ARFactory::instance().createTag(elt->getName(), elt->getID());
		end = copy(elt, end);
		clonevisitor::push (end, false);
		fEndTags[end->getName()] = end;
		done = true;			// manual copy to keep the element in the end map
	}
	// check if it corresponds to a previous end tag
	else if (elt->beginTag() && (status == markers::kOpenedBegin)) {
		string name = elt->matchTag();
		Sguidoelement end = fEndTags[name];
		if (end) {
			markers::setMark (elt, markers::kClosed);		// mark both as close
			markers::setMark (end, markers::kClosed);		// they should be removed
			fEndTags[name] = (void*)0;
		}
	}
	if (!fInTie && !done) clonevisitor::visitStart (elt);
}

void seqCleaner::visitEnd ( Sguidotag& elt )
{
	if (fInTie) {
		fInTie = false;
		fTieChord = false;
		fFirstTied = (void*)0;
		fFirstCTied = (void*)0;
	}
	else clonevisitor::visitEnd (elt);
}

//_______________________________________________________________________________
void seqCleaner::visitStart ( SARChord& elt )
{
	fCurrentDuration = elt->totalduration (fCurrentDuration, 0);
	if (fInTie) {						// we should get 2 chords to merge
		fTieChord = true;				// a flag to prevent notes from being handled by ARNote visit
		if (fFirstCTied) {				// we've already got the first chord
			ARChord * c = dynamic_cast<ARChord*>((guidoelement*)fFirstCTied);
			if (c) *c = c->duration() + fCurrentDuration;	// add the current chord duration to this first chord
		}
		else {										// this is the first chord
			clonevisitor cc;
			*elt = fCurrentDuration;				// makes explicit duration
			fFirstCTied = cc.clone(elt);			// make a copy and store it for future use
			clonevisitor::push(fFirstCTied, false);	// and push it to the current stack
		}
	}
	else clonevisitor::visitStart (elt);
}

void seqCleaner::visitEnd ( SARChord& elt )
{
	if (!fTieChord) clonevisitor::visitEnd(elt);
}


//_______________________________________________________________________________
void seqCleaner::visitStart	( SARNote& elt )
{
	if (!elt->implicitDuration()) fCurrentDuration = elt->duration();
	fEndTags.clear();		// any note between repeat begin and repeat end cancels xxxBegin|End analysis
	if (fTieChord) return;	// tied notes inside chords are handled at chord level
	if (fInTie) {			// we should get 2 notes to merge
		if (fFirstTied)	{						// we've already got the first one
			*fFirstTied += fCurrentDuration;		// add the current duration to this first note
		}
		else {									// this is the first one
			fFirstTied = copy(elt);				// make a copy ans store it for future use
			*fFirstTied = fCurrentDuration;		// makes explicit duration
			clonevisitor::push(fFirstTied);		// and push it 
		}
	}
	else clonevisitor::visitStart (elt);
}

//______________________________________________________________________________
// seq operations
//_______________________________________________________________________________
SARMusic seqOperation::operator() ( const SARMusic& score1, const SARMusic& score2 )
{
	fCurrentDuration = rational(1,4);
	Sguidoelement outscore = ARFactory::instance().createMusic();
	if (outscore) {
		push (outscore);
		
		Sguidoelement sc1 = score1 ? Sguidoelement(score1) : outscore;
		Sguidoelement sc2 = score2 ? Sguidoelement(score2) : outscore;

		ctree<guidoelement>::literator s1i = sc1->lbegin();
		ctree<guidoelement>::literator s2i = sc2->lbegin();

		fFirstInSecondScore = true;
		tree_browser<guidoelement> browser(this);
        // browse voice by voice in parallel		
        while ((s1i != sc1->lend()) && (s2i != sc2->lend())) {
			fRangeTags.clear();
			fPosTags.clear();
			fOpenedTags.clear();
			if (s1i != sc1->lend()) {
				fState = kInFirstScore;
				browser.browse(*(*s1i));
				s1i++;
			}
			if (s2i != sc2->lend()) {
				fState = kInSecondScore;
                countvisitor<SARKey> keys;
				if (fPosTags["key"] && !keys.count(*s2i)) {
					Sguidotag k = ARFactory::instance().createTag("key");
					Sguidoattribute attr = guidoattribute::create();
					attr->setValue(0L);
					k->add(attr);
					visitStart (k);
				}
				browser.browse(*(*s2i));
				s2i++;
			}
		}
		fState = kRemainVoice;
		for (; s1i != sc1->lend(); s1i++)	{ browser.browse(*(*s1i)); }
		for (; s2i != sc2->lend(); s2i++)	{ browser.browse(*(*s2i)); }
#if 0		
 cerr << "----------------------------------" << endl;
 cerr << outscore << endl;
 cerr << "----------------------------------" << endl;
#endif
		seqCleaner	cleanTags;			// clean closed tags (i.e. handle slurs, ties, repeats...)
		tree_browser<guidoelement> clean(&cleanTags);
		clean.browse (*outscore);
		outscore = cleanTags.result();

		closedRemover rmTags;			// remove remaining closed tags (currently matched repeats)
		tree_browser<guidoelement> rm(&rmTags);
		rm.browse (*outscore);
		outscore = rmTags.result();
	}
	return dynamic_cast<ARMusic*>((guidoelement*)outscore);
}

//________________________________________________________________________
void seqOperation::storeTag(Sguidotag tag)
{
	const string& name = tag->getName();
	if (tag->beginTag())
		fRangeTags[name] = tag;
	else if (tag->endTag())
		fRangeTags[tag->matchTag()] = (void*)0;
	else if (tag->size())
		fRangeTags[name] = tag;
	else {
		int type = tag->getType();
		bool store = 
			(type == kTBarFormat ) || 
			(type == kTBeamsAuto ) || 
			(type == kTBeamsOff ) || 
            (type == kTClef ) || 
            (type == kTKey ) || 
			(type == kTColor ) || 
			(type == kTComposer ) || 
			(type == kTDotFormat ) || 
			((type >= kTHeadsCenter) && (type <= kTKey)) ||
			(type == kTMeter ) || 
			((type >= kTNoteFormat) && (type <= kTPort)) ||
			(type == kTRestFormat ) || 
			((type >= kTStaff) && (type <= kTTempo)) ||
			(type == kTTitle ) || 
			(type == kTUnits );
		if (store) 	fPosTags[name] = tag;
	}
}

//________________________________________________________________________
void seqOperation::endTag(Sguidotag tag)
{
	const string& name = tag->getName();
	if (tag->size()) {
		fRangeTags[name] = (void*)0;
		if (markers::opened (tag) > 1) {
			Sguidotag copy = dynamic_cast<guidotag*>((guidoelement*)fStack.top());
			fOpenedTags[name] = copy;
		}
	}
}

//________________________________________________________________________
// check if a tag is already active i.e. if the same position tag has been found before
bool seqOperation::currentTag(Sguidotag tag, bool end)
{
	bool ret = false;
	const string& name = tag->getName();
	if (!tag->size()) {						// for position tags only
		Sguidotag cur = fPosTags[name];		// look for an previous similar tag
		if (cur) {
			ret = (*cur == tag);			// check if equal
			if (end) fPosTags[name] = (void*)0;	// in case this is the tag end, remove the current one
		}
	}
	return ret;
}

//________________________________________________________________________
// check if 2 ties can be merged
bool seqOperation::compareContent (Sguidotag tag1, Sguidotag tag2)
{
	// opened ties should contain only one element: a note or a chord
	if ((tag1->size() == 1) && (tag2->size()==1)) {
		Sguidoelement e1 = tag1->elements()[0];
		Sguidoelement e2 = tag2->elements()[0];
		SARNote n1 = dynamic_cast<ARNote*>((guidoelement*)e1);
		SARNote n2 = dynamic_cast<ARNote*>((guidoelement*)e2);
		if (n1 && n1) {
			int octave = fCurrentOctave;
			int p1 = n1->midiPitch(fCurrentOctave);
			int p2 = n2->midiPitch(octave);
			return p1 == p2;
		}

		SARChord c1 = dynamic_cast<ARChord*>((guidoelement*)e1);
		SARChord c2 = dynamic_cast<ARChord*>((guidoelement*)e2);
		if (c1 && c2) {
			vector<int> plist1, plist2;
			int octave = fCurrentOctave;
			c1->midiPitch (fCurrentOctave, plist1);		// octave should to be explicit, 
			c2->midiPitch (octave, plist2);				// thus currentoctave could be ignored
			if (plist1.size() && (plist1.size() == plist2.size())) {
				for (unsigned int i=0; i<plist1.size(); i++) {
					if (plist1[i] != plist2[i]) return false;
				}
				return true;
			}
		}		
	}
	return false;
}

//________________________________________________________________________
// check if 2 opened tags match i.e. if their openness match (begin<->end)
bool seqOperation::checkmatch(Sguidotag tag1, Sguidotag tag2)
{
	if (markers::opened (tag2) & markers::kOpenedEnd) {
		if (tag2->getType() == kTTie) {
			if (fCurrentMatch == tag1) return true;		
			return compareContent (tag1, tag2);
		}
		else return true;
	}
	return false;
}

//________________________________________________________________________
// updates the current opened tag list
bool seqOperation::matchOpenedTag(Sguidotag tag, bool end)
{
	int type = markers::opened (tag);
	if (type & markers::kOpenedBegin) {					// first check the tag openness status
		Sguidotag match = fOpenedTags[tag->getName()];	// and look for a previously similar opened tag
		if (match) {									// we've found one

			if (checkmatch (tag, match)) {				// and we check if they match i.e. if their openness match (begin<->end)
				if (end) {								// done with the current match
														// the opened marker should be updated
					markers::setMark (match, (markers::opened (match)==markers::kOpenedEnd) ? markers::kClosed : markers::kOpenedBegin);
					fOpenedTags[tag->getName()] = (void*)0;	// the tag is removed from the opened tag list
					fCurrentMatch = (void*)0;				// and the current match is cleared
					fFirstInSecondScore = false;
				}
				else {									// that's the beginning of a match
					fCurrentMatch = tag;				// store the first tag of the matching pair
					match->push (tag->elements());		// transfer elements to matched tag
					tag->clear();						// clears the current tag
				}
				return true;
			}
		}
	}
	return false;
}

//________________________________________________________________________
// The visit methods
//________________________________________________________________________
void seqOperation::visitStart ( Sguidotag& elt )
{
	switch (fState) {
		case kInFirstScore:
			clonevisitor::visitStart (elt);
			storeTag (elt);
			break;
		case kRemainVoice:			
		case kInSecondScore:			
			if (!fFirstInSecondScore)
				clonevisitor::visitStart (elt);
			else if (!matchOpenedTag (elt) && !currentTag(elt))
				clonevisitor::visitStart (elt);
			break;
	}
}

//________________________________________________________________________
void seqOperation::visitEnd	( Sguidotag& elt )
{
	switch (fState) {
		case kInFirstScore:			
			endTag (elt);
			clonevisitor::visitEnd (elt);
			break;
		case kRemainVoice:			
		case kInSecondScore:			
			if (!fFirstInSecondScore)
				clonevisitor::visitEnd (elt);
			else if (!matchOpenedTag (elt, true) && !currentTag(elt))
				clonevisitor::visitEnd (elt);
			break;
	}
}

//________________________________________________________________________
// notes are visited for correct duration link
void seqOperation::visitStart ( SARNote& elt ) 
{ 
	bool done = false;
	rational duration = elt->duration();
	int octave = elt->GetOctave();

	if (fState == kInFirstScore) {
		// clear opened tags since only the last note is concerned
		fOpenedTags.clear();
		// maintain the current duration status while in the first score
		if (!ARNote::implicitDuration (duration))
			fCurrentDuration = duration;
		// maintain the current octave number while in the first score
		if (!ARNote::implicitOctave (octave))
			fCurrentOctave = octave;
	}
	else if (fState == kInSecondScore) {
		SARNote	note = copy (elt);
		push( note );
		done = true;

		if (fFirstInSecondScore) {		// force implicit values in second score when unspecified
			 if (ARNote::implicitOctave (octave)) {
				note->SetOctave(ARNote::kDefaultOctave);
			}
			if ( ARNote::implicitDuration (duration) && (fCurrentDuration != ARNote::getDefaultDuration()) ) {
				*note = ARNote::getDefaultDuration();		// force explicit default duration
			}
			fFirstInSecondScore = false;
		}
	}
	if (!done) clonevisitor::visitStart (elt);
}

// end bars are skipped for the first score
void seqOperation::visitStart ( SAREndBar& elt ) 
{ 
	Sguidotag tag = elt;
	switch (fState) {
		case kRemainVoice:			
		case kInSecondScore:			
			clonevisitor::visitStart (tag);
			break;
		default:
			break;
	}
}

// a voice is created only for the first score
void seqOperation::visitStart ( SARVoice& elt ) 
{ 
	fCurrentOctave = ARNote::kDefaultOctave;
	fFirstInSecondScore = true;
	switch (fState) {
		case kInFirstScore:	
		case kRemainVoice:			
			clonevisitor::visitStart (elt);
			break;
		default:
			break;
	}
}

// a voice is close only for the second score
void seqOperation::visitEnd   ( SARVoice& elt )
{ 
	switch (fState) {
		case kInSecondScore:			
		case kRemainVoice:			
			clonevisitor::visitEnd (elt);
			break;
		default:
			break;
	}
}

} // namespace
