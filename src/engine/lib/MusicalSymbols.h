#ifndef MusicalSymbols_H
#define MusicalSymbols_H

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
	The guido2 font should makes use of ISO-8859-1 encoding (also named latin1).
	Note that ISO-8859-1 is fully compatible with unicode encoding.
	
	In order to avoid chars mapping, the font must define a glyph for all characters 
	ranging from 3 to 255. To operate correctly on windows, the ranges 3 to 31 and 127 to 159
	should not be used. ISO-8859-1 reserves these ranges for control characters and apart the for 
	the exceptions defined by Windows Latin 1 (also named ANSI), windows won't display the corresponding
	glyphs.
*/

const int kMaxMusicalSymbolID				= 256;

typedef const unsigned char ConstMusicalSymbolID;	// will be replaced by const unsigned int

ConstMusicalSymbolID kNoneSymbol			 =   0;	// must stay = 0
ConstMusicalSymbolID kEmptySymbol			 =  32;
ConstMusicalSymbolID kSharpSymbol			 =  35;
ConstMusicalSymbolID kSegnoSymbol			 =  37;
ConstMusicalSymbolID kClefViolin			 =  38;
ConstMusicalSymbolID kIntensFFFSymbol		 =  39;
ConstMusicalSymbolID kLeftHPizzSymbol		 =  43;	// left-hand pizzicato ("+")
ConstMusicalSymbolID kBreathMarkSymbol		 =  44;	// ','
ConstMusicalSymbolID kTenutoSymbol			 =  45;
ConstMusicalSymbolID kNoteDotSymbol	 		 =  46;
ConstMusicalSymbolID kStaccatoSymbol		 =  46;
ConstMusicalSymbolID kP128Symbol			 =  47;	  
ConstMusicalSymbolID kStaffSymbol			 =  61;
ConstMusicalSymbolID kAccentSymbol			 =  62;
ConstMusicalSymbolID kClefBass				 =  63;
ConstMusicalSymbolID kP16Symbol				 =  64;	  
ConstMusicalSymbolID kClefBratsche			 =  66;
ConstMusicalSymbolID kC2Symbol				 =  67;	// 'C'
ConstMusicalSymbolID kP2Symbol				 =  68;
ConstMusicalSymbolID kHalfNoteHeadSymbol	 =  69;
ConstMusicalSymbolID kIntensMFSymbol		 =  70;
ConstMusicalSymbolID kFlag8DownSymbol		 =  74;  
ConstMusicalSymbolID kFlag16DownSymbol		 =  75;
ConstMusicalSymbolID kInvertedMordSymbol	 =  77;
ConstMusicalSymbolID kIntensMPSymbol		 =  80;
ConstMusicalSymbolID kIntensSFSymbol		 =  83;
ConstMusicalSymbolID kTurnSymbol			 =  84;
ConstMusicalSymbolID kFermataUpSymbol		 =  85;
ConstMusicalSymbolID kLongaHeadSymbol		 =  87;
ConstMusicalSymbolID kFullHeadSymbol		 =  88;
ConstMusicalSymbolID kStaffLineSymbol	 	 =  89;
ConstMusicalSymbolID kFinishBarSymbol	 	 =  92;
ConstMusicalSymbolID kRepeatBeginSymbol		 =  93;	// ']'
ConstMusicalSymbolID kMarcatoUpSymbol		 =  94;
ConstMusicalSymbolID kLedgerLineSymbol	 	 =  95;	// Ledger line
ConstMusicalSymbolID kP8Symbol				 =  97;	  
ConstMusicalSymbolID kFlatSymbol			 =  98;
ConstMusicalSymbolID kCSymbol				 =  99;	// 'c'
ConstMusicalSymbolID kBarGSSymbol	 		 = 101;
ConstMusicalSymbolID kIntensFSymbol			 = 102;	  
ConstMusicalSymbolID kFlag8UpSymbol			 = 106;
ConstMusicalSymbolID kFlag16UpSymbol		 = 107;
ConstMusicalSymbolID kP64Symbol				 = 108;
ConstMusicalSymbolID kMordSymbol			 = 109;	// 'm'
ConstMusicalSymbolID kNaturalSymbol			 = 110;
ConstMusicalSymbolID kHarmonicSymbol		 = 111;	// ("o")
ConstMusicalSymbolID kIntensPSymbol			 = 112;
ConstMusicalSymbolID kClefPerc				 = 114;
ConstMusicalSymbolID kIntensFFFFSymbol		 = 115;
ConstMusicalSymbolID kFermataDownSymbol		 = 117;
ConstMusicalSymbolID kMarcatoDownSymbol		 = 118;
ConstMusicalSymbolID kWholeNoteHeadSymbol	 = 119;
ConstMusicalSymbolID kRepeatEndSymbol		 = 125;


