/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004 	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

// - Standard C++ 
#include <sstream>		// Deprecated ?
#include <fstream>
#include <iostream>
#include <algorithm>
#include "secureio.h"

#include "ARMeter.h"
#include "ARMusic.h"
#include "ARMusicalVoice.h"
#include "ARNote.h"
#include "ARPageFormat.h"
#include "ARSpace.h"
#include "GObject.h"
#include "GRBar.h"
#include "GRDefine.h"
#include "GRFixVisitor.h"
#include "GRMusic.h"
#include "GRPage.h"
#include "GRPrintVisitor.h"
#include "GRSingleNote.h"
#include "GRStaff.h"
#include "GRStaffManager.h"
#include "GRSystem.h"
#include "GRSystemSlice.h"
#include "GRTrillsLinker.h"
#include "kf_ivect.h"
#include "TCollisions.h"
#include "VGDevice.h"

using namespace std;

// --------------------------------------------------------------------------
GRMusic::GRMusic(const ARMusic * ar, const ARPageFormat * inFormat, const GuidoLayoutSettings *settings, bool ownsAR )
  : GREvent( 0, ar, ownsAR )
{	
	assert( ar );
	fInFormat = 0;
    mAR2GRTime = -1;
    mDrawTime  = -1;
	fLyricsChecked = false;
	GuidoGetDefaultLayoutSettings (&fSettings);
	createGR(inFormat, settings );
}

// --------------------------------------------------------------------------
GRMusic::~GRMusic()
{
	DeleteContent( &mPages );
	// deletes all, because voicelist owns it elements which are GRVoice-Objects
	DeleteContent( &mVoiceList );
	delete fInFormat;
}

// --------------------------------------------------------------------------
float GRMusic::getNotesDensity () const
{
	float density = 0;
	for (int i= 0; i < getNumPages(); i++) {
		GRPage * page = mPages[i];
		density += page->getNotesDensity();
	}
	return density / getNumPages();
}

// --------------------------------------------------------------------------
bool GRMusic::checkLyricsCollisions()
{
	size_t n = checkCollisions(true);
	if (n) {
		resolveCollisions (getCollisions());
		fLyricsChecked = true;
		return true;
	}
	return false;
}

// --------------------------------------------------------------------------
size_t GRMusic::checkCollisions(bool lyrics)
{
	fCollisions.clear();
	for (int i= 0; i < getNumPages(); i++) {
		GRPage * page = mPages[i];
		page->checkCollisions(fCollisions, lyrics);
	}
	return fCollisions.count();
}

//-------------------------------------------------------------------------------
static bool sortByDate (const TCollisionInfo& i, const TCollisionInfo& j) {
	return (i.date() < j.date());
}

//-------------------------------------------------------------------------------
vector<TCollisionInfo> GRMusic::strip (const vector<TCollisionInfo>& list) const {
	vector<TCollisionInfo> outlist;
	const TCollisionInfo* max = 0;
	size_t n = list.size();
	for (size_t i = 1; i < n; i++) {
		if (list[i-1].date() == list[i].date()) {
			// same date: keep the largest gap
			if (max) {
				if (list[i].space() > max->space())
					max = &list[i];
			}
			else max = (list[i-1].space() > list[i].space()) ? &list[i-1] : &list[i];
		}
		else {
			if (max) {
				outlist.push_back(*max);
				max = 0;
			}
			else outlist.push_back(list[i-1]);
		}
	}
	if (max) outlist.push_back(*max);
	else outlist.push_back(list[n-1]);
	return outlist;
}

//static void print (ostream& out, const vector<TCollisionInfo>& list) {
//	for (size_t i = 0; i < list.size(); i++)
//		out << list[i] << endl;
//}

