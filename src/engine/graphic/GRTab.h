#pragma once

/*
  GUIDO Library
  Copyright (C) 2023 D. Fober

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
  
*/

#include <string>

#include "GRSingleNote.h"

/** \brief Graphical representation of a guitar tablature.
*/
class ARTab;
class GRStaff;

class GRTab : public GRSingleNote
{
	public:
	  			 GRTab (GRStaff * inStaff, const ARTab * tab, const TYPE_TIMEPOSITION & date, const TYPE_DURATION & duration );
	virtual 	~GRTab () {}

		const ARTab * getARTab() const;

		virtual void 	OnDraw( VGDevice & hdc ) const;

		virtual void 	recalcVerticalPosition() {}
		virtual void 	drawStemOnly( int flag ) {}
		virtual void 	setStemDirection( GDirection dir )				{};
		virtual float	setStemLength( float inLen, bool userLength)	{ return 0.f; }
		virtual GRGlobalStem * getGlobalStem() const 					{ return  nullptr; }
		virtual ARTHead::HEADSTATE adjustHeadPosition(ARTHead::HEADSTATE sugHeadState = ARTHead::NORMAL) { return ARTHead::NORMAL; }
		virtual bool	isTab() const				{ return true; }
		virtual int		getNumFaehnchen() const		{ return 0; }
		virtual void 	updateBoundingBox();
		virtual void	setNoteFormat(const ARNoteFormat * frmt);

	private:
		std::string fDisplay;
		float 		fSize;
};


