/*
  Copyright � Grame 2003

  This library is free software; you can redistribute it and modify it under
  the terms of the GNU Library General Public License as published by the
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  research@grame.fr
  
*/

#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <cmath>
#include "guidorational.h"

using namespace std;

namespace guido
{

string rational::toString() const
{
    ostringstream res;
    res << fNumerator << "/" << fDenominator;
    return res.str();
}

rational::operator string() const
{
	return toString();
}

rational::operator double() const
{ 
	return toDouble(); 
}

rational::operator float() const
{ 
	return toFloat(); 
}

rational::operator int() const
{ 
	const double x = toDouble();
	return ((int)floor(x + 0.5f));
}

rational::rational(const string &str)
{
    const char *cstr;
    cstr = str.c_str();
    const char *denom;
    denom = strstr(cstr,"/");
    if (denom) ++denom;
    fNumerator = atol(cstr);
    if (denom) fDenominator = atol(denom);
}

rational::rational(long int num, long int denom) : fNumerator(num), fDenominator(denom)
{
    // don't allow zero denominators!
    if (fDenominator == 0) fDenominator = 1;
}

rational::rational(const rational& d)
{
    fNumerator = d.fNumerator;
    fDenominator = d.fDenominator;
}

rational rational::operator +(const rational &dur) const {
    return rational(fNumerator * dur.fDenominator + dur.fNumerator * fDenominator, fDenominator * dur.fDenominator);
}

rational rational::operator -(const rational &dur) const {
    return rational(fNumerator * dur.fDenominator - dur.fNumerator * fDenominator, fDenominator * dur.fDenominator);
}

rational rational::operator *(const rational &dur) const {
    return rational(fNumerator * dur.fNumerator, fDenominator * dur.fDenominator);
}

rational rational::operator /(const rational &dur) const {
    return rational(fNumerator * dur.fDenominator, fDenominator * dur.fNumerator);
}

rational rational::operator *(int num) const {
	return rational(fNumerator * num, fDenominator);
}

rational rational::operator /(int num) const {
	return rational(fNumerator, fDenominator * num);
}

rational& rational::operator +=(const rational &dur)
{
    if(fDenominator == dur.fDenominator) {
		fNumerator += dur.fNumerator;
	} else {
		fNumerator = fNumerator * dur.fDenominator + dur.fNumerator * fDenominator;
		fDenominator *= dur.fDenominator;
	}
    return (*this);
}

rational& rational::operator -=(const rational &dur)
{
	if(fDenominator == dur.fDenominator) {
		fNumerator -= dur.fNumerator;
	} else {
		fNumerator = fNumerator * dur.fDenominator - dur.fNumerator * fDenominator;
		fDenominator *= dur.fDenominator;
    }
	return (*this);
}

rational& rational::operator *=(const rational &dur)
{
	fNumerator   *= dur.fNumerator;
    fDenominator *= dur.fDenominator;
    return (*this);
}

rational& rational::operator /=(const rational &dur)
{
    fNumerator   *= dur.fDenominator;
    fDenominator *= dur.fNumerator;
    return (*this);
}

rational& rational::operator =(const rational& dur) {
    fNumerator   = dur.fNumerator;
    fDenominator = dur.fDenominator;
    return (*this);
}

bool rational::operator >(const rational &dur) const
{
    // a/b > c/d if and only if a * d > b * c.
    return ((fNumerator * dur.fDenominator) > (fDenominator * dur.fNumerator));
}

bool rational::operator <(const rational &dur) const
{
    // a/b < c/d if and only if a * d < b * c.
    return ((fNumerator * dur.fDenominator) < (fDenominator * dur.fNumerator));
}

bool rational::operator ==(const rational &dur) const
{
    // a/b < c/d if and only if a * d < b * c.
    return ((fNumerator * dur.fDenominator) == (fDenominator * dur.fNumerator));
}

bool rational::operator >(double num) const
{	
	return (toDouble() > num);
}

bool rational::operator >= (double num) const
{
	return (toDouble() >= num);
}

bool rational::operator <(double num) const
{
	return (toDouble() < num);
}

bool rational::operator <=(double num) const
{
	return (toDouble() <= num);
}

bool rational::operator ==(double num) const
{
	return (toDouble() == num);
}

// gcd(a, b) calculates the gcd of a and b using Euclid's algorithm.
long int rational::gcd(long int a1, long int b1)
{
    long int r;

    long int a = (long int)abs(double(a1));
    long int b = (long int)abs(double(b1));

    if (!(a == 0) || (b == 0)){
        while (b > 0){
            r = a % b;
            a = b;
            b = r;
        }
        return a;
    }
    return 1;
}

rational& rational::rationalise()
{
    long int g = gcd(fNumerator, fDenominator);
    fNumerator /= g;
    fDenominator /= g;
    if (fNumerator == 0) fDenominator = 1;
	return *this;
}

double rational::toDouble() const
{
    return (fDenominator != 0) ? ((double)fNumerator/(double)fDenominator) : 0;
}

float rational::toFloat() const
{
    return (fDenominator != 0) ? ((float)fNumerator/(float)fDenominator) : 0;
}

ostream& operator << (ostream& os, rational r) {
	os << string(r);
	return os;
}

ostream& operator << (ostream& os, rationals l) {
	rationals::const_iterator i = l.begin();
	while (i!=l.end()) {
		os << string(*i++);
		if (i!=l.end()) os << ", ";
	}
	return os;
}

}
