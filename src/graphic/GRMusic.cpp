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
#include "secureio.h"
using namespace std;

// - Guido AR
#include "ARMusic.h"
#include "ARNote.h"
#include "ARMeter.h"
#include "ARPageFormat.h"
#include "ARMusicalVoice.h"

// - Guido GR
#include "GRMusic.h"
#include "GRStaffManager.h"
#include "GRSingleNote.h"
#include "GRPage.h"
#include "GRDefine.h"

// - Guido Misc
#include "GuidoFeedback.h"
#include "GUIDOInternal.h" 	// for gGlobalSettings.gFeedback, AddGGSOutput
#include "VGDevice.h"

// - Guido debug
#include "GObject.h"
#include "GRSystem.h"
#include "GRSystemSlice.h"
#include "GRStaff.h"
#include "kf_ivect.h"

using namespace std;

// - Globals
GRMusic * gCurMusic = 0;

long ggsoffsetx = 0L;
long ggsoffsety = 0L;

GRMusic::GRMusic(ARMusic * inARMusic, ARPageFormat * inFormat, bool ownsAR ) // ,int ignorepf)
  : GREvent( 0, inARMusic, ownsAR ) // owns Elements!
	 //mPages( new PagePointerList(1)) // mit OWNS-Elements!!!
{	
	assert( inARMusic );

	gCurMusic = this;
	createGR( inFormat );
}

GRMusic::~GRMusic()
{
	if( gCurMusic == this )
		gCurMusic = 0;

	if( gGlobalSettings.gFeedback )
		gGlobalSettings.gFeedback->SetCancelButtonState( 0 );

	DeleteContent( &mPages );

	// deletes all, because voicelist owns it 
	// elements which are GRVoice-Objects
	DeleteContent( &mVoiceList );

	if( gGlobalSettings.gFeedback )
		gGlobalSettings.gFeedback->SetCancelButtonState( 1 );
}

/** \brief Guido Graphic Stream.
*/
char * GRMusic::getGGSInfo(int infotype) const
{
	char * buf = new char[100];
	strncpy( buf, "this is grmusic ...",100);
	return buf;
}

void GRMusic::GGSOutputPage( int inPageNum ) const
{
	const GRPage * curpage = getPage(inPageNum);
	if (curpage == 0) return;

	ggsoffsetx = 0;
	ggsoffsety = 0;

	char buffer[100];
	snprintf(buffer,100,"\\unit<%d>\n", (int)LSPACE/2);
	AddGGSOutput(buffer);

	curpage->GGSOutput();	
}

void GRMusic::OnDraw( VGDevice & hdc) const
{
  	// OnDraw( hdc,0);
}

/** \brief Draws a page of music according to input drawing parameters.

	\param hdc a graphic device context object.
	\param inDrawInfos informations about what and how to draw.

	Do nothing if required page can't be found.
*/
void GRMusic::OnDraw( VGDevice & hdc, const GuidoOnDrawDesc & inDrawInfos ) 
{
	const GRPage * drawpage = getPage( inDrawInfos.page );
	if( drawpage ) 
	{
		drawpage->OnDraw( hdc, inDrawInfos );
	}
}
	
void GRMusic::GetMap( int inPage, float w, float h, GuidoeElementSelector sel, MapCollector& f ) const 
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
	
void GRMusic::voicetrace(VGDevice & hdc)
{
    if (!getNumVoices()) return;
    GRVoice * voice = getVoice(0);
    if (voice) {
        NVRect r (voice->getBoundingBox());
        r += voice->getPosition();
        cout << "Voice trace - voice " << r << endl;
    }
    else cout << "==> voice is NULL" << endl;
}
	
void GRMusic::trace(VGDevice & hdc)
{
    cout << "==> GRMusic content" << endl;
    cout << "    num pages  : " << getNumPages() << endl;
    cout << "    num voices : " << getNumVoices() << endl;
    voicetrace(hdc);
}

/** \brief Adds a page to the page list.
*/
void GRMusic::addPage(GRPage * newPage)
{
	assert(newPage);
	mPages.push_back(newPage);
}

/** \brief Prints all pages of music.
*/
void GRMusic::print() const
{
	for( PageList::const_iterator ptr = mPages.begin(); ptr != mPages.end(); ++ptr )
	{
		const GRPage * page = *ptr;
		page->print();
	}
}

