/*
File Name:	EGRAF.H

Copyright 1999 Coda Music Technology (all rights reserved)

Author:  	Randy Stokes
Created:	06/02/99

Purpose:

Modification History:

*//*~*/
/*
$Log: egraf.h,v $
Revision 1.1.1.1  2002/12/19 19:13:11  dfober
first import

** 
** 14    8/16/99 12:02p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 13    7/22/99 3:21 PM ccianflo
** moved HFIXPOINT to egraf.h from Mac HARDROTS.CPP
** 
** 12    7/20/99 4:33p Rstokes
** ===== shipped Finale Mac 2000 =====
** 
** 11    6/17/99 3:36 PM ccianflo
** Moved some stuff from egraf.h to efix.h
** 
** 10    6/17/99 1:32p Rstokes
** removed outdated comments; made sure operator=() returns the right type
** 
** 9     6/17/99 11:00 AM Tfischer
** Renamed IsEmpty() -> IsNull().  Added new IsEmpty, which determines
** whether a rect has zero area or is inverted.
** 
** 8     6/15/99 5:34p Rstokes
** made some inlines take const refs
** 
** 7     6/15/99 2:37 PM ccianflo
** added EPOINT::ToEFIXPOINT()
** 
** 6     6/14/99 10:59 AM ccianflo
** added a bunch of useful methods to our point classes
** 
** 5     6/09/99 12:24p Rstokes
** added ClearRect() to various rectangle classes
** 
** 4     6/07/99 2:25p Rstokes
** comments, reversed params on EHRECT
*/

#ifndef _EGRAF_H_
#define _EGRAF_H_

#ifndef _VERSION_H_
#include "version.h"
#endif

// Forward declarations for classes/structs defined here.

class EHPOINT;
class EHRECT;
struct HFIXPOINT;
struct EPOINT;
struct EFIXPOINT;
struct FPOINT;
class ERECT;
class EFRECT;
struct ETOFFSET;
struct ETPOINT;

// 6/2/99: EHPOINT is a class wrapper around the native point structs for the OS.
// It adds no data members or virtual functions, so it is identical in size to
// the native struct, but has some nice member functions. Note that the access
// members take and return the native component type (LONG and short). This
// ensures that the code will behave exactly as it would with the native structs,
// but points up the potential problem that the structs really aren't compatible.

#if OPERATING_SYSTEM == WINDOWS
typedef LONG OS_BASE_UNIT;
#elif OPERATING_SYSTEM == MAC_OS
typedef short OS_BASE_UNIT;
#endif

class EHPOINT : public WINCODE(POINT) MACCODE(Point)
{
public:
#if OPERATING_SYSTEM == WINDOWS

	EHPOINT()
	{
		this->x = 0;
		this->y = 0;
	}
	EHPOINT( const OS_BASE_UNIT ix, const OS_BASE_UNIT iy )
	{
		this->x = ix;
		this->y = iy;
	}
	EHPOINT( const POINT& ipnt )
	{
		this->x = ipnt.x;
		this->y = ipnt.y;
	}
	EHPOINT& operator=( const POINT& ipnt )
	{
		if ( this != &ipnt )
		{
			this->x = ipnt.x;
			this->y = ipnt.y;
		}
		return *this;
	}

	// field names are different between platforms, so we need these common access functions
	OS_BASE_UNIT GetX() const {	return this->x; }
	OS_BASE_UNIT GetY() const {	return this->y; }
	void SetX( const OS_BASE_UNIT nx ) {	this->x = nx; }
	void SetY( const OS_BASE_UNIT ny ) {	this->y = ny; }
	void Set( const OS_BASE_UNIT nx, const OS_BASE_UNIT ny )
	{
		this->x = nx;
		this->y = ny;
	}

#elif OPERATING_SYSTEM == MAC_OS

