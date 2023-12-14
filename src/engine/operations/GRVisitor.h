/*
  GUIDO Library
  Copyright (C) 2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef ___GRVisitor___
#define ___GRVisitor___

class GRNotationElement;
class GRArticulation;
class GRBar;
class GRBeam;
class GRClef;
class GRDynamics;
class GREmpty;
class GRIntens;
class GRKey;
class GRMeter;
class GRMusic;
class GROctava;
class GRPage;
class GRRange;
class GRRepeatEnd;
class GRRepeatBegin;
class GRSingleNote;
class GRSingleRest;
class GRSlur;
class GRStaff;
class GRSystem;
class GRText;
class GRTrill;
class GRVoice;

class GRVisitor
{
	public:
		virtual ~GRVisitor() {};
    
		virtual bool voiceMode () = 0;
    
		virtual void visitStart (GRNotationElement* o) {};
		virtual void visitEnd   (GRNotationElement* o) {};

		virtual void visitStart (GRMusic* o)	{}
		virtual void visitEnd   (GRMusic* o)	{}

		virtual void visitStart (GRPage* o)		{}
		virtual void visitEnd   (GRPage* o)		{}

		virtual void visitStart (GRVoice* o)	{}
		virtual void visitEnd   (GRVoice* o)	{}

		virtual void visitStart (GRSystem* o)	{}
		virtual void visitEnd   (GRSystem* o)	{}

		virtual void visitStart (GRStaff* o)	{}
		virtual void visitEnd   (GRStaff* o)	{}

		virtual void visitStart (GRSlur* o)		{}
		virtual void visitEnd   (GRSlur* o)		{}

		virtual void visitStart (GRDynamics* o)	{}
		virtual void visitEnd   (GRDynamics* o)	{}

		virtual void visitStart (GRIntens* o)	{}
		virtual void visitEnd   (GRIntens* o)	{}

		virtual void visitStart (GRMeter* o)	{}
		virtual void visitEnd   (GRMeter* o)	{}

		virtual void visitStart (GRClef* o)		{}
		virtual void visitEnd   (GRClef* o)		{}

		virtual void visitStart (GRKey* o)		{}
		virtual void visitEnd   (GRKey* o)		{}

		virtual void visitStart (GREmpty* o)	{}
		virtual void visitEnd   (GREmpty* o)	{}

		virtual void visitStart (GRRange* o)	{}
		virtual void visitEnd   (GRRange* o)	{}

		virtual void visitStart (GRBeam* o)		{}
		virtual void visitEnd   (GRBeam* o)		{}

		virtual void visitStart (GRArticulation* o)	{}
		virtual void visitEnd   (GRArticulation* o)	{}

		virtual void visitStart (GRBar* o)		{}
		virtual void visitEnd   (GRBar* o)		{}

		virtual void visitStart (GRRepeatBegin* o)		{}
		virtual void visitEnd   (GRRepeatBegin* o)		{}

		virtual void visitStart (GRText* o)		{}
		virtual void visitEnd   (GRText* o)		{}

		virtual void visitStart (GRTrill* o)		{}
		virtual void visitEnd   (GRTrill* o)		{}

		virtual void visitStart (GRSingleNote* o)	{}
		virtual void visitEnd   (GRSingleNote* o)	{}

		virtual void visitStart (GRSingleRest* o)	{}
		virtual void visitEnd   (GRSingleRest* o)	{}

		virtual void visitStart (GROctava* o)	{}
		virtual void visitEnd   (GROctava* o)	{}
};

#endif