ARMusic * GRMusic::getARMusic()
{
	return /*dynamic*/static_cast<ARMusic*>(getAbstractRepresentation());
}

const ARMusic * GRMusic::getconstARMusic() const
{
	return /*dynamic*/static_cast<const ARMusic*>(getAbstractRepresentation());
}

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

/** \brief Converts from pixel (device) to virtual (logical) coordinates 

void GRMusic::convertToVirtualSize( VGDevice & hdc, int page, float zoom, 
											float * pi1, float * pi2 )
{
	GRPage * curpage = getPage(page);
	assert(curpage);
	curpage->convertToVirtualSize(hdc,zoom,pi1,pi2);
}
*/
/** \brief Converts from virtual (logical) to pixel (device) coordinates. 
	
	For example, to calculate, how big an GIF-Image is going to be, just set the
	parameters pi1 and pi2 to the VirtualSizes (obtained by
	calling getSizeX() and getSizeY(). After the function
	call, you get the corresponding pixel-sizes.

void GRMusic::convertToPixelSize( VGDevice & hdc, int page, float zoom, 
											float * pi1, float * pi2 )
{
	GRPage * curpage = getPage(page);
	assert(curpage);
	curpage->convertToPixelSize(hdc, zoom, pi1, pi2);
}
*/

/** \brief Not yet implemented.
*/
void GRMusic::setSpringParameter(float npar)
{
}

const NVstring & GRMusic::getName()
{
	if (mName.length() == 0)
		return getARMusic()->getName();
	return mName;
}

/** \brief Returns the nth GRVoice of our voice list.

	zero based
*/
GRVoice * GRMusic::getVoice(int num)
{
	if(( num < 0 ) || num >= (int)mVoiceList.size())
		return 0;

	return mVoiceList[ (size_t)num ];
}

/** \brief Returns the GRVoice corresponding to input musical voice.
*/
GRVoice * GRMusic::getVoice(ARMusicalVoice * arv)
{
	for( VoiceList::iterator ptr = mVoiceList.begin(); ptr != mVoiceList.end(); ++ptr )
	{
		GRVoice * voice = *ptr;
		if( voice->getARMusicalVoice() == arv )
			return voice;
	}
	return 0;
}

/** \brief Adds a notation-element to a GRVoice
*/
void GRMusic::addVoiceElement( int num, GRNotationElement * el )
{
	addVoiceElement( getVoice( num ), el );
}

/** \brief Adds a notation element to a voice of music.
*/
void GRMusic::addVoiceElement(ARMusicalVoice * arv, GRNotationElement * el )
{
	addVoiceElement( getVoice( arv ), el );
}

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

/** \brief Remove a notation element from a voice of music.
*/
void GRMusic::removeVoiceElement(ARMusicalVoice * arv, GRNotationElement * el)
{
	GRVoice * voice = getVoice(arv);
	if (voice)
		voice->RemoveElement(el);
}

/** \brief Returns the number of voices of the music.
*/
int GRMusic::getNumVoices() const
{
	return (int)mVoiceList.size();
}

/** \brief Called from the GRVoiceManager to tell
	a voice to prepare itself for the possibility of
	 a newline at the current position.
*/
void GRMusic::setPossibleVoiceNLinePosition(ARMusicalVoice * arv, const TYPE_TIMEPOSITION & tp)
{
	GRVoice * voice = getVoice(arv);
	if (voice)
		voice->setPossibleNLinePosition(tp);
}

/** \brief Called to remember a previously saved NLine-Position.
*/
void GRMusic::rememberVoiceNLinePosition(ARMusicalVoice * arv, const TYPE_TIMEPOSITION & tp)
{
	GRVoice * voice = getVoice(arv);
	if (voice)
	{
		voice->rememberNLinePosition(tp);
	}
}

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

