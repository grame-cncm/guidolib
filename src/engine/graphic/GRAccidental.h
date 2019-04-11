#ifndef GRAccidental_H
#define GRAccidental_H

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

#include "GRNotationElement.h"

#include "ARAccidental.h"

class GREvent;


/** \brief Accidentals: sharp, flat, natural, double sharp, double flat.
*/
class GRAccidental : public GRNotationElement
{
	enum { kNotAnID = -999999 };
	public:
				 GRAccidental(GREvent * sngnot, float notebreite, float inAccidentalID, float p_size = 1.0f, float curLSPACE = 50.0f );
				 GRAccidental();					// a none accidental
				 GRAccidental(float detune, bool cautionnary, float p_size);	//
	 virtual	~GRAccidental();

	 virtual void OnDraw(VGDevice & hdc) const;
	 virtual void setAccidental		 ( float inAccidentalID, float notebreite, float curLSPACE );
	 virtual void setAccidentalByQuarter( int quarterTones, int offset, float notebreite, float curLSPACE = 50.0f );
	 virtual void setCautionary(int offset, float notebreite, float curLSPACE = 50.0f );
	 virtual void setStyleNone();
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

	 virtual void setDxy(const NVPoint & pt)	{ fDxy = pt; }
	 virtual void setAlterSize(float size)		{ fAlterSize = size; }

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

	  NVPoint 	offset;
	  NVPoint 	fDxy;				// can only be set with alter tag
	  float 	fAlterSize = 1;		// can only be set with alter tag
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
