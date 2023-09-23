#ifndef GRText_H
#define GRText_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>
#include <vector>

#include "GRVisitor.h"
#include "GRPTagARNotationElement.h"

class ARText;
class GRStaff;
class GRRod;
class VGFont;

/** \brief A text string.

	Can represent standard text, lyrics, fingering, label, cue-text, marks...

*/
class GRText : public GRPTagARNotationElement
{	
	FloatRect fTextRect;

	public:
		class GRTextSaveStruct : public GRPositionTag::GRSaveStruct
		{
			public:
						GRTextSaveStruct()  {}
				virtual ~GRTextSaveStruct() {};
				
				NVPoint position;
				NVRect boundingBox;
				std::string text;
		};

						 GRText( GRStaff *, const ARText * abstractRepresentationOfText );
		virtual 		~GRText();

		virtual void	accept (GRVisitor& visitor);
		virtual void 	removeAssociation( GRNotationElement * el );
		virtual void 	tellPosition( GObject * caller, const NVPoint & inPosition );
		virtual void 	addAssociation( GRNotationElement * el );
		
		virtual void 	OnDraw( VGDevice & hdc ) const;

		virtual FloatRect getTextMetrics(VGDevice & hdc, const GRStaff* staff) const;
		virtual float 	getLineHeight(VGDevice & hdc) const;
		const ARText * 	getARText() const;
	
		virtual unsigned int getTextAlign() const { return mTextAlign; }

		virtual float 	getLeftSpace() const;
		virtual float 	getRightSpace() const;

		virtual void 	setPosition(const NVPoint & inPosition );
		virtual void 	setHPosition( float nx );
				void	mustFollowPitch( bool flag ) { mMustFollowPitch = flag; }

		virtual bool	checkCollisionWith() const	{ return true; }
		virtual bool	isLyrics() const;
		virtual const GRText*	isText() const			{ return this; }

	protected:
		virtual const VGColor 	startDraw( VGDevice & hdc, unsigned int& fontalign ) const;
		virtual void			endDraw( VGDevice & hdc, const VGColor, unsigned int fontalign) const;
	
		virtual GRPositionTag::GRSaveStruct * getNewGRSaveStruct()
			{ return new GRTextSaveStruct; }	

		unsigned int mTextAlign;
		bool	mMustFollowPitch; // (when the text tag has a range)
		float	mStaffBottom;
		float 	fLineHeight = 0.f;

	private:
		std::vector<std::string> fSubstrings;
		const VGFont* fFont;
};

#endif