/** \brief Creates the Graphical Representation from the Abstract Representation.
	
	Part of the AR to GR process.

	It works as follows:
	
	1. Reset the current graphical representation, if any. Delete pages and voices.
	2. Creates the new GR voices.
	3. Build a GR StaffManager.
	4. Let the StaffManager handle the creation of staves and pages,
	   	let him call AddSystem or DoPageBreak of this class.
*/
void GRMusic::createGR(ARPageFormat * inPageFormat)
{
	if( gGlobalSettings.gFeedback ) 
		gGlobalSettings.gFeedback->SetCancelButtonState(0);

	// - Reset the previous GR
	ARMusic * arm = getARMusic();
	arm->resetGRRepresentation();	// (JB) this may conflicts with the new 
									// "multi GR -> one AR" scheme.
	DeleteContent( &mPages );
	DeleteContent( &mVoiceList );

	gCurMusic = this;

	if( gGlobalSettings.gFeedback ) 
		gGlobalSettings.gFeedback->SetCancelButtonState(1);

	// - Creates new voices

	GuidoPos pos = arm->GetHeadPosition();
	while (pos)
	{
		ARMusicalVoice * arv = arm->GetNext(pos);
		GRVoice * grv = new GRVoice(arv, false);
		mVoiceList.push_back(grv);
	}

	// - Creates new staves and pages, using a staff manager.

	/* was ->
	GRStaffManager * grsm = new GRStaffManager(this, inPageFormat);
	if( grsm )
	{
		grsm->createStaves();
		delete grsm;
	}

	-> */
	
	GRStaffManager grsm( this, inPageFormat );
	grsm.createStaves();

	// <-
}

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

/** \brief Returns some code whether the operation was successful or not. 

	This is something completely new! Input to the Renderer ...
*/
int GRMusic::GGSInputPage(int page, const char * str)
{
	// first, we have to figure out, where we are,
	// then, we have to manipulate the Abstract Representation
	// and then we have to recreate the Graphical Represenation
	// this process can be done in an intelligent way "later"
	// right now, we just want to show that it basically works...

	// -> decipher the input 
	// how to we call the parser on strings?
	// what reactions do we get from the parser -> another parser-flag ...?

	// write our own parser for handling stuff ?
	// this is a very dumm/first approach to handling GGS-input

	printf("Inside GGSInput: %s\n",str);
  
	char type[250];
	type[0] = 0;
	int xpos = 0, ypos = 0, staffid = 0;
	long int id=0;
	int isadd = 0;
	if (!(strncmp(&str[1],"add",3)))
	{
		// this is an add-command ...
		printf("is an add\n");
		isadd = 1;
		// try to read the parameters ...
		// syntax \add<"type",xpos,ypos,staffid>

		int ret = sscanf(&str[6],"%249[^\"]\",%d,%d,%d>",type, &xpos,&ypos,&staffid);
		printf("ret returnd %d, %s,%d,%d,%d\n",ret,type,xpos,ypos,staffid);
	}
	else if (!strncmp(&str[1],"move",4))
    {
		// it is a move ...
		printf("is a move\n");
		// syntax \move<id,dx,dy>
		int ret = sscanf(&str[6],"%ld,%d,%d>",&id,&xpos,&ypos);
		printf("ret returned %d, %ld, %d, %d\n",ret,id,xpos,ypos);

		// now, we try to really move the object ....
		// this should work quite easily ...
		// first try: do it with an offset, next really change the height of notes 
		GRSingleNote * nt = dynamic_cast<GRSingleNote *>((GObject *) id);
		GREvent * ev = GREvent::cast((GObject *) id);
		GRNotationElement *el = dynamic_cast<GRNotationElement *>((GObject *) id);
		if (nt)
		{
			// we have a real note ....
			// offset is only set in x-direction, y-offset changes note-height
			nt->addToOffset(NVPoint(float(xpos), 0));

			// y-pos changes note-height ...
			ARNote * arnote = nt->getARNote();
			arnote->resetGRRepresentation();

			// this removes the elements from the note ...
			nt->removeElements();

			// the y-difference is calcualted to real half-steps ...
			int steps =  (int)(- ypos * 2 / LSPACE);
			printf("number of steps : %d\n",steps);

			// this sets the new pitch ...
			arnote->offsetpitch(steps);

			// this creates the new elements ...
			// a new stem, a new stemdirection and all that ...
			nt->doCreateNote(nt->getDuration());

#ifdef _DEBUG
			ofstream os("music.sal");
			getARMusic()->operator<<(os);
			// os << (*music);
			os.close();
#endif
		}
		else if (ev)
		{
			// we really have an element ...
			NVPoint position = ev->getPosition();
			printf("Name of object : %s\n",typeid(ev).name());
			ev->addToOffset(NVPoint(float(xpos), float(ypos)));
		}
		else if (el)
		{
			el->addToOffset(NVPoint(float(xpos), float(ypos)));
		}
	}
	else if (!strncmp(&str[1], "remove_image", 12))
	{
		// it is a delete ...
		printf("is a remove_image\n");
		// syntax \remove_image<id>
		int ret = sscanf(&str[14],"%ld>",&id);
		printf("ret returned %d, %ld\n",ret,id);
	}

	if (!isadd) return guidoErrActionFailed;  // (jb) why ? Only Adds can be successful ?
  											// (was: return 0; );

	// first dummy: we always add a quarter note at the end of the abstract representation:
	ARMusic * arm = getARMusic();
	arm->resetGRRepresentation();
	arm->removeAutoTags();

	// - Release all pages and voices
	DeleteContent( &mPages );
	DeleteContent( &mVoiceList );

	// - Adds a note to the end
	assert(arm);
	ARMusicalVoice * vc = arm->GetHead();
	assert(vc);
	ARNote * nt = new ARNote(ARNoteName("c"),0,1,1,4,80);
	vc->AddTail(nt);
	arm->doAutoStuff();

#ifdef _DEBUG
	ofstream os("music.sal");
	arm->operator<<(os);
	// os << (*music);
	os.close();
#endif
	createGR( 0 );
	return guidoNoErr;	// (JB) was: return 1;
}

