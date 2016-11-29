/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GROctava.h"
#include "GRStaff.h"
#include "GRDefine.h"

#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice
#include "FontManager.h"	// for gFontDefs.gFontText
#include "VGDevice.h"
#include "VGFont.h"
#include "GRSpecial.h"
#include "GRSingleNote.h"
#include "AROctava.h"

using namespace std;

GROctava::GROctava( GRStaff * staff, const NVstring & text, AROctava* arOctava)
//GROctava::GROctava( GRStaff * staff, const NVstring & text, bool bassa)
//	: fStaff(staff), fText (text), fBassa(bassa), fElement(0)
	: GRPTagARNotationElement(arOctava), fStaff(staff), fText (text), fBassa(arOctava->getOctava() < 0), fElement(0)
{
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = staff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->p = (void *) getNewGRSaveStruct();
	mStartEndList.AddTail(sse);

	mNeedsSpring = 1;
	float x = 0;
	float y = 0;
	if( gGlobalSettings.gDevice )
		FontManager::gFontText->GetExtent( fText.c_str(), fText.length(), &x, &y, gGlobalSettings.gDevice );

	mBoundingBox.right = (GCoord)(x);
	mBoundingBox.top = (GCoord)(y - staff->getStaffLSPACE());

	mBoundingBox.left -= (GCoord)(LSPACE/4);
	mBoundingBox.bottom = (GCoord)(4*LSPACE);
//cerr << "==> GROctava::GROctava " << getBoundingBox() << endl;
}

GROctava::~GROctava()
{
	if (fElement)
		fElement->removeAssociation(this);
}

void GROctava::GGSOutput() const
{
}

void GROctava::setColRef(const TagParameterString *tps) {
    if (!mColRef)
        mColRef = new unsigned char[4];

    tps->getRGB(mColRef);
}

void GROctava::OnDraw( VGDevice & hdc) const
{
//cerr << "GROctava::OnDraw staff bb " << fStaff->getBoundingBox() << endl;
    VGColor prevTextColor = hdc.GetFontColor();
    if (mColRef)
        hdc.SetFontColor(VGColor(mColRef));
	OnDrawText(hdc,  fText.c_str(), int(fText.length()) );
    if (mColRef)
        hdc.SetFontColor(prevTextColor);
}

void GROctava::addAssociation(GRNotationElement * el)
{
cerr << "GROctava::addAssociation " << *el << endl;
	// attention, otherwise this is cyclic!
#if 0
	GRNotationElement::addAssociation(el);
#else
	if (!fElement)
	{
		fElement = el;
		el->addAssociation(this);
	}
#endif
}

void GROctava::tellPosition(GObject * caller, const NVPoint & pos)
{

cerr << "GROctava::tellPosition " << pos << " assoc: " << getAssociations() << endl;
//	if (caller != getAssociations()->GetTail()) // useless dynamic cast ?
//		return;
//
//	GRSingleNote * note = dynamic_cast<GRSingleNote *>(getAssociations()->GetHead());
//	if (note)
//		mBoundingBox.left = note->getStemStartPos().x;

	setPosition(pos);
}
