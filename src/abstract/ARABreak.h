#ifndef ARABreak_H
#define ARABreak_H

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

#include "ARMTParameter.h"

class ARMusicalObject;
class ARMusicalVoiceState;

/** \brief The Break tag
*/

class ARABreak : public ARMTParameter   // public ARMusicalObject
{
	friend class ARMusicalVoiceState;

public:

	enum breakstate { ON, OFF };

					ARABreak();
	virtual 		~ARABreak();

	breakstate getSystemBreakState() const { return mSystemBreakState; }
	breakstate getPageBreakState() const { return mPageBreakState; } 
	virtual void setTagParameterList(TagParameterList & tpl);
	virtual void PrintName( std::ostream & os) const;
	virtual void PrintParameters( std::ostream & os) const;

	virtual void print() const { }
	virtual bool IsStateTag() const { return true; }

protected:

	breakstate mPageBreakState;
	breakstate mSystemBreakState;

	static ListOfTPLs ltpls;
};

#endif