//-------------------------------------------------------------------------------
void GRMusic::resolveCollisions (vector<TCollisionInfo> list)
{
//	cerr << "before sort: \n---------------------------" << endl;
//	::print (cerr, list);
	sort (list.begin(), list.end(), sortByDate);
//	cerr << "after sort : \n---------------------------" << endl;
//	::print (cerr, list);
	list = strip (list);
//	cerr << "after strip : \n---------------------------" << endl;
//	::print (cerr, list);
	
	for (size_t i=0; i< list.size(); i++) {
		TCollisionInfo ci = list[i];
		ARMusicalVoice * voice = getARVoice (ci.fVoice - 1);		// look for the collision voice
		if (voice) {
			GuidoPos pos = voice->GetElementPos(ci.fARObject);		// get the colliding position
			if (pos) {
				ARMusicalObject* ar = voice->GetAt(pos);			// and the corresponding ar objet
				if (ar->isARBar()) {								// skip barlines (to put the space after)
					voice->GetNextObject(pos);
					if (pos) voice->AddElementAfter (pos, ci.fSpace);	// and add the space tag
				}
				else voice->AddElementAfter (pos, ci.fSpace);		// add the space tag
			}
			else {		// colliding position not found, maybe in the position tags list
				pos = voice->getPositionTagPos(dynamic_cast<const ARPositionTag*>(ci.fARObject));
				if (pos) {
					TYPE_TIMEPOSITION d = ci.fARObject->getRelativeTimePosition();
					pos = voice->GetHeadPosition();
					GuidoPos prev = pos;
					while (pos) {		// browse the voice up to the corresponding date
						const ARMusicalObject* obj = voice->GetNextObject(pos);
						// place the space tag before the next note
						if (obj->isARNote() && (obj->getRelativeTimePosition() > d)) {
							voice->AddElementAt(prev,  ci. fSpace);
							break;
						}
						prev = pos;
					}
				}
			}
		}
	}
	if (list.size()) createGR();
}

//-------------------------------------------------------------------------------
void GRMusic::removeAutoSpace(ARMusic * arm)
{
	GuidoPos pos = arm->GetHeadPosition();
	while (pos) {
		ARMusicalVoice * voice = arm->GetNext(pos);
		if (voice) {
			GuidoPos vpos = voice->GetHeadPosition();
			while (vpos) {
				ARMusicalObject* obj = voice->GetNextObject(vpos);
				ARSpace* space = obj ? obj->isARSpace() : 0;
				if (space && space->getIsAuto())
					voice->RemoveElement (obj);
			}
		}
	}
}

// --------------------------------------------------------------------------
void GRMusic::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	if (visitor.voiceMode()) {
		for (size_t i = 0; i<mVoiceList.size(); i++) {
			GRVoice* voice = mVoiceList[i];
			if (voice) voice->accept(visitor);
		}
	}
	else {
		for (int i= 0; i < getNumPages(); i++) {
			GRPage * page = mPages[i];
			page->accept (visitor);
		}
	}
	visitor.visitEnd (this);
}

// --------------------------------------------------------------------------
/** \brief Draws a page of music according to input drawing parameters.

	\param hdc a graphic device context object.
	\param inDrawInfos informations about what and how to draw.

	Do nothing if required page can't be found.
*/
void GRMusic::DrawMusic( VGDevice & hdc, const GuidoOnDrawDesc & inDrawInfos )
{
	const GRPage * drawpage = getPage(inDrawInfos.page);

	GRBar::reset();
	if (drawpage)
		drawpage->OnDraw(hdc, inDrawInfos);

//	trace(hdc);
}
	
// --------------------------------------------------------------------------
void GRMusic::GetMap( int inPage, float w, float h, GuidoElementSelector sel, MapCollector& f ) const
{
	const GRPage * page = getPage( inPage );
	if( page ) {
		MapInfos infos;
		page->getScaling(w, h);
		infos.fScale.x = w;
		infos.fScale.y = h;
		page->GetMap( sel, f, infos );
	}
}
	