	EHPOINT()
	{
		this->h = 0;
		this->v = 0;
	}
	EHPOINT( const OS_BASE_UNIT ix, const OS_BASE_UNIT iy )
	{
		this->h = ix;
		this->v = iy;
	}
	EHPOINT( const Point& ipnt )
	{
		this->h = ipnt.h;
		this->v = ipnt.v;
	}
	EHPOINT& operator=( const Point& ipnt )
	{
		if ( this != &ipnt )
		{
			this->h = ipnt.h;
			this->v = ipnt.v;
		}
		return *this;
	}

	// field names are different between platforms, so we need these common access functions
	OS_BASE_UNIT GetX() const {	return this->h; }
	OS_BASE_UNIT GetY() const {	return this->v; }
	void SetX( const OS_BASE_UNIT nx ) {	this->h = nx; }
	void SetY( const OS_BASE_UNIT ny ) {	this->v = ny; }
	void Set( const OS_BASE_UNIT nx, const OS_BASE_UNIT ny )
	{
		this->h = nx;
		this->v = ny;
	}

#endif
	~EHPOINT() {}

	tbool IsEmpty( void ) const
	{
		return ( this->GetX() == 0 && this->GetY() == 0 );
	}
	
	// Would it be better to use GetX() SetX(), etc. below?
	inline void Offset(OS_BASE_UNIT dx, OS_BASE_UNIT dy)
		{  MACCODE( h += dx; v += dy; )
		   WINCODE( x += dx; y += dy; ) }
	inline void Offset( const EHPOINT& pt )	// Same as operator+
		{	MACCODE( h += pt.h;	v += pt.v; )
			WINCODE( x += pt.x;	y += pt.y; ) }
	
	inline bool operator==( const EHPOINT& pt ) const
		{ 	MACCODE( return (h == pt.h && v == pt.v); )
			WINCODE( return (x == pt.x && y == pt.y); ) }
	inline bool operator!=( const EHPOINT& pt ) const
		{ 	MACCODE( return (h != pt.h || v != pt.v); )
			WINCODE( return (x != pt.x || y != pt.y); ) }
	
	inline void operator+=( const EHPOINT& pt )
		{ 	MACCODE( h += pt.h;	v += pt.v; )
			WINCODE( x += pt.x;	y += pt.y; ) }
	inline void operator-=( const EHPOINT& pt )
		{ 	MACCODE( h -= pt.h;	v -= pt.v; )
			WINCODE( x -= pt.x;	y -= pt.y; ) }
		
	inline EHPOINT operator+( const EHPOINT& pt ) const
		{ 	MACCODE( return EHPOINT(h + pt.h, v + pt.v); )
			WINCODE( return EHPOINT(x + pt.x, y + pt.y); ) }
	inline EHPOINT operator-( const EHPOINT& pt ) const
		{ 	MACCODE( return EHPOINT(h - pt.h, v - pt.v); )
			WINCODE( return EHPOINT(x - pt.x, y - pt.y); ) }
	
	inline EHPOINT operator-() const
		{ 	MACCODE( return EHPOINT(-h, -v); )
			WINCODE( return EHPOINT(-x, -y); ) }
};

#if OPERATING_SYSTEM == MAC_OS
// evs 10/20/98 - MAKE_EHPOINT does not work as would be expected on Windows, the point that it creates
// will not work in Windows API calls.  Using this macro to extract the mouse location from the LPARAM in
// a dialog proc will fail, even though that is exactly how it is used on Mac. In all but one case it
// is not used in any Windows code. I took care of that case in CHDEFDLG today but it seems safest
// not to allow Windows code to use this macro. A compile error is a lot
// easier to track then a run time error.
#define MAKE_EHPOINT(l)	    (*((EHPOINT *)&(l)))
#endif

#if OPERATING_SYSTEM == MAC_OS
#define EHPOINT_X(p)	    (p).h
#define EHPOINT_Y(p)	    (p).v
#elif OPERATING_SYSTEM == WINDOWS
#define EHPOINT_X(p)	    (p).x
#define EHPOINT_Y(p)	    (p).y
#endif

