#ifndef MusicalSymbolsSMUFL_H
#define MusicalSymbolsSMUFL_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

// -> (JB) - Was char consts for scriabin / guido2

/** \brief A collection of constants defining each musical symbol.

	Those constants currently match the character constants of corresponding symbols
	in the Guido2 (scriabin) font, but it will no more be the case in a near futur.

	ie. each score rendering function that takes a MusicalSymbolID as a parameter will
	have to convert it into an ascii character value  (if it uses musical fonts
	for score rendering).

	A kMaxMusicalSymbolID constant is provided for the creation of "symbolID" to 
	"anything-else" convertion tables:

			Foo mySymbolToFooTable [ kMaxMusicalSymbolID ];
	
*/

/* 
	SMUFL compliant font description
*/


ConstMusicalSymbolID kNoneSymbol			 = 0;	// must stay = 0
ConstMusicalSymbolID kEmptySymbol			 = 0;

ConstMusicalSymbolID kClefViolin			 = 0xE050;
ConstMusicalSymbolID kClefViolin8b		 	 = 0xE052;
ConstMusicalSymbolID kClefViolin8bold		 = 0xE055;
ConstMusicalSymbolID kClefViolin15b		 	 = 0xE051;
ConstMusicalSymbolID kClefViolin8a		 	 = 0xE053;
ConstMusicalSymbolID kClefViolin15a		 	 = 0xE054;
ConstMusicalSymbolID kClefBass				 = 0xE062;
ConstMusicalSymbolID kClefBass8b			 = 0xE064;
ConstMusicalSymbolID kClefBass15b			 = 0xE063;
ConstMusicalSymbolID kClefBass8a			 = 0xE065;
ConstMusicalSymbolID kClefBass15a			 = 0xE066;
ConstMusicalSymbolID kClefBratsche			 = 0xE05C;
ConstMusicalSymbolID kClefPerc				 = 0xE069;

ConstMusicalSymbolID kIntensMFSymbol		 = 0xE52D;
ConstMusicalSymbolID kIntensMPSymbol		 = 0xE52C;
ConstMusicalSymbolID kIntensSFSymbol		 = 0xE536;
ConstMusicalSymbolID kIntensFSymbol			 = 0xE522;
ConstMusicalSymbolID kIntensFFSymbol		 = 0xE52F;
ConstMusicalSymbolID kIntensFFFSymbol		 = 0xE530;
ConstMusicalSymbolID kIntensFFFFSymbol		 = 0xE531;
ConstMusicalSymbolID kIntensPSymbol			 = 0xE520;
ConstMusicalSymbolID kIntensPPSymbol		 = 0xE52B;
ConstMusicalSymbolID kIntensPPPSymbol		 = 0xE52A;
ConstMusicalSymbolID kIntensPPPPSymbol		 = 0xE529;

ConstMusicalSymbolID kTenutoSymbol			 = 0xE4A4;  // above version, below is 0xE4A5
ConstMusicalSymbolID kStaccatoSymbol		 = 0xE4A2;  // above version, below is 0xE4A3
ConstMusicalSymbolID kAccentSymbol			 = 0xE4A0;  // above version, below is 0xE4A1
ConstMusicalSymbolID kMarcatoUpSymbol		 = 0xE4AC;
ConstMusicalSymbolID kMarcatoDownSymbol		 = 0xE4AD;
ConstMusicalSymbolID kStaccmoUpSymbol		 = 0xE4A6;
ConstMusicalSymbolID kStaccmoDownSymbol		 = 0xE4A7;

ConstMusicalSymbolID kP1Symbol				 = 0xE4E3;
ConstMusicalSymbolID kP2Symbol				 = 0xE4E4;
ConstMusicalSymbolID kP4Symbol				 = 0xE4E5;	// Rests
ConstMusicalSymbolID kP8Symbol				 = 0xE4E6;
ConstMusicalSymbolID kP16Symbol				 = 0xE4E7;
ConstMusicalSymbolID kP32Symbol				 = 0xE4E8;
ConstMusicalSymbolID kP64Symbol				 = 0xE4E9;
ConstMusicalSymbolID kP128Symbol			 = 0xE4EA;

