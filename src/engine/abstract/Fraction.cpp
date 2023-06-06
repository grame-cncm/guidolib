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

#include <cfloat>
#include <climits>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>

#include "Fraction.h"

Fraction::Fraction(double x)
{
//	assert(false);
	numerator=0;
	denominator=1;
	dval = 0.0;

	// ATTENTION kf: is that ok
	//   -> Where are the exactnesslimits
	if (x > -DBL_MIN && x < DBL_MIN) 		// minimum normalised double
		return;

	 // following from LEDA-R 3.3 _rational.c
	 if (x != 0.0)
	 { int neg = (x < 0);
		if (neg) x = -x;

		const unsigned shift = 15;   // a safe shift per step
		const double width = 32768.0;  // = 2^shift
		const int maxiter = 20;      // ought not be necessary, but just in case,
											  // max 300 bits of precision
		int expt;
		double mantissa = frexp(x, &expt);
		int exponent = (int) expt;
		double intpart;
		int k = 0;
		while (mantissa != 0.0 && k++ < maxiter) {
		  mantissa *= width; // shift double mantissa
		  mantissa = modf(mantissa, &intpart);
		  numerator <<= shift;
		  numerator += (int)intpart;
		  exponent -= shift;
		}
		if (exponent > 0)
		  numerator <<= (unsigned)exponent;
		else if (exponent < 0)
		  denominator <<= (unsigned)(-exponent);
		if (neg)
		  numerator = -numerator;
	 } // if (x != 0) then
	 (*this).normalize();

	 // if denomator == 0 (not allowed) the number will be set to
	 // its  MAX-value
	 if (denominator == 0)
		{
		numerator = INT_MAX;
		denominator = 1;
		}


	// not needed!
	// dval = (double) numerator / (double) denominator;
}

Fraction::Fraction( int theNumerator, int theDenominator )
				: numerator(theNumerator), denominator(theDenominator)
{
//	assert( denominator != 0 ); // denominator must not be 0
	dval = (double)theNumerator / (double)theDenominator;
}

void Fraction::set(long long num, long long denom)
{
	assert( denom != 0 ); // denominator must not be 0
	numerator = num;
	denominator = denom;
	dval = (double)num / (double)denom;
}


Fraction::operator std::string() const		{
	std::stringstream s;
	print (s);
	return s.str();
}

void Fraction::print(std::ostream& out) const
{
    out << numerator << "/" << denominator;
}

std::ostream& operator<< (std::ostream& os, const Fraction& f) { f.print(os); return os; }

/** Simplifies the fraction and recalculates the dval
*/
void Fraction::normalize()
{
	 // following from LEDA-R 3.3 _rational.c

	// divide numerator and denominator by their greatest common divisor
	// denominator is assumed to be nonzero and positive
	if (numerator == denominator)
	{
		  numerator = denominator = 1;
		  dval = 1.0;
		  return;
	}
	if (-numerator == denominator)
	{
		numerator = -1; denominator = 1;
		dval = -1.0;
		return;
	}
	
	const long long ggt = gcd(numerator, denominator);
//	if (ggt != 1)
//	{
	  numerator /= ggt;
	  denominator /= ggt;
//	}

	if (denominator < 0)
	{
		numerator *= -1;
		denominator *= -1;
	}
	dval = (double)numerator / (double)denominator;
}

// static function
long long Fraction::gcd(long long i1, long long i2)
{
// gcd -- calculate the GCD for two integer values
// Input: Two int numbers
// Output: Greatest common denominator
	  long long temp;
	  while (i2) {					// While non-zero value
		temp = i2;					// Save current value
		i2 = i1 % i2;				// Assign remainder of division
		i1 = temp;					// Copy old value
	  }
	  return i1;					// Return GCD of numbers
}

// smallest common multiple
long long Fraction::scm(long long i1, long long i2)
{	
	if (i1 == i2) return i1;

	const long long tmp1 = gcd(i1, i2);
	return (( i1 / tmp1 ) * i2 );
}

void Fraction::setDenominator(long long newDenominator)
{
	assert(newDenominator!=0);
	denominator = newDenominator;
	dval = (double)numerator / (double)newDenominator;
}

void Fraction::setNumerator(long long newNumerator)
{
	numerator = newNumerator;
	dval = (double)newNumerator / (double)denominator;
}

// create some often used fractions

const Fraction Frac_n1(-1, 1);
const Fraction Frac_0(0, 1);
const Fraction Frac_7_4(7, 4);
const Fraction Frac_3_2(3,2);
const Fraction Frac_1(1);
const Fraction Frac_7_8(7,8);
const Fraction Frac_3_4(3,4);
const Fraction Frac_1_2(1,2);
const Fraction Frac_7_16(7,16);
const Fraction Frac_3_8(3,8);
const Fraction Frac_1_4(1,4);
const Fraction Frac_7_32(7,32);
const Fraction Frac_3_16(3,16);
const Fraction Frac_1_8(1,8);
const Fraction Frac_7_64(7,64);
const Fraction Frac_3_32(3,32);
const Fraction Frac_3_64(3,64);
const Fraction Frac_3_128(3,128);
const Fraction Frac_7_128(7,128);
const Fraction Frac_7_256(7,256);
const Fraction Frac_1_12(1,12);
const Fraction Frac_1_16(1,16);
const Fraction Frac_1_32(1,32);
const Fraction Frac_1_64(1,64);
const Fraction Frac_1_128(1,128);

const Fraction Frac_Max(0xffff,1);

void Fraction::add (const Fraction &inValue)
{
	const long long scmv = scm( inValue.denominator, denominator );
	const long long mul1 = scmv / denominator;
	const long long mul2 = scmv / inValue.denominator;

	numerator = numerator * mul1 + inValue.numerator * mul2;
	denominator = scmv;
}