void GRMusic::pagetrace(VGDevice & hdc)
{
	size_t n = mPages.size();
	for (size_t i=0; i < n; i++) {
		cerr << "page " << i << endl;
		const SystemPointerList * sysl = mPages[i]->getSystems();
		if (sysl) {
			size_t m = sysl->size();
			for (size_t j=0; j < m; j++) {
				cerr << "system --- " << j << endl;
				const GRSystem* sys = (*sysl)[j];
				const StaffVector * staves = sys->getStaves();
				if (staves) {
					for (int i = staves->GetMinimum(); i < staves->GetMaximum(); i++) {
						const GRStaff * staff = staves->Get(i);
						if (staff) {
							cerr << "staff --- " << i << endl;
							const NEPointerList& elts = staff->getElements();
							GuidoPos pos = elts.GetHeadPosition();
							while (pos) {
								const GRNotationElement* ne = elts.GetNext(pos);
								cerr << ne << endl;
							}
						}
						else cerr << "?? staff " << i << " NULL" << endl;
					}
				}
			}
		}
		else cerr << "?? page " << i << " systems NULL" << endl;
	}
}

void GRMusic::voicetrace(VGDevice & hdc)
{
    int n = getNumVoices();
    for (int i = 0; i < n; i++){
		GRVoice * voice = getVoice(i);
		if (voice) {
			cerr << "voice " << i << endl;
			const NEPointerList & elts = voice->GetCompositeElements();
			GuidoPos pos = elts.GetHeadPosition();
			while (pos) {
				const GRNotationElement* ne = elts.GetNext(pos);
				cerr << ne << endl;
			}
		}
		else cerr << "?? voice " << i << " is NULL" << endl;
	}

//    GRVoice * voice = getVoice(0);
//    if (voice) {
//        NVRect r (voice->getBoundingBox());
//        r += voice->getPosition();
//        cerr << "Voice trace - voice " << r << endl;
//    }
//    else cerr << "==> voice is NULL" << endl;
}
	
void GRMusic::trace(VGDevice & hdc)
{
    cerr << "==> GRMusic content" << endl;
    cerr << "    num pages  : " << getNumPages() << endl;
    pagetrace(hdc);
    cerr << "\n    num voices : " << getNumVoices() << endl;
    voicetrace(hdc);
}

// --------------------------------------------------------------------------
/** \brief Adds a page to the page list.
*/
void GRMusic::addPage(GRPage * newPage)
{
	assert(newPage);
	mPages.push_back(newPage);
}

// --------------------------------------------------------------------------
const ARMusic * GRMusic::getARMusic() const
{
	return /*dynamic*/static_cast<const ARMusic*>(getAbstractRepresentation());
}

// --------------------------------------------------------------------------
const ARMusic * GRMusic::getconstARMusic() const
{
	return /*dynamic*/static_cast<const ARMusic*>(getAbstractRepresentation());
}

// --------------------------------------------------------------------------
void GRMusic::updateBoundingBox()
{
	if (mPages.empty()) return;

	mBoundingBox.Set( 0, 0, 1, 1 );

	for( PageList::iterator ptr = mPages.begin(); ptr != mPages.end(); ++ptr )
	{
		addToBoundingBox( *ptr );
		// NVRect eltBox ( e->getBoundingBox());
		// eltBox += e->getPosition();

		// mBoundingBox.Merge( eltBox );
	}

	// This adds some space to the right and at the bottom.
	// The NoteServer can display a nicer picture using this ....
	// maybe this needs to be changed later!?
	
	// (JB) Additionnal margins could be passed as argument, but for
	// now, I need the exact bounding rectangle, so I put following two lines in comments.
	
		// mBoundingBox.right += (GCoord)(5 * LSPACE);
		// mBoundingBox.bottom += (GCoord)(5 * LSPACE);
}

// -----------------------------------------------------------------------------
/** \brief Returns the horizontal size of input page, in internal (logical) units.
	\param 	inPageNum >= 1
	\return 0 if the page does not exist..
*/
float GRMusic::getPageWidth( int inPageNum ) const
{
	const GRPage * page = getPage( inPageNum );
	return page ? page->getPageWidth() : 0;
}

// -----------------------------------------------------------------------------
/** \brief Gives the vertical size of input page, in internal (logical) units.
	\param 	inPageNum >= 1
	\return 0 if the page does not exist..
*/
float GRMusic::getPageHeight( int inPageNum ) const
{
	const GRPage * page = getPage( inPageNum );
	return page ? page->getPageHeight() : 0;
}

