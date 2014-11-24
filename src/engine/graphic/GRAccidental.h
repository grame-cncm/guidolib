#ifndef GRAccidental_H
#define GRAccidental_H

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

#include "GRNotationElement.h"

#include "ARAccidental.h"

class GREvent;


/** \brief Accidentals: sharp, flat, natural, double sharp, double flat.
*/
class GRAccidental : public GRNotationElement
{
	enum { kNotAnID = -999999 };
	public:

		enum TYPES 
		{ 
	  		AC_NONE 	= kNoneSymbol,
			AC_SHARP 	= kSharpSymbol,
			AC_DSHARP 	= kDSharpSymbol,
			AC_CSHARP	= kCauSharpSymbol,
			AC_CDSHARP 	= kCauDSharpSymbol,
			AC_FLAT 	= kFlatSymbol,
			AC_DFLAT 	= kDFlatSymbol, 
			AC_CFLAT	= kCauFlatSymbol,
			AC_CDFLAT 	= kCauDFlatSymbol, 
		 	AC_NATURAL 	= kNaturalSymbol,
			AC_CNATURAL = kCNaturalSymbol,
			
			AC_QSHARP	= kQSharpSymbol,
			AC_3QSHARP	= k3QSharpSymbol,
			AC_QFLAT	= kQFlatSymbol,
			AC_3QFLAT	= k3QFlatSymbol,

			AC_CQSHARP	= kCauQSharpSymbol,
			AC_C3QSHARP	= kCau3QSharpSymbol,
			AC_CQFLAT	= kCauQFlatSymbol,
			AC_C3QFLAT	= kCau3QFlatSymbol
	  	};
		
				 GRAccidental(GREvent * sngnot, float notebreite, float inAccidentalID, float p_size = 1.0f, float curLSPACE = 50.0f );
	 virtual	~GRAccidental();

//	 virtual void GGSOutput() const;
//	 virtual void OnDraw(VGDevice & hdc) const;
//	 virtual void setPosition( const NVPoint & position );
	 virtual void setAccidental		 ( float inAccidentalID, float notebreite, float curLSPACE );
	 virtual void setAccidentalByQuarter( int quarterTones, int offset, float notebreite, float curLSPACE = 50.0f );
	 virtual void setCautionary(int offset, float notebreite, float curLSPACE = 50.0f );
	 virtual void setColor(const TagParameterString *tps);
     virtual void setColRef(unsigned char *colRef);
	 virtual void setAccidentalLayout( float notebreite, float curLSPACE );
	 
	 virtual void addToOffset( const NVPoint & pt );
	 virtual const NVPoint &	getReferencePosition() const;


	 virtual const NVPoint & getOffset() const			{ return offset; }
	 virtual const unsigned char * getColRef() const	{ return mColRef; }
	 virtual float getSize() const						{ return mAccidentalSize; }

	 virtual void setOffsetX(float x)			{ offsetset = true; offset.x = x; }	
	 virtual void setOffsetY(float y)			{ offsetset = true; offset.y = y; }	
	 virtual void setOffset(const NVPoint & pt)	{ offsetset = true; offset = pt; }
	 
	 virtual void setSize( float newsize )		{ mAccidentalSize = newsize; }
	 bool getOffsetSet() const					{ return offsetset; }

  protected:
	  bool   offsetset; 

	  static NVPoint sRefposNone;
	  static NVPoint sRefposSharp;
	  static NVPoint sRefposDSharp;
	  static NVPoint sRefposCSharp;
	  static NVPoint sRefposFlat;
	  static NVPoint sRefposDFlat;
	  static NVPoint sRefposCFlat;
	  static NVPoint sRefposNatural;
	  static NVPoint sRefposCNatural;

	  static NVPoint sRefposQSharp;
	  static NVPoint sRefpos3QSharp;
	  static NVPoint sRefposQFlat;
	  static NVPoint sRefpos3QFlat;

	  NVPoint offset;
	  unsigned char * mColRef;
	  float mAccidentalSize;
	
	private:
		void 			initialize			(GREvent * sngnot, float p_size);
		int				accidentalID2acc	(int inAccidentalID, bool& cautionary) const;
		unsigned int	accidentalID2symbol	(int inAccidentalID) const;
		unsigned int	accidentalDetunedID2symbol	(float inAccidentalID) const;
		unsigned int	quarters2symbol		(int quarters) const;
		unsigned int	getCautionary		(unsigned int symbol) const;
		NVPoint &		getRefPos() const;
};

#endif