void GRMusic::getGuido() const
{
	const ARMusic * arm = getconstARMusic();

	ostringstream os;		// was ostrstream os; (deprecated)

  // no AUTO-tags !

	arm->output( os,0 );
  
//#ifdef LINUX	
//  os.freeze();	// Deprecated: it was here because of old ostrstream.
//#endif

	size_t charCount = os.str().size(); // was: os.pcount();
	char * s = new char[ charCount +  2];

	const char * guidoString = os.str().c_str();
	if( guidoString && charCount)
		strncpy( s, guidoString, charCount );

	s[ charCount ] = '\n';
	s[ charCount ] = 0;

  AddGuidoOutput( s );

  delete [] s;
}

/** \brief Not yet implemented.
*/
void GRMusic::MarkVoice(int voicenum, int numfrom, int denomfrom, 
			int numlength, int denomlength,unsigned char red, unsigned char green, unsigned char blue)
{
}

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

/** \brief Finds the page which contains input time pos.

	\return a page index, or 0 if no page found.
*/
int GRMusic::getPageNumForTimePos( int num, int denom ) const
{
	const TYPE_TIMEPOSITION inDate ( num, denom );

// - version B: uses custom "debug" dates
	
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

// - version A: uses durations. bug: system durations are WRONG
/*	
	TYPE_TIMEPOSITION startDate = 0;
	TYPE_TIMEPOSITION endDate = 0;
	TYPE_TIMEPOSITION duration = 0;

	const int pageCount = getNumPages();
	for( int index = 1; index <= pageCount; ++index )
	{
		GRPage * page = getPage( index );
		if( page == 0 ) return 0;	

		duration = page->getDuration();
		endDate = startDate + duration;

		if(( inDate >= startDate ) && ( inDate < endDate )) // end date is exclusive
		{
			return index;
		}

		startDate += duration;
	}
	
	return 0;
*/
/*	uses voices events: bug: last system points to the next page bug.
	for( VoiceList::iterator ptr = mVoiceList.begin(); ptr != mVoiceList.end(); ++ptr )
	{
		GRVoice * voice = *ptr;
		if (voice)
		{
			GRPage * page = voice->getPageForTimePos( num, denom );
			if( page == 0 )
				return guidoErrActionFailed;
			
			return getPageIndex( page );
		}
	}
	return guidoErrActionFailed;*/	
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

/** \brief Sets the 'opt' force of the space-force algorithm.
*/
void GRMusic::setOptForce(float newoptforce)
{
	GRSpaceForceFunction2::setOptForce(newoptforce);
}

/** \brief Returns the 'opt' force of the space-force algorithm.
*/
float GRMusic::getOptForce()
{
	return GRSpaceForceFunction2::getOptForce();
}

