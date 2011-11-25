#ifndef Fraction_H
#define Fraction_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

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

*/

#include <iostream>

/** \brief Numerator and denominator
*/
class Fraction
{
  public:

		Fraction() : numerator(0), denominator(1), dval(0.0) { }

		Fraction( int theNumerator, int theDenominator );
		Fraction( double x );
		Fraction( int value ) : numerator(value), denominator(1)	{ dval = (double)value; }

		void print(std::ostream&) const;

		int getNumerator() const;
		int getDenominator() const;
		void normalize();

		// greatest common denominator
		static int gcd(int i1, int i2);
		
		// smallest common multiple
		static int scm(int i1, int i2);

		void set( int num, int denom = 1 );
		void setDenominator(int newDenominator);
		void setNumerator(int newNumerator);

		void invert(); 

		// test if fraction is like m/(n^k) is used for the tuplet creation
		int isMultiple(int n);		// Result: exponent (>=0) or -1

		bool 	isOdd() const;
		bool	isEven() const;

      Fraction getBiggestFullNote(int exp) const;
      Fraction getReallySmallerNote(int exp) const;

		operator double() const;
		operator float() const;
		bool operator >(const Fraction & in) const  { return (dval > in.dval); }
		bool operator ==(const Fraction & in) const { return (dval == in.dval); }
		bool operator !=(const Fraction & in) const { return (dval != in.dval); }
		bool operator <(const Fraction & in) const  { return (dval < in.dval); }
		bool operator >=(const Fraction & in) const { return (dval >= in.dval); }
		bool operator <=(const Fraction & in) const { return (dval <= in.dval); }
		Fraction & operator +=(const Fraction &tmp);
		Fraction & operator -=(const Fraction &tmp);
		Fraction & operator %=(const Fraction &tmp);
 		Fraction & operator *=(const Fraction &tmp);
		Fraction & operator *=(const int tmp);

	private:
		int numerator;
		int denominator;
		double dval;
};

std::ostream& operator<< (std::ostream& os, const Fraction& f);


//## Other Operations (inline)
inline Fraction::operator float() const			{ return (float)dval; }
inline Fraction::operator double() const		{ return dval; }

inline int Fraction::getNumerator() const		{ return numerator; }
inline int Fraction::getDenominator() const		{ return denominator; }

Fraction operator +(const Fraction & tmp1, const Fraction & tmp);
Fraction operator -(const Fraction & tmp1, const Fraction & tmp);
Fraction operator *(const Fraction & tmp1, const Fraction & tmp);
Fraction operator *(const Fraction & tmp1, const int multiplier);
Fraction operator %(const Fraction & tmp1, const Fraction & tmp2);

extern const  Fraction Frac_n1;
extern const  Fraction Frac_0;
extern const  Fraction Frac_7_4;
extern const  Fraction Frac_3_2;
extern const  Fraction Frac_1;
extern const  Fraction Frac_7_8;
extern const  Fraction Frac_3_4;
extern const  Fraction Frac_1_2;
extern const  Fraction Frac_7_16;
extern const  Fraction Frac_3_8;
extern const  Fraction Frac_1_4;
extern const  Fraction Frac_7_32;
extern const  Fraction Frac_3_16;
extern const  Fraction Frac_1_8;
extern const  Fraction Frac_7_64;
extern const  Fraction Frac_3_32;
extern const  Fraction Frac_3_64;
extern const  Fraction Frac_3_128;
extern const  Fraction Frac_7_128;
extern const  Fraction Frac_7_256;
extern const  Fraction Frac_1_12;
extern const  Fraction Frac_1_16;
extern const  Fraction Frac_1_32;
extern const  Fraction Frac_1_64;
extern const  Fraction Frac_1_128;
extern const  Fraction Frac_Max;


#endif