Fraction & Fraction::operator +=(const Fraction & inValue)
{
	add (inValue);
//	// smallest common multiple 
//	const long long scmv = scm( inValue.denominator, denominator );
//	const long long mul1 = scmv / denominator;
//	const long long mul2 = scmv / inValue.denominator;
//
//	numerator = numerator * mul1 + inValue.numerator * mul2;
//	denominator = scmv;
//	// done by normalize: dval = numerator / (double)denominator;
	normalize();
	return (*this);
}

Fraction & Fraction::operator -=(const Fraction & inValue)
{
	// smallest common multiple 
	const long long scmv = scm(inValue.denominator, denominator);
	const long long mul1 = scmv / denominator;
	const long long mul2 = scmv / inValue.denominator;

	numerator = numerator * mul1 - inValue.numerator * mul2;
	denominator = scmv;
	// done by normalize: dval = numerator / (double)denominator;
	normalize();
	return (*this);

}

// Modulo-Operation for fractions 
Fraction & Fraction::operator %=(const Fraction & inValue)
{
	// smallest common multiple 
	const long long scmv = scm(inValue.denominator, denominator);
	const long long mul1 = scmv / denominator;
	const long long mul2 = scmv / inValue.denominator;

	numerator = (numerator * mul1) % (inValue.numerator * mul2);
	denominator = scmv;
	// done by normalize: dval = numerator / (double)denominator;
	normalize();
	return (*this);
}

// Multiply
Fraction & Fraction::operator *=(const Fraction & inValue)
{
	const long long saveDenominator = denominator;

	denominator = inValue.denominator;
	normalize();

	numerator *= inValue.numerator;
	denominator *= saveDenominator;
	normalize();

	// done by normalize: dval = numerator / (double)denominator;
	return (*this);

/*	Fraction tmp;
	tmp.numerator = tmp1.numerator;
	tmp.denominator = tmp2.denominator;
	tmp.normalize();

	tmp.numerator *= tmp2.numerator;
	tmp.denominator *= tmp1.denominator;
	tmp.normalize();

	//tmp.numerator = tmp1.numerator * tmp2.numerator;
	//tmp.denominator = tmp1.denominator * tmp2.denominator;
	tmp.dval = tmp.numerator / (double) tmp.denominator;
	// tmp.normalize();
	return tmp;
*/
}

Fraction & Fraction::operator *=(const int tmp)
{
	numerator *= tmp;
	// done by normalize: dval = numerator / (double)denominator;
	normalize();
	return (*this);
}

Fraction operator +(const Fraction &tmp1, const Fraction &tmp2)
{
	Fraction outValue (tmp1);
	outValue += tmp2;
	return outValue;
}

// could be done also bei adding the negativ value (* -1)
Fraction operator -(const Fraction &tmp1, const Fraction &tmp2)
{
	Fraction outValue (tmp1);
	outValue -= tmp2;
	return outValue;
}

Fraction operator %(const Fraction &tmp1, const Fraction &tmp2)
{
	Fraction outValue (tmp1);
	outValue %= tmp2;
	return outValue;
}

Fraction operator *(const Fraction &tmp1, const Fraction &tmp2)
{
	Fraction outValue (tmp1);
	outValue *= tmp2;
	return outValue;
}

Fraction operator *(const Fraction &tmp1, const int multiplier)
{
	Fraction outValue (tmp1);
	outValue *= multiplier;
	return outValue;
}

// this functions tests if a fraction
// is of the from m/(n^k)
// this needs to be done for the creation of n-tuplets
// ATTENTION: it normalises the fraction! Therefore not possible
// with const. 
// Result: exponent (>=0) or -1
int Fraction::isMultiple(int n)
{
	normalize();

	// here are some problems with the Fractions (depending if Win16 or WIN32)
	// Therefore Integer solution
	int tmp = n;
	int cnt = 1;

	if (denominator == 1)
		return 0;

	while (tmp < denominator)
	{
		tmp *= n;
		++cnt;
	}
	if (denominator == tmp)
		return cnt;

	return -1;
}

/** \brief Returns true if the fraction is even or equal to zero. 
	
	Returns false if the fraction is odd, or has a decimal part.

*/
bool Fraction::isEven() const
{
	const double floorVal = floor( dval );
	if( floorVal != dval ) return false;
	
	const int intVal = (int)floorVal;
	return ((intVal | 1) != intVal);
}

/** \brief Returns true if the fraction is odd. 
	
	Returns false if the fraction is even, or has a decimal part.

*/
bool Fraction::isOdd() const
{
	const double floorVal = floor( dval );
	if( floorVal != dval ) return false;
	
	const int intVal = (int)floorVal;
	return ((intVal | 1) == intVal);
}

Fraction Fraction::getBiggestFullNote(int exp) const
{
	double f = (double) *this;

	int power;
	if (exp == 1)
		power = 1;
	else
		power = (int) ceil( - log(f) / log(double(exp)) );
	
	if (power < 0)
		power = 0;
	
	power = (int)pow(double(exp),power);

	return Fraction(1, power);
}

Fraction Fraction::getReallySmallerNote(int exp) const
{
	double f = (double) *this;

	int power;
	if (exp == 1)
		power = 1;
	else
		power = (int) ceil( - log(f) / log(double(exp)) ) + 1;

	if (power < 0)
		power = 0;
	
	power = (int)pow(double(exp),power);

	return Fraction(1,power);
}

void Fraction::invert() 
{
	const long long oldnumerator = numerator;
	numerator = denominator;
	denominator = oldnumerator;
	if (denominator == 0)
	{
		denominator = 1;
		numerator = 0;
	}
	normalize();
}