// 6/2/99: EHRECT is a class wrapper around the native rect structs for the OS.
// It adds no data members or virtual functions, so it is identical in size to
// the native struct, but has some nice member functions. Note that the access
// members take and return the native component type (LONG and short). This
// ensures that the code will behave exactly as it would with the native structs,
// but points up the potential problem that the structs really aren't compatible.
//
// Unlike EHPOINT, Mac and Windows use the same field names for their native
// rect structs. However, be aware that the order of the fields is completely
// different -- another incompatability.

class EHRECT : public WINCODE(RECT) MACCODE(Rect)
{
public:
	EHRECT()
	{
		this->top = 0;
		this->left = 0;
		this->bottom = 0;
		this->right = 0;
	}
	EHRECT( const OS_BASE_UNIT ileft, const OS_BASE_UNIT itop,  
		const OS_BASE_UNIT iright, const OS_BASE_UNIT ibottom )
	{
		this->top = itop;
		this->left = ileft;
		this->bottom = ibottom;
		this->right = iright;
	}
	EHRECT( const EHPOINT& itopleft, const EHPOINT& ibottomright )
	{
		this->top = itopleft.GetY();
		this->left = itopleft.GetX();
		this->bottom = ibottomright.GetY();
		this->right = ibottomright.GetX();
	}
#if OPERATING_SYSTEM == WINDOWS
	EHRECT( const RECT& irect )
#elif OPERATING_SYSTEM == MAC_OS
	EHRECT( const Rect& irect )
#endif
	{
		this->top = irect.top;
		this->left = irect.left;
		this->bottom = irect.bottom;
		this->right = irect.right;
	}

#if OPERATING_SYSTEM == WINDOWS
	// For conversion to a RECT *:
	// (6/4/99 RES: I don't understand why this is necessary... isn't a pointer
	// to EHRECT by definition a pointer to RECT as well?
	inline operator RECT*() { return this; }
	inline operator const RECT*() const { return this; }
#elif OPERATING_SYSTEM == MAC_OS
	// For conversion to a Rect *:
	// (6/4/99 RES: I don't understand why this is necessary... isn't a pointer
	// to EHRECT by definition a pointer to Rect as well?
	inline operator Rect*() { return this; }
	inline operator const Rect*() const { return this; }
#endif
	
#if OPERATING_SYSTEM == WINDOWS
	EHRECT& operator=( const RECT& irect )
#elif OPERATING_SYSTEM == MAC_OS
	EHRECT& operator=( const Rect& irect )
#endif
	{
		if ( this != &irect )
		{
			this->top = irect.top;
			this->left = irect.left;
			this->bottom = irect.bottom;
			this->right = irect.right;
		}
		return *this;
	}

	~EHRECT() {}

	// TSF 6/17/99:  IsNull determines if a rect has all-zero coordinates
	tbool IsNull( void ) const
	{
		return ( this->top == 0 && this->left == 0
			&& this->bottom == 0 && this->right == 0 );
	}
	
	// TSF 6/17/99:  IsEmpty determines if a rect has zero area, or is inverted.
	tbool IsEmpty( void ) const
	{
		return (( this->bottom <= this->top ) ||
				( this->right <= this->left ));
	}
	
	void ClearRect()
	{
		this->top = 0;
		this->left = 0;
		this->bottom = 0;
		this->right = 0;
	}
	void SetRect( const OS_BASE_UNIT ileft, const OS_BASE_UNIT itop,  
		const OS_BASE_UNIT iright, const OS_BASE_UNIT ibottom )
	{
		this->top = itop;
		this->left = ileft;
		this->bottom = ibottom;
		this->right = iright;
	}
};

struct HFIXPOINT
{
public:
	HFIX x;
	HFIX y;

	HFIXPOINT() : x(0), y(0) {}
	HFIXPOINT( const HFIX ix, const HFIX iy ) :
		x(ix),
		y(iy)
	{}
	
	inline void Offset(HFIX dx, HFIX dy)
		{  x += dx;		y += dy; }
	inline void Offset( const HFIXPOINT& pt )	// Same as operator+
		{  x += pt.x;	y += pt.y; }
	
	inline bool operator==( const HFIXPOINT& pt ) const
		{ return (x == pt.x && y == pt.y); }
	inline bool operator!=( const HFIXPOINT& pt ) const
		{ return (x != pt.x || y != pt.y); }
	