// -----------------------------------------------------------------------------
/** \brief Return the nth GRPage instance.

	\param 	inPageNum >= 1
	\return a page instance, or 0 if the page does not exist.
*/
const GRPage * GRMusic::getPage( int inPageNum ) const
{
	if(( inPageNum > 0 ) && ( inPageNum <= getNumPages()))
		return mPages[ (size_t)inPageNum - 1 ];
	else
		return 0;
}

// -----------------------------------------------------------------------------
/** \brief Return the nth GRPage instance.

	\param 	inPageNum >= 1
	\return a page instance, or 0 if the page does not exist.
*/
GRPage * GRMusic::getPage( int inPageNum ) 
{
	if(( inPageNum > 0 ) && ( inPageNum <= getNumPages()))
		return mPages[ (size_t)inPageNum - 1 ];
	else
		return 0;
}

// -----------------------------------------------------------------------------
/** \brief Returns the number of pages of music.
*/
int GRMusic::getNumPages() const
{
	return (int)mPages.size();
}

// -----------------------------------------------------------------------------
/** \brief Returns the number of pages of music.
*/
int GRMusic::getNumSystems(int pagenum) const
{
	if(( pagenum > 0 ) && ( pagenum <= getNumPages())) {
		GRPage* page = mPages[ (size_t)pagenum - 1 ];
		return int(page->getSystems()->size());
	}
	return guidoErrBadParameter;
}

// --------------------------------------------------------------------------
/** \brief Adjusts the pagesizes so that they fit the music on them ...
	this is done with the bounding_rectangle.
	
	Called when in NoteServer-Mode.
*/
void GRMusic::adjustPageSize()
{
	for( PageList::iterator ptr = mPages.begin(); ptr != mPages.end(); ++ptr )
	{
		GRPage * page = *ptr;
		page->adjustPageSize();
	}
}

// --------------------------------------------------------------------------
/** \brief Not yet implemented.
*/
void GRMusic::setSpringParameter(float npar)
{
}

// --------------------------------------------------------------------------
const NVstring & GRMusic::getName()
{
	if (mName.length() == 0)
		return getARMusic()->getName();
	return mName;
}

// --------------------------------------------------------------------------
/** \brief Returns the nth GRVoice of our voice list.

	zero based
*/
GRVoice * GRMusic::getVoice(int num)
{
	if(( num < 0 ) || num >= (int)mVoiceList.size())
		return 0;

	return mVoiceList[ (size_t)num ];
}

// --------------------------------------------------------------------------
/** \brief Returns the GRVoice corresponding to input musical voice.
*/
GRVoice * GRMusic::getVoice(const ARMusicalVoice * arv)
{
	for( VoiceList::iterator ptr = mVoiceList.begin(); ptr != mVoiceList.end(); ++ptr )
	{
		GRVoice * voice = *ptr;
		if( voice->getARMusicalVoice() == arv )
			return voice;
	}
	return 0;
}

// --------------------------------------------------------------------------
/** \brief Adds a notation-element to a GRVoice
*/
void GRMusic::addVoiceElement( int num, GRNotationElement * el )
{
	addVoiceElement( getVoice( num ), el );
}

// --------------------------------------------------------------------------
/** \brief Adds a notation element to a voice of music.
*/
void GRMusic::addVoiceElement(const ARMusicalVoice * arv, GRNotationElement * el )
{
	addVoiceElement( getVoice( arv ), el );
}

// --------------------------------------------------------------------------
/** \brief Adds a notation element to a voice of music.
*/
void GRMusic::addVoiceElement( GRVoice * voice, GRNotationElement * el )
{
	if( el == 0 )
	{
//		GuidoTrace("WARNING, trying to add a NULL element to voice!" );
		return;
	}

	if( voice )
		voice->AddTail(el);
}

// --------------------------------------------------------------------------
/** \brief Remove a notation element from a voice of music.
*/
void GRMusic::removeVoiceElement(ARMusicalVoice * arv, GRNotationElement * el)
{
	GRVoice * voice = getVoice(arv);
	if (voice)
		voice->RemoveElement(el);
}

