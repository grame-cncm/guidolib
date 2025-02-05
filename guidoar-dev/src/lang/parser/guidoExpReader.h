/*
  GUIDO Library
  Copyright (C) 2008  Grame

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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __guidoExpReader__
#define __guidoExpReader__

#include <map>
#include <string>

#include "guidoexpression.h"
#include "glangreader.h"

namespace guidolang 
{

//______________________________________________________________________________
/*!
\brief	An concrete class supporting the glang parser interface.
*/
class guidoExpReader : public glangreader
{ 
	public:
		typedef std::map<std::string, SGLExpr> ExpList;
		
		int				version() const;
		const char*		versionStr() const;

		bool parseFile  (FILE* fd);
		bool parseFile  (const char* file);
		bool parseString(const char* string);

		virtual void	 newIDExpr			(const char *, SGLExpr*);
		virtual SGLExpr* newScoreExpr		(const char *);
		virtual SGLExpr* newBinaryExpr		(const char *, SGLExpr*, SGLExpr*);
		virtual SGLExpr* newAbstractExpr	(const char *, SGLExpr*, SGLExpr*);
		virtual SGLExpr* newNamedExpr		(const char *);
		virtual SGLExpr* newGroupExpr		(SGLExpr*);

		virtual int error(const char * msg, int lineno);

		virtual void			clear()					{ return fExprMap.clear(); }
		virtual const ExpList&	getEnv()				{ return fExprMap; }
		virtual SGLExpr			getId(const std::string& id);

	protected:
		ExpList	fExprMap;
};

} // namespace

#endif