	inline void operator+=( const HFIXPOINT& pt )
		{ x += pt.x;	y += pt.y; }
	inline void operator-=( const HFIXPOINT& pt )
		{ x -= pt.x;	y -= pt.y; }
		
	inline HFIXPOINT operator+( const HFIXPOINT& pt ) const
		{ return HFIXPOINT(x + pt.x, y + pt.y); }
	inline HFIXPOINT operator-( const HFIXPOINT& pt ) const
		{ return HFIXPOINT(x - pt.x, y - pt.y); }
	
	inline HFIXPOINT operator-() const
		{ return HFIXPOINT(-x, -y); }
};

/* generic Enigma Point structure: */

struct EPOINT
{
public:
	EVPU x;
	EVPU y;

	EPOINT() : x(0), y(0) {}
	EPOINT( const EVPU ix, const EVPU iy ) :
		x(ix),
		y(iy)
	{}

	inline void Offset(EVPU dx, EVPU dy)
		{  x += dx;		y += dy; }
	inline void Offset( const EPOINT& pt )	// Same as operator+
		{  x += pt.x;	y += pt.y; }
	
	inline bool operator==( const EPOINT& pt ) const
		{ return (x == pt.x && y == pt.y); }
	inline bool operator!=( const EPOINT& pt ) const
		{ return (x != pt.x || y != pt.y); }
	
	inline void operator+=( const EPOINT& pt )
		{ x += pt.x;	y += pt.y; }
	inline void operator-=( const EPOINT& pt )
		{ x -= pt.x;	y -= pt.y; }
		
	inline EPOINT operator+( const EPOINT& pt ) const
		{ return EPOINT(x + pt.x, y + pt.y); }
	inline EPOINT operator-( const EPOINT& pt ) const
		{ return EPOINT(x - pt.x, y - pt.y); }
	
	inline EPOINT operator-() const
		{ return EPOINT(-x, -y); }
	
	// ---
	inline EFIXPOINT ToEFIXPOINT( void ) const;	// See EFIX.H
};

struct EFIXPOINT
{
public:
	EFIX x;
	EFIX y;

	EFIXPOINT() : x(0), y(0) {}
	EFIXPOINT( const EFIX ix, const EFIX iy ) :
		x(ix),
		y(iy)
	{}
	
	// CFC 06/11/99: I suppose the next step is to make a point
	// template class so we do not have to duplicate all this
	// stuff for each point class, EPOINT, EHPOINT, EFIXPOINT, FPOINT, etc.
	// For now I have duplicated everything.  I used CPoint in MFC
	// as a model for some of these routines.
	
	inline void Offset(EFIX dx, EFIX dy)
		{  x += dx;		y += dy; }
	inline void Offset( const EFIXPOINT& pt )	// Same as operator+
		{  x += pt.x;	y += pt.y; }
	
	inline bool operator==( const EFIXPOINT& pt ) const
		{ return (x == pt.x && y == pt.y); }
	inline bool operator!=( const EFIXPOINT& pt ) const
		{ return (x != pt.x || y != pt.y); }
	
	inline void operator+=( const EFIXPOINT& pt )
		{ x += pt.x;	y += pt.y; }
	inline void operator-=( const EFIXPOINT& pt )
		{ x -= pt.x;	y -= pt.y; }
		
	inline EFIXPOINT operator+( const EFIXPOINT& pt ) const
		{ return EFIXPOINT(x + pt.x, y + pt.y); }
	inline EFIXPOINT operator-( const EFIXPOINT& pt ) const
		{ return EFIXPOINT(x - pt.x, y - pt.y); }
	
	inline EFIXPOINT operator-() const
		{ return EFIXPOINT(-x, -y); }
		
	// ---
	inline FPOINT ToFPOINT( void ) const; // See EFIX.H
};

struct FPOINT					/* floating point (no pun intended!) */
{
public:
	float x;
	float y;

	FPOINT() : x(0.0), y(0.0) {}
	FPOINT( const float ix, const float iy ) :
		x(ix),
		y(iy)
	{}
	