// --------------------------------------------------------------------------
/** \brief Returns the number of voices of the music.
*/
int GRMusic::getNumVoices() const
{
	return (int)mVoiceList.size();
}

// --------------------------------------------------------------------------
/** \brief Called from the GRVoiceManager to tell
	a voice to prepare itself for the possibility of
	 a newline at the current position.
*/
void GRMusic::setPossibleVoiceNLinePosition(const ARMusicalVoice * arv, const TYPE_TIMEPOSITION & tp)
{
	GRVoice * voice = getVoice(arv);
	if (voice)
		voice->setPossibleNLinePosition(tp);
}

// --------------------------------------------------------------------------
/** \brief Called to remember a previously saved NLine-Position.
*/
void GRMusic::rememberVoiceNLinePosition(const ARMusicalVoice * arv, const TYPE_TIMEPOSITION & tp)
{
	GRVoice * voice = getVoice(arv);
	if (voice)
	{
		voice->rememberNLinePosition(tp);
	}
}

// --------------------------------------------------------------------------
/** \brief Called from GRPage whenever a new system is created.
	This needs to be done, so that the GRVoice(s) know, where it starts.

	This is never called !?
*/
void GRMusic::startNewSystem(GRSystem * grsystem)
{
	for( VoiceList::iterator ptr = mVoiceList.begin(); ptr != mVoiceList.end(); ++ptr )
	{
		GRVoice * voice = *ptr;
		voice->startNewSystem(grsystem);
	}
}

// --------------------------------------------------------------------------
/** \brief Creates the Graphical Representation from the Abstract Representation.
	
	Part of the AR to GR process.

	It works as follows:
	
	1. Reset the current graphical representation, if any. Delete pages and voices.
	2. Creates the new GR voices.
	3. Build a GR StaffManager.
	4. Let the StaffManager handle the creation of staves and pages,
	   	let him call AddSystem or DoPageBreak of this class.
*/
void GRMusic::createGR (const ARPageFormat * inPageFormat, const GuidoLayoutSettings * settings)
{
	if (inPageFormat) {
		if(fInFormat) delete fInFormat;
		fInFormat = new ARPageFormat(*inPageFormat);
	}
	if (settings) fSettings = *settings;

	// - Reset the previous GR
	const ARMusic * arm = getARMusic();
	DeleteContent( &mPages );
	DeleteContent( &mVoiceList );

	// - Creates new voices
	GuidoPos pos = arm->GetHeadPosition();
	while (pos)
	{
		ARMusicalVoice * arv = arm->GetNext(pos);
		GRVoice * grv = new GRVoice(arv, false);
		mVoiceList.push_back(grv);
	}
	GRStaffManager grsm( this, fInFormat, &fSettings);
	grsm.createStaves();
	fLyricsChecked = false;

	// intended to fix tellPosition order issues
	GRFixVisitor ffix;
	this->accept( ffix );

//cerr << "GRMusic: ---------- visit ---------" << endl;
	GRTrillLinker v;
	accept (v);
//	float d = getNotesDensity();
//cerr << "GRMusic::createGR density: " << d << endl;

//cerr << "GRMusic: ---------- voices ---------" << endl;
//printVoices(cerr);
//cerr << "\n---------- pages ----------" << endl;
//print(cerr);
}

//-------------------------------------------------------------------------------
/** \brief Prints all pages of music.
*/
void GRMusic::print(std::ostream& os) const
{
	for( size_t i = 0; i < mPages.size(); i++ ) {
		os << "Page " << i << endl;
		mPages[i]->print(os);
	}
}

//-------------------------------------------------------------------------------
void GRMusic::printVoices (std::ostream& os) const
{
	for (size_t i = 0; i<mVoiceList.size(); i++) {
		GRVoice* voice = mVoiceList[i];
		if (voice) {
			cerr << "Voice " << i << endl;
			GuidoPos first = voice->First();
			GuidoPos last = voice->Last();
			while (first != last) {
				GRNotationElement * e = GetNext(first);
				cerr << "     " << e << endl;
			}
		}
	}
}