ConstMusicalSymbolID kNaturalSymbol			 = 0xE261;
ConstMusicalSymbolID kSharpSymbol			 = 0xE262;
ConstMusicalSymbolID kDSharpSymbol			 = 0xE263;
ConstMusicalSymbolID kFlatSymbol			 = 0xE260;
ConstMusicalSymbolID kDFlatSymbol			 = 0xE264;
// Stein-Zimmermann accidentals
ConstMusicalSymbolID kQSharpSymbol			 = 0xE282;	// a quarter sharp
ConstMusicalSymbolID k3QSharpSymbol			 = 0xE283;	// 3/4 sharp
ConstMusicalSymbolID kQFlatSymbol			 = 0xE280;	// a quarter flat
ConstMusicalSymbolID k3QFlatSymbol			 = 0xE281;	// 3/4 flat

ConstMusicalSymbolID kAccidentalParensLeftSymbol  = 0xE26A;		//
ConstMusicalSymbolID kAccidentalParensRightSymbol = 0xE26B;		//
ConstMusicalSymbolID kCauSharpSymbol		 = kSharpSymbol;	// (#) should be left_kSharpSymbol_right
ConstMusicalSymbolID kCauFlatSymbol			 = kFlatSymbol;		// (b)
ConstMusicalSymbolID kCauDSharpSymbol		 = kDSharpSymbol;	// cautionary double sharp
ConstMusicalSymbolID kCauDFlatSymbol		 = kDFlatSymbol;	// cautionary double flat
ConstMusicalSymbolID kCauQSharpSymbol		 = kQSharpSymbol;	// cautionary quarter sharp
ConstMusicalSymbolID kCau3QSharpSymbol		 = k3QSharpSymbol;	// cautionary 3/4 sharp
ConstMusicalSymbolID kCauQFlatSymbol		 = kQFlatSymbol;	// cautionary quarter flat
ConstMusicalSymbolID kCau3QFlatSymbol		 = k3QFlatSymbol;	// cautionary 3/4 flat
ConstMusicalSymbolID kCNaturalSymbol		 = kNaturalSymbol;	// should be left_kNaturalSymbol_right

ConstMusicalSymbolID kBarSymbol				 = 0xE030;
ConstMusicalSymbolID kDoubleBarSymbol		 = 0xE031;
ConstMusicalSymbolID kRepeatBeginSymbol		 = 0xE040;
ConstMusicalSymbolID kRepeatEndSymbol		 = 0xE041;
ConstMusicalSymbolID kFinishBarSymbol	 	 = 0xE032;

ConstMusicalSymbolID kSegnoSymbol			 = 0xE047;
ConstMusicalSymbolID kCodaSymbol			 = 0xE048;

ConstMusicalSymbolID kC2Symbol				 = 0xE08B;
ConstMusicalSymbolID kCSymbol				 = 0xE08A;
ConstMusicalSymbolID kMeterPlusSymbol		 = 0xE08C;
ConstMusicalSymbolID kMeter0Symbol		 	 = 0xE080;

ConstMusicalSymbolID kLongaHeadSymbol		 = 0xE0A0;
ConstMusicalSymbolID kWholeNoteHeadSymbol	 = 0xE0A2;
ConstMusicalSymbolID kHalfNoteHeadSymbol	 = 0xE0A3;
ConstMusicalSymbolID kFullHeadSymbol		 = 0xE0A4;

ConstMusicalSymbolID kMordSymbol			 = 0xE56C;	// 'm'
ConstMusicalSymbolID kInvertedMordSymbol	 = 0xE56D;
ConstMusicalSymbolID kTurnSymbol			 = 0xE567;
ConstMusicalSymbolID kInvertedTurnSymbol	 = 0xE568;
ConstMusicalSymbolID kPrallPrallSymbol		 = 0xE56E;
ConstMusicalSymbolID kPrallMordentSymbol	 = 0xE5BD;  // not available ?
ConstMusicalSymbolID kTrillSymbol			 = 0xE566 ;

ConstMusicalSymbolID kFermataUpSymbol		 = 0xE4C0 ;
ConstMusicalSymbolID kFermataDownSymbol		 = 0xE4C1;
ConstMusicalSymbolID kShortFermataUpSymbol   = 0xE4C4;
ConstMusicalSymbolID kShortFermataDownSymbol = 0xE4C5;
ConstMusicalSymbolID kLongFermataUpSymbol	 = 0xE4C6;
ConstMusicalSymbolID kLongFermataDownSymbol  = 0xE4C7;
ConstMusicalSymbolID kBreathMarkSymbol		 = 0xE4CE;

ConstMusicalSymbolID kSnapPizzSymbol		 = 0xE631;
ConstMusicalSymbolID kBuzzPizzSymbol		 = 0xE632;		// inverted in SMUFL
ConstMusicalSymbolID kFingernailPizzSymbol	 = 0xE636;		// inverted in SMUFL
ConstMusicalSymbolID kLeftHPizzSymbol		 = 0xE633;

