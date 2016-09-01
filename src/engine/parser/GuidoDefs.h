#ifndef GuidoDefs_H
#define GuidoDefs_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#include "MusicalSymbols.h"
#include "GUIDOTypes.h"

const float LSPACE = 50;		// Distance between two lines of staff (a dent).

// This means: 1 Halfspace is 3pt
// unit of kVirtualToCm: cm/virtual. For LSPACE==32 follows
//  0,006614583333333 cm/virtual pixel, 
//  that means one system has a height of 0,8 cm
//           pt * hspace * inch * cm
// units: -----------------------------
//        virtual * hspace * inch * pt 

//const float kVirtualToCm = (float)(3.0/ double(LSPACE) * 2.0 / 72.27 * 2.54);
// Multiply a value by kVirtualToCm to convert it from virtual units to centimeters.
// Divide by kVirtualToCm to convert from centimeters to virtual units.

const float kVirtualToCm = (float)((3.0 * 2.0 * 2.54) / (LSPACE * 72.27));	// Was UNCONST
const float kCmToVirtual = (float)1.0 / kVirtualToCm;			// Was INVCONST

const float kVirtualToInch = kVirtualToCm / (float)2.54;
const float kInchToVirtual = (float)1.0 / kVirtualToInch;

const float kPxToVirtual = (float)72.0 * kVirtualToInch;
const float kVirtualToPx = (float)1.0 / kPxToVirtual;

//const int MINSPACING = 0; 
const int NUMNOTES = 12;

// For the stem ...
const float STEMLENGTH = float(3.5 * LSPACE);
const float STRAIGHTFLAG = float(0.5 * LSPACE);

// define the default SCONST for some elements
const float SCONST_BAR = 7.0f;
const float SCONST_CLEF = 100.0f;
const float SCONST_DEFAULT = 20.0f;
const float SCONST_GLUESTART = 50.0f;
const float SCONST_GLUENOSTART = 7.0f;
const float SCONST_KEY = 100.0f;
const float SCONST_INTENS = 100.0f;
const float SCONST_METER = 100.0f;

const float	kLineThick = LSPACE * 0.08f;

// Default format of Guido Score Page, in centimeters.
// sx and sy define the default size. (the size value includes margins)
// ml, mt, mr, mb define the default left, top, right and bottom margins.

const float DF_SX = 21.0f;
const float DF_SY = 29.7f;
const float DF_ML = 2.0f;
const float DF_MT = 2.0f;
const float DF_MR = 2.0f;
const float DF_MB = 2.0f;

// - The layout settings default values
const float	kSettingDefaultSystemDistance        = float(1.5 * LSPACE);
const int	kSettingDefaultSystemDistrib         = 1; // 1=auto, 2=always, 3=never
const float kSettingDefaultDistribLimit          = float(0.25);	// % of the height of the page
const float kSettingDefaultForce                 = float(750.0);
const float kSettingDefaultSpring                = float(1.1);
const bool	kSettingDefaultNeighborhood          = 0;
const bool	kSettingDefaultOptimalPageFill       = 1;
const bool  kSettingDefaultResizePage2Music      = 1;
const float kSettingDefaultProportionalRendering = 0;

// - Notes *must* stay in this order. Several algorithms rely on it
enum
{
	EMPTY,
	REST,
	NOTE_C,
	NOTE_D,
	NOTE_E,
	NOTE_F,
	NOTE_G,
	NOTE_A,
	NOTE_H,
	NOTE_CIS,	// "IS" notes must stay contiguous
	NOTE_DIS,
	NOTE_FIS,
	NOTE_GIS,
	NOTE_AIS
};


#endif