/** \brief If given size is negative, we remove corresponding staff size in map
 */
void GRMusic::setStaffSize(int staffNum, float size) {
    if (size < 0)
        fStaffSizes.erase(staffNum);
    else
        fStaffSizes[staffNum] = size;
}

/** \brief If size of given staff is not set, we return -1.
 */
float GRMusic::getStaffSize(int staffNum) {
    if (fStaffSizes.find(staffNum) == fStaffSizes.end())
        return -1;
    else
        return fStaffSizes[staffNum];
}

//-------------------------------------------------------------------------------
ARMusicalVoice* GRMusic::getARVoice (int n)
{
	const ARMusic * arm = getARMusic();
	GuidoPos pos = arm->GetHeadPosition();
	while (pos) {
		ARMusicalVoice * voice = arm->GetNext(pos);
		if (!n--) return voice;
	}
	return 0;
}

// --------------------------------------------------------------------------
/** \brief Finds the index of a given voice (zero based)

	\return a voice index, or -1 if no voice found.
*/
int GRMusic::getVoiceNum(ARMusicalVoice * arv) const
{
	int cnt = 0;

	for( VoiceList::const_iterator ptr = mVoiceList.begin(); ptr != mVoiceList.end(); ++ptr )
	{
		GRVoice * voice = *ptr;
		if( voice->getARMusicalVoice() == arv )
			return cnt;

		++cnt;
	}
	return -1;
}

// --------------------------------------------------------------------------
/** \brief Not yet implemented.
*/
void GRMusic::MarkVoice(int voicenum, int numfrom, int denomfrom, int numlength, int denomlength, unsigned char red, unsigned char green, unsigned char blue)
{
}

// --------------------------------------------------------------------------
/** \brief Finds the page of the element which date matches the input time pos.

	Input time pos must match an existing element.
*/
int GRMusic::getPageNum(int num, int denom) const
{
	// we have to find the position of the elements
	for( VoiceList::const_iterator ptr = mVoiceList.begin(); ptr != mVoiceList.end(); ++ptr )
	{
		GRVoice * voice = *ptr;
		if (voice)
		{
			GRPage * page = voice->getPageNum( num, denom );
			if( page == 0 )
				return guidoErrActionFailed;

			return getPageIndex( page );
		}
	}
	return guidoErrActionFailed;	
}

// --------------------------------------------------------------------------
/** \brief Finds the page which contains input time pos.

	\return a page index, or 0 if no page found.
*/
int GRMusic::getPageNumForTimePos( int num, int denom ) const
{
	const TYPE_TIMEPOSITION inDate ( num, denom );
	if( inDate > getDuration()) return 0; // check if in the music duration range

	const int pageCount = getNumPages();
	for( int index = 2; index <= pageCount; ++index )
	{
		const GRPage * page = getPage( index );
		if( page == 0 ) return 0;
	
		const TYPE_TIMEPOSITION & pageDate = page->mDebugPageDate; //page->getRelativeTimePosition();
		if( pageDate > inDate )
			return (index - 1);

	}
	return pageCount;
}

// --------------------------------------------------------------------------
/** \brief Finds the number of a given page.

	\return a page index, or 0 if no page was found.
*/
int	GRMusic::getPageIndex( const GRPage * inPage ) const
{
//	if( inPage == 0 ) return 0; 		// Following code already returns 0 when inPage == 0 ?

	int count = 1;
	for( PageList::const_iterator ptr = mPages.begin(); ptr != mPages.end(); ++ptr )
	{
		if( *ptr == inPage )
			return count;
		
		++ count;
	}
	return 0;
}

// --------------------------------------------------------------------------
/** \brief Returns the relative time position of given page.

	return false if input page was not found.
*/
bool GRMusic::getRTPofPage( int pagenum, int * num, int * denom ) const
{
	const GRPage * page = getPage(pagenum);
	if (page)
	{
		const TYPE_TIMEPOSITION tp (page->getRelativeTimePosition());
		*num = tp.getNumerator();
		*denom = tp.getDenominator();
		return true;
	}
	return false;
}