ConstMusicalSymbolID kAccoladeSymbol	 	 = 0xE000;

ConstMusicalSymbolID kFlag8DownSymbol		 = 0xE241;
ConstMusicalSymbolID kFlag16DownSymbol		 = 0xE243;
ConstMusicalSymbolID kFlag32DownSymbol		 = 0xE245;
ConstMusicalSymbolID kFlag64DownSymbol		 = 0xE247;
ConstMusicalSymbolID kFlag8UpSymbol			 = 0xE240;
ConstMusicalSymbolID kFlag16UpSymbol		 = 0xE242;
ConstMusicalSymbolID kFlag32UpSymbol		 = 0xE244;
ConstMusicalSymbolID kFlag64UpSymbol		 = 0xE246;

ConstMusicalSymbolID kStemDown1Symbol		 = 0xE210;
ConstMusicalSymbolID kStemDown2Symbol		 = 0xE210;
ConstMusicalSymbolID kStemUp1Symbol			 = 0xE210;
ConstMusicalSymbolID kStemUp2Symbol			 = 0xE210;

ConstMusicalSymbolID kFullDiamondHeadSymbol	         = 0xE1B9;
ConstMusicalSymbolID kHalfDiamondHeadSymbol	         = 0xE1B8;
ConstMusicalSymbolID kFullXHeadSymbol				 = 0xE0A9 ;
ConstMusicalSymbolID kHalfXHeadSymbol		         = 0xE0B2;
ConstMusicalSymbolID kFullSquareHeadSymbol           = 0xE0B9;
ConstMusicalSymbolID kHalfSquareHeadSymbol 	         = 0xE0B8;
ConstMusicalSymbolID kFullRoundHeadSymbol            = 0xE113;
ConstMusicalSymbolID kHalfRoundHeadSymbol 	         = 0xE114;
ConstMusicalSymbolID kFullTriangleHeadSymbol         = 0xE0BE;
ConstMusicalSymbolID kHalfTriangleHeadSymbol         = 0xE0BF;
ConstMusicalSymbolID kFullReversedTriangleHeadSymbol = 0xE0C7;
ConstMusicalSymbolID kHalfReversedTriangleHeadSymbol = 0xE0C6;
//ConstMusicalSymbolID kErrorHeadSymbol				 = 0xE0A9;

ConstMusicalSymbolID kTuplet0Symbol 		= 0xE880;
ConstMusicalSymbolID kTuplet9Symbol 		= 0xE889;
ConstMusicalSymbolID kTupletColonSymbol 	= 0xE88A;


//ConstMusicalSymbolID kRoundLeftBracket 	= 0xE0F5;
//ConstMusicalSymbolID kRoundRightBracket	= 0xE0F6;
//ConstMusicalSymbolID kRoundLeftBracket	= 0xE571;
//ConstMusicalSymbolID kRoundRightBracket	= 0xE572;
ConstMusicalSymbolID kRoundLeftBracket 		= 0xE542;
ConstMusicalSymbolID kRoundRightBracket 	= 0xE543;
ConstMusicalSymbolID kSquareLeftBracket  	= 0xE544;
ConstMusicalSymbolID kSquareRightBracket 	= 0xE545;
//ConstMusicalSymbolID kAngledLeftBracket 	= 240;
//ConstMusicalSymbolID kAngledRightBracket 	= 241;
ConstMusicalSymbolID kAngledLeftBracket  	= 0;
ConstMusicalSymbolID kAngledRightBracket 	= 0;

ConstMusicalSymbolID kHarmonicSymbol		 = 0xE614;

ConstMusicalSymbolID kNoteDotSymbol	 		 = 0xE1E7;
ConstMusicalSymbolID kDotSymbol				 = 0xE1E7;

ConstMusicalSymbolID kStaffSymbol			 = 0xE014;
ConstMusicalSymbolID kStaffLineSymbol	 	 = 0xE016;  // ?
ConstMusicalSymbolID kLedgerLineSymbol	 	 = 0xE022;	// Ledger line

ConstMusicalSymbolID kTilde					 = 0xEAA4;	// trill
//ConstMusicalSymbolID kBembelSymbol			 = 164;


ConstMusicalSymbolID kUpArrowSymbol			 = 0xEB60;
ConstMusicalSymbolID kDownArrowSymbol		 = 0xEB64;

ConstMusicalSymbolID kArpeggioSymbol         = 32;


#endif