// new encoding 8859-1 - version 1.38
ConstMusicalSymbolID kDFlatSymbol			 =  72;
ConstMusicalSymbolID kBarSymbol				 = 101;
ConstMusicalSymbolID kStemDown1Symbol		 = 104;	// vertical line, junction to the head
ConstMusicalSymbolID kStemDown2Symbol		 = 104;	// vertical line a little longer
ConstMusicalSymbolID kStemUp1Symbol			 = 120;	// vertical line, junction to the head
ConstMusicalSymbolID kStemUp2Symbol			 = 120;	// vertical line a little longer
ConstMusicalSymbolID kTilde					 = 126;	// tilde
ConstMusicalSymbolID kBembelSymbol			 = 164;
ConstMusicalSymbolID kP4Symbol				 = 165;	// Rests
ConstMusicalSymbolID kShortFermataUpSymbol   = 168;
ConstMusicalSymbolID kAccoladeSymbol	 	 = 171;
ConstMusicalSymbolID kP32Symbol				 = 174;
ConstMusicalSymbolID kDoubleBarSymbol		 = 175;
ConstMusicalSymbolID kShortFermataDownSymbol = 179;
ConstMusicalSymbolID kDSharpSymbol			 = 180;
ConstMusicalSymbolID kPrallPrallSymbol		 = 181;
ConstMusicalSymbolID kLongFermataUpSymbol	 = 184;
ConstMusicalSymbolID kLongFermataDownSymbol  = 185;
ConstMusicalSymbolID kPrallMordentSymbol	 = 187;
ConstMusicalSymbolID kTrillSymbol			 = 188;
ConstMusicalSymbolID kCauSharpSymbol		 = 189;	// (#)
ConstMusicalSymbolID kCauFlatSymbol			 = 190;	// (b)
ConstMusicalSymbolID kErrorHeadSymbol		 = 191;
ConstMusicalSymbolID kCNaturalSymbol		 = 192;
ConstMusicalSymbolID kIntensFFSymbol		 = 193;
ConstMusicalSymbolID kIntensPPSymbol		 = 194;
ConstMusicalSymbolID kIntensPPPPSymbol		 = 195;
ConstMusicalSymbolID kIntensPPPSymbol		 = 200;
ConstMusicalSymbolID kFlag64UpSymbol		 = 202;
ConstMusicalSymbolID kFlag64DownSymbol		 = 203;
ConstMusicalSymbolID kFlag32UpSymbol		 = 221;
ConstMusicalSymbolID kFlag32DownSymbol		 = 222;
ConstMusicalSymbolID kStaccmoDownSymbol		 = 224;
ConstMusicalSymbolID kSnapPizzSymbol		 = 225;  
ConstMusicalSymbolID kBuzzPizzSymbol		 = 226;
ConstMusicalSymbolID kFingernailPizzSymbol	 = 227;
ConstMusicalSymbolID kStaccmoUpSymbol		 = 228;
ConstMusicalSymbolID kInvertedTurnSymbol	 = 229;
ConstMusicalSymbolID kCodaSymbol			 = 230;

// version 1.39
ConstMusicalSymbolID kP1Symbol				 = 231;
// version 1.43
ConstMusicalSymbolID kCauDFlatSymbol		 = 121;	// cautionary double flat
ConstMusicalSymbolID kQSharpSymbol			 = 178;	// a quarter sharp
ConstMusicalSymbolID k3QSharpSymbol			 = 204;	// 3/4 sharp
ConstMusicalSymbolID kQFlatSymbol			 = 205;	// a quarter flat
ConstMusicalSymbolID k3QFlatSymbol			 = 206;	// 3/4 flat
ConstMusicalSymbolID kUpArrowSymbol			 = 207;
ConstMusicalSymbolID kDownArrowSymbol		 = 208;
ConstMusicalSymbolID kCauQSharpSymbol		 = 209;	// cautionary quarter sharp
ConstMusicalSymbolID kCau3QSharpSymbol		 = 210;	// cautionary 3/4 sharp
ConstMusicalSymbolID kCauQFlatSymbol		 = 211;	// cautionary quarter flat
ConstMusicalSymbolID kCau3QFlatSymbol		 = 212;	// cautionary 3/4 flat
ConstMusicalSymbolID kCauDSharpSymbol		 = 213;	// cautionary double sharp

// version 1.51
ConstMusicalSymbolID kRoundLeftBracket               = 40;
ConstMusicalSymbolID kRoundRightBracket              = 41;
ConstMusicalSymbolID kFullDiamondHeadSymbol	         = 81;
ConstMusicalSymbolID kFullXHeadSymbol				 = 191;
ConstMusicalSymbolID kHalfDiamondHeadSymbol	         = 214;
ConstMusicalSymbolID kFullSquareHeadSymbol           = 215;
ConstMusicalSymbolID kHalfSquareHeadSymbol 	         = 217;
ConstMusicalSymbolID kFullRoundHeadSymbol            = 220;
ConstMusicalSymbolID kHalfRoundHeadSymbol 	         = 232;
ConstMusicalSymbolID kHalfXHeadSymbol		         = 233;
ConstMusicalSymbolID kFullTriangleHeadSymbol         = 234;
ConstMusicalSymbolID kHalfTriangleHeadSymbol         = 235;
ConstMusicalSymbolID kFullReversedTriangleHeadSymbol = 236;
ConstMusicalSymbolID kHalfReversedTriangleHeadSymbol = 237;
ConstMusicalSymbolID kSquareLeftBracket              = 238;
ConstMusicalSymbolID kSquareRightBracket             = 239;
ConstMusicalSymbolID kAngledLeftBracket              = 240;
ConstMusicalSymbolID kAngledRightBracket             = 241;


#endif

