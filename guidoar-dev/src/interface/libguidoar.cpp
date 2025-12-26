/*

  guidoar Library
  Copyright (C) 2008-2012  Grame

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

#include <fstream>
#include <string.h>

#include "libguidoar.h"

#include "AROthers.h"
#include "bottomOperation.h"
#include "clonevisitor.h"
#include "durationOperation.h"
#include "eheadOperation.h"
#include "etailOperation.h"
#include "event2timevisitor.h"
#include "guidoelement.h"
#include "guidoparser.h"
#include "headOperation.h"
#ifdef MIDIEXPORT
#include "midiconverter.h"
#endif
#include "mirrorOperation.h"
#include "parOperation.h"
#include "seqOperation.h"
#include "tailOperation.h"
#include "topOperation.h"
#include "transposeOperation.h"
#include "ringvector.h"
#include "rythmApplyOperation.h"
#include "pitchApplyOperation.h"
#include "unrolled_guido_browser.h"

using namespace std;
namespace guido
{

//----------------------------------------------------------------------------
gar_export float			guidoarVersion()	{ return 1.00; }
gar_export const char*		guidoarVersionStr()	{ return "1.00"; }

//----------------------------------------------------------------------------
static SARMusic read (const char* buff)
{
	if (!buff) return 0;
	guidoparser r;
	return r.parseString(buff);
}

//----------------------------------------------------------------------------
gar_export garErr guido2unrolled(const char* gmn, std::ostream& out)
{
	garErr err = kNoErr;
	Sguidoelement score =  read(gmn);
	if (!score) return kInvalidArgument;

	clonevisitor cv;
	unrolled_guido_browser ugb(&cv);
	ugb.browse (score);
	score = cv.result();
	if (score) out << score << endl;
	else err = kOperationFailed;			
	return err;
}

//----------------------------------------------------------------------------
gar_export rational	guidoEv2Time(const char* gmn, unsigned int index, unsigned int voice)
{
	Sguidoelement score =  read(gmn);
	if (!score) return kInvalidArgument;
	event2timevisitor convert;
	return convert.event2time (score, index, voice);
}

//----------------------------------------------------------------------------
gar_export int guidoTime2Ev(const char* gmn, const rational& date, unsigned int voice)
{
	Sguidoelement score =  read(gmn);
	if (!score) return kInvalidArgument;
	event2timevisitor convert;
	return convert.time2event (score, date, voice);
}


//----------------------------------------------------------------------------
// wrappers for score operations
//----------------------------------------------------------------------------
template<typename OP, typename ARG> garErr opWrapper(const char* gmn, ARG param, std::ostream& out)
{
	Sguidoelement score =  read(gmn); 
	if (!score) return kInvalidArgument;

	OP op;
	score = op(score, param);
	if (score) out << score << endl;
	else return kOperationFailed;		
	return kNoErr;
}

//----------------------------------------------------------------------------
template<typename OP> garErr opgmnWrapper(const char* gmn, const char* gmnSpec,  std::ostream& out)
{
	SARMusic score =  read(gmn);
	SARMusic dscore = read(gmnSpec);
	if (!score || !dscore) return kInvalidArgument;

	OP op;
	score = op(score, dscore);
	if (score) out << Sguidoelement(score) << endl;
	else return kOperationFailed;		
	return kNoErr;
}

//----------------------------------------------------------------------------
// score operations
//----------------------------------------------------------------------------
gar_export garErr guidoGBottom(const char* gmn, const char* gmnSpec, std::ostream& out)
							{ return opgmnWrapper<bottomOperation>(gmn, gmnSpec, out); }
gar_export garErr guidoVBottom(const char* gmn, int nvoices, std::ostream& out)
							{ return opWrapper<bottomOperation, int>(gmn, nvoices, out); }

//----------------------------------------------------------------------------
gar_export garErr guidoGTop(const char* gmn, const char* gmnSpec, std::ostream& out)
							{ return opgmnWrapper<topOperation>(gmn, gmnSpec, out); }
gar_export garErr guidoVTop(const char* gmn, int nvoices, std::ostream& out)
							{ return opWrapper<topOperation, int>(gmn, nvoices, out); }

//----------------------------------------------------------------------------
gar_export garErr guidoGSetDuration(const char* gmn, const char* gmnSpec, std::ostream& out)
							{ return opgmnWrapper<durationOperation>(gmn, gmnSpec, out); }
gar_export garErr guidoVSetDuration(const char* gmn, rational duration, std::ostream& out)
							{ return opWrapper<durationOperation, rational>(gmn, duration, out); }
gar_export garErr guidoVMultDuration(const char* gmn, float duration, std::ostream& out)
							{ return opWrapper<durationOperation, float>(gmn, duration, out); }

//----------------------------------------------------------------------------
gar_export garErr guidoApplyRythm(const char* gmn, const char* gmnSpec, TApplyMode mode, std::ostream& out)
{ 
	switch (mode) {
		case kApplyOnce:
			return opgmnWrapper<rythmApplyOperation<vector<rational> > >(gmn, gmnSpec, out); 
		case kApplyForwardLoop:
			return opgmnWrapper<rythmApplyOperation<ringvector<rational> > >(gmn, gmnSpec, out); 
		case kApplyForwardBackwardLoop: ;
			return opgmnWrapper<rythmApplyOperation<fwbwvector<rational> > >(gmn, gmnSpec, out); 
	}
	return kInvalidArgument;
}

//----------------------------------------------------------------------------
gar_export garErr guidoApplyPitch(const char* gmn, const char* gmnSpec, TApplyMode mode, chordPitchMode pmode, std::ostream& out)
{ 
	switch (pmode) {
		 case kUseLowest:
			switch (mode) {
				case kApplyOnce:
					return opgmnWrapper<pitchLowApplyOperation<vector<pitchvisitor::TPitch> > >(gmn, gmnSpec, out); 
				case kApplyForwardLoop:
					return opgmnWrapper<pitchLowApplyOperation<ringvector<pitchvisitor::TPitch> > >(gmn, gmnSpec, out); 
				case kApplyForwardBackwardLoop: ;
					return opgmnWrapper<pitchLowApplyOperation<fwbwvector<pitchvisitor::TPitch> > >(gmn, gmnSpec, out); 
			}
			break;
		 case kUseHighest:
			switch (mode) {
				case kApplyOnce:
					return opgmnWrapper<pitchHighApplyOperation<vector<pitchvisitor::TPitch> > >(gmn, gmnSpec, out); 
				case kApplyForwardLoop:
					return opgmnWrapper<pitchHighApplyOperation<ringvector<pitchvisitor::TPitch> > >(gmn, gmnSpec, out); 
				case kApplyForwardBackwardLoop: ;
					return opgmnWrapper<pitchHighApplyOperation<fwbwvector<pitchvisitor::TPitch> > >(gmn, gmnSpec, out); 
			}
			break;
	}
	return kInvalidArgument;
}

//----------------------------------------------------------------------------
gar_export garErr guidoVTranpose(const char* gmn, int interval, std::ostream& out)
							{ return opWrapper<transposeOperation, int>(gmn, interval, out); }
gar_export garErr guidoGTranpose(const char* gmn, const char* gmnSpec, std::ostream& out)
							{ return opgmnWrapper<transposeOperation>(gmn, gmnSpec, out); }

//----------------------------------------------------------------------------
gar_export garErr guidoVHead(const char* gmn, rational duration, std::ostream& out)
							{ return opWrapper<headOperation, rational>(gmn, duration, out); }
gar_export garErr guidoGHead(const char* gmn, const char* gmnSpec, std::ostream& out)
							{ return opgmnWrapper<headOperation>(gmn, gmnSpec, out); }

//----------------------------------------------------------------------------
gar_export garErr guidoVEHead(const char* gmn, int duration, std::ostream& out)
							{ return opWrapper<eheadOperation, int>(gmn, duration, out); }
gar_export garErr guidoGEHead(const char* gmn, const char* gmnSpec, std::ostream& out)
							{ return opgmnWrapper<eheadOperation>(gmn, gmnSpec, out); }

//----------------------------------------------------------------------------
gar_export garErr guidoVTail(const char* gmn, rational duration, std::ostream& out)
							{ return opWrapper<tailOperation, rational>(gmn, duration, out); }
gar_export garErr guidoGTail(const char* gmn, const char* gmnSpec, std::ostream& out)
							{ return opgmnWrapper<tailOperation>(gmn, gmnSpec, out); }

//----------------------------------------------------------------------------
gar_export garErr guidoVETail(const char* gmn, int duration, std::ostream& out)
							{ return opWrapper<etailOperation, int>(gmn, duration, out); }
gar_export garErr guidoGETail(const char* gmn, const char* gmnSpec, std::ostream& out)
							{ return opgmnWrapper<etailOperation>(gmn, gmnSpec, out); }

//----------------------------------------------------------------------------
gar_export garErr guidoGSeq(const char* gmn1, const char* gmn2, std::ostream& out)
							{ return opgmnWrapper<seqOperation>(gmn1, gmn2, out); }
gar_export garErr guidoGPar(const char* gmn1, const char* gmn2, std::ostream& out)
							{ return opgmnWrapper<parOperation>(gmn1, gmn2, out); }
gar_export garErr guidoGRPar(const char* gmn1, const char* gmn2, std::ostream& out)
							{ return opgmnWrapper<rparOperation>(gmn1, gmn2, out); }

//----------------------------------------------------------------------------
gar_export garErr guidoGMirror(const char* gmn1, const char* gmn2, std::ostream& out)
							{ return opgmnWrapper<mirrorOperation>(gmn1, gmn2, out); }

//----------------------------------------------------------------------------
gar_export rational guidoDuration(const char* gmn)
{
	rational duration (-1,1);
	Sguidoelement score =  read(gmn); 
	if (score) {
		durationvisitor dv;
		duration = dv.duration (score);
	}
	return duration;
}

//----------------------------------------------------------------------------
gar_export garErr guido2midifile(const char* gmn, const char* file)
{
#ifdef MIDIEXPORT
	Sguidoelement score =  read(gmn);
	if (!score) return kInvalidArgument;

	midiconverter mc;
	return mc.score2midifile(score, file) ? kNoErr : kOperationFailed; 
#else
	cerr << "guido2midifile: no support for midifile conversion embedded in the guidoar library" << endl;
	return kOperationFailed;
#endif
}

//----------------------------------------------------------------------------
gar_export bool guidocheck(const char* gmn)
{
	Sguidoelement score =  read(gmn); 
	return score ? true : false;
}

}