	inline void Offset(float dx, float dy)
		{  x += dx;		y += dy; }
	inline void Offset( const FPOINT& pt )	// Same as operator+
		{  x += pt.x;	y += pt.y; }
	
	inline bool operator==( const FPOINT& pt ) const
		{ return (x == pt.x && y == pt.y); }
	inline bool operator!=( const FPOINT& pt ) const
		{ return (x != pt.x || y != pt.y); }
	
	inline void operator+=( const FPOINT& pt )
		{ x += pt.x;	y += pt.y; }
	inline void operator-=( const FPOINT& pt )
		{ x -= pt.x;	y -= pt.y; }
		
	inline FPOINT operator+( const FPOINT& pt ) const
		{ return FPOINT(x + pt.x, y + pt.y); }
	inline FPOINT operator-( const FPOINT& pt ) const
		{ return FPOINT(x - pt.x, y - pt.y); }
	
	inline FPOINT operator-() const
		{ return FPOINT(-x, -y); }
	
	// ---
	inline EFIXPOINT ToEFIXPOINT( void ) const; // See EFIX.H

};

/* generic Enigma Rectangle structure: */

class ERECT
{
public:
	EVPU left;
	EVPU top;
	EVPU right;
	EVPU bottom;

	ERECT() : left(0), top(0), right(0), bottom(0) {}
	ERECT( const EVPU ileft, const EVPU itop, 
			const EVPU iright, const EVPU ibottom ) :
		left(ileft),
		top(itop),
		right(iright),
		bottom(ibottom) 
	{}
	ERECT( const EPOINT& itopleft, const EPOINT& ibottomright )
	{
		this->left = itopleft.x;
		this->top = itopleft.y;
		this->right = ibottomright.x;
		this->bottom = ibottomright.y;
	}
	void ClearRect()
	{
		this->top = 0;
		this->left = 0;
		this->bottom = 0;
		this->right = 0;
	}
};

class EFRECT
{
public:
	EFLOAT left;
	EFLOAT top;
	EFLOAT right;
	EFLOAT bottom;

	EFRECT() : left(0.0), top(0.0), right(0.0), bottom(0.0) {}
	EFRECT( const EFLOAT ileft, const EFLOAT itop, 
			const EFLOAT iright, const EFLOAT ibottom ) :
		left(ileft),
		top(itop),
		right(iright),
		bottom(ibottom) 
	{}
	EFRECT( const FPOINT& itopleft, const FPOINT& ibottomright )
	{
		this->left = itopleft.x;
		this->top = itopleft.y;
		this->right = ibottomright.x;
		this->bottom = ibottomright.y;
	}
	EFRECT( const EPOINT& itopleft, const EPOINT& ibottomright )
	{
		this->left = (EFLOAT)itopleft.x;
		this->top = (EFLOAT)itopleft.y;
		this->right = (EFLOAT)ibottomright.x;
		this->bottom = (EFLOAT)ibottomright.y;
	}
	void ClearRect()
	{
		this->top = 0;
		this->left = 0;
		this->bottom = 0;
		this->right = 0;
	}
};

/* Enigma offset structure (twobyte components): */

// 6/2/99 RES: Holding off for now on making this a class. It is not used very
// often, but smrtshap.h uses it and lots of the aggregate initialization in
// that file would have to be reworked.

struct ETOFFSET
{
public:
	twobyte x;
	twobyte y;
};
//class ETOFFSET
//{
//public:
//	twobyte x;
//	twobyte y;
//
//	ETOFFSET() : x(0), y(0) {}
//	ETOFFSET( const twobyte ix, const twobyte iy ) :
//		x(ix),
//		y(iy)
//	{}
//};

/* Enigma point structure (twobyte components): */

struct ETPOINT
{
public:
	twobyte x;
	twobyte y;

	ETPOINT() : x(0), y(0) {}
	ETPOINT( const twobyte ix, const twobyte iy ) :
		x(ix),
		y(iy)
	{}
};

#endif	// _EGRAF_H_
