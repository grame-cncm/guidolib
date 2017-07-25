/*
  GUIDO Library
  Copyright (C) 2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef ___GRTrillLinker___
#define ___GRTrillLinker___

#include <iostream>
#include <map>
#include "GRVisitor.h"

class GRNote;
class GREvent;
class GRNotationElement;

class ChordTrills
{
	public:
	typedef	std::map<GRNote*, GRTrill*> TTrills;
	
				 ChordTrills() {}
		virtual ~ChordTrills() {}
	
		void	add(GRNote* note, GRTrill* trill);
		void	check (ChordTrills& previous, GRBar* bar);
		void	check (GREvent* ev, GRBar* bar);
		TTrills::iterator	find (GRNote* note);
		TTrills::iterator	begin ()	{ return fTrills.begin(); }
		TTrills::iterator	end ()		{ return fTrills.end(); }
		GRNote* firstNote()				{ return fFirstNote; }
		void	print(std::ostream &) const;
	
	private:
		GRNotationElement* next(GREvent* note, GRBar* bar) const;
		bool sameNote (GRNote* n1, GRNote* n2) const;

		TTrills	fTrills;
		GRNote* fFirstNote = 0;
};
std::ostream& operator << (std::ostream & os, const ChordTrills&);

class GRTrillLinker : public GRVisitor
{
	public:
				 GRTrillLinker() { }
		virtual ~GRTrillLinker() { delete fPrevious; delete fCurrent; }
	
		virtual bool voiceMode () { return true; }

		virtual void visitStart (GRVoice* o);
		virtual void visitEnd   (GRVoice* o);

		virtual void visitStart (GRSingleNote* o);
		virtual void visitStart (GRSingleRest* o);
		virtual void visitStart (GREmpty* o);
		virtual void visitStart (GRBar* o);
		virtual void visitStart (GRTrill* o);

	private:
		bool sameNote (GRNote* n1, GRNote* n2) const;
		GRNotationElement* next(GRNotationElement* o) const;
	
		typedef	std::map<GRNote*, GRTrill*> TActiveTrills;
		TActiveTrills	fCurrentTrills;
		GRTrill*		fLastTrill = 0;
		GRBar*			fLastBar = 0;
		bool			fInChord = false;

		ChordTrills*	fPrevious = 0;
		ChordTrills*	fCurrent = 0;
};

#endif
