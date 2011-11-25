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
#include <string.h>
#include <typeinfo>

#include "secureio.h"

#include "TagParameterInt.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "TagParameterInt.h"

#include "defines.h"	// For GuidoWarn

using namespace std;
/** \brief Manual to using TPL (TagParameterLists)

 A TagParameterList contains a list of TagParameters. 
 It can either be created by supplying a string or by adding one tag after another.
 The second mechanism is used in ARFactory while parsing the tagparameters. 
 The first mechanism is used statically for each parameter-Tag to find matching tag-parameters.
 The string is in the following format:
 "TYPE,NAME,DEFVALUE,REQUIRED" where
 TYPE is one of 
	"U" : a unit parameter (having a unit ("cm","pt" ..)
		OR if none the DEFAULT UNIT set with
		the unit(s) (?) tag.
	"S" : a string parameter
	"I" : an Integer parameter
	"F" : a Float parameter
 NAME is a name that can be used when naming tag
   parameters instead of using the posiitons
 DEFVALUE is the default value of the parameter
  (this makes only sense, if the parameter is optional, in which
   case the defvalue is stored when parsing the tagparameters)
 REQUIRED is either "r" or "o" for required or
   optional parameters.

 Each (Parameter)Tag should define a list
 of TagParameterLists in their desired matching order.
 When the tagparameters are parsed, a match of parameters
 is tried out (by name, by position, by type and so forth)
 The first TPL in the LTPL which matches is returned 
 by number and the values are set accordingly.
 Each tag than should handle the values in its own fashion.

*/


/** \brief Builds a TagParameterList out of a definition string.
	By default it owns elements.
*/
TagParameterList::TagParameterList(const NVstring & str)
		: KF_IPointerList<TagParameter>(1)
{
	// now we have to go through the string and build the TagParmaters according to the specification ...	
	minimum = 0;
	int len = int(str.length()+1);
	char * buffer = new char[len];;
	char * ptr = buffer;
	strncpy(ptr, str.c_str(), len);
	char * oldp;
	bool done = false;
	
	while (ptr && *ptr)
	{
		while (!done)
		{
			oldp = ptr;
			ptr = nextToken (ptr, ',');
			if (!ptr) break;
			
			TagParameter * tp = NULL;

			if (!strcmp("U", oldp))
			{
				TagParameterFloat * tpf = new TagParameterFloat(0.0f);
				tpf->fUnittag = true;
				tp = tpf;
			}
			else if (!strcmp("S",oldp))
				tp = new TagParameterString("");
			else if (!strcmp("F",oldp))
			{
				TagParameterFloat * tpf = new TagParameterFloat(0.0f);
				tpf->fUnittag = false;
				tp = tpf;
			}
			else if (!strcmp("I",oldp))
				tp = new TagParameterInt(0);
			else 
				cerr << "Warning: unknown tag parameter type " << oldp << endl;
			
			oldp = ptr;
			ptr = nextToken (ptr, ',');
			if (!ptr) break;
			
			if (tp) tp->setName(oldp);
			oldp = ptr;
			ptr = nextToken (ptr, ',');
			if (!ptr) break;

			if (tp) tp->setValue(oldp);
			
			oldp = ptr;
			// the last one is different!
			ptr = nextToken (ptr, ';');
			if (!ptr) done = true;

			if (tp) {
				if (!strcmp(oldp,"r"))
				{
					tp->setRequired(1);
					++minimum;
				}
				else if (!strcmp(oldp,"o"))	tp->setRequired(0);	
				AddTail(tp);
			}
		}
	}
	delete [] buffer;
}

/** \brief Used by ARFactory to build the 
	tagparameterlist while parsing tag parameters.
*/
TagParameterList::TagParameterList(int p_owns)
		: KF_IPointerList<TagParameter>(p_owns)
{
	minimum = -1;
}

char* TagParameterList::nextToken(char *ptr, char sep) const
{	
	while (*ptr) {
		if (*ptr == sep) {
			*ptr = 0;
			return ++ptr;
		}
		ptr++;
	}
	return 0;
}

//----------------------------------------------------------------------------------
/** \brief Returns the number of parameters that can be stored maximally.
*/
int TagParameterList::getMaximum() const
{
	// this is just the count of elements in the list
	return GetCount();
}

//----------------------------------------------------------------------------------
/** \brief counts those parameters that are required.
*/
int TagParameterList::getMinimum() const
{	
	if (minimum != -1)
		return minimum;
	
	minimum = 0;
	GuidoPos pos = GetHeadPosition();
	while (pos)
	{
		TagParameter * tp = GetNext(pos);
		if (tp->required)
			++minimum;
	}
	
	return minimum;
}

//----------------------------------------------------------------------------------
// try to find the named parameter in a list.
bool TagParameterList::Match(const TagParameterList* tpl, const TagParameter * tp) const
{
	GuidoPos tmppos = tpl->GetHeadPosition();
	while (tmppos)
	{
		TagParameter *tmptp = tpl->GetNext(tmppos);
		if (tmptp && (tmptp->name == tp->name))
		{
			// found a match				
			// then, we have to compare the type ...
			// also, is the parameter already  set by name? (which means that
			// it was already defined earlier)  if so, this parameter is ignored!?
			if ((tmptp->pflag == TagParameter::NOTSET) || (tmptp->pflag == TagParameter::SETBYPOSITION))
			{
				// name override! now, compare the type ...
				// then, we try to copy the value
				if (!tmptp->copyValue(tp))
				{
					GuidoWarn("Named Parameter has wrong type: ", tp->name.c_str() );
					break;
				}
				tmptp->pflag = TagParameter::SETBYNAME;
			}
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------------------
// check that all required parameters have been set.
bool TagParameterList::checkRequired (const TagParameterList* tpl) const
{
	GuidoPos pos = tpl->GetHeadPosition();
	while (pos) 
	{
		TagParameter * tp = tpl->GetNext(pos);
		if (tp->required && (tp->pflag == TagParameter::NOTSET))
		{
			GuidoWarn("Required tag parameter was not set: ", tp->name.c_str() );
			return false;
		}
	}
	return true;
}

//----------------------------------------------------------------------------------
// get the tp at a given position
TagParameter* TagParameterList::getPosition (const TagParameterList * tpl, int n) const
{
	int i = 0;
	GuidoPos pos = tpl->GetHeadPosition();
	while (pos) 
	{
		TagParameter * tp = tpl->GetNext(pos);
		if (i == n) return tp;
		i++;		
	}
	return 0;
}

//----------------------------------------------------------------------------------
void TagParameterList::copyTemplate	(TagParameterList *dst) const
{
	GuidoPos pos = GetHeadPosition();
	while (pos)
	{
		TagParameter * tp = GetNext(pos);
		if (tp)
		{
			TagParameter * newtp = tp->getCopy();
			newtp->pflag = TagParameter::NOTSET;
			dst->AddTail(newtp);
		}
	}
}


//----------------------------------------------------------------------------------
/** \brief Does the actual matching of 
	the current instance of the TagParameterList (this)
	and another, given TagParameterList.

	The idea is as follows:
	1. Named parameters override
	2. If no name is given, the position determines the value

	a new TagParameterList is build, which
	actually copies the parameters into the
	correct order, and also has all default-values
	of optional (not specified) tags.
*/
TagParameterList * TagParameterList::Match( const TagParameterList & tpl) const
{
	// first, we copy the parameters of the template list (which is *this)
	// the result list owns the parameters ...
	TagParameterList * result = new TagParameterList(1);
	copyTemplate (result);
	
	// now, we travers the given tpl and try to match it onto the result-tpl
	bool error = false;
	int cnt = 0;
	GuidoPos pos = tpl.GetHeadPosition();
	while (pos)
	{
		TagParameter * tp = tpl.GetNext(pos);
		if (tp)
		{
			// now, we have two cases:
			if (tp->name.length() > 0)			// 1. the tp is named
			{
				if (!Match(result, tp))
				{
					error = true;	// then, we have an error ...
					// Removed, because it complains for nothing if there is several formats of tag parameters list. (i.e: slurs)
					// GuidoWarn("Did not find matching name for tag paramater:", tp->name );
					break;
				}
			}
			else								// 2. the tp is not named
			{
				// not named -> then the position (cnt) is important ...
				// find the position in the resulting TagParameterList  ...
				TagParameter *tmptp = getPosition(result, cnt); 
				if (tmptp == NULL)
				{
					error = true;		// then, the count does not exists in resulting list  ...
					GuidoWarn("Tag parameter position error: ", tp->name.c_str() );
					break;
				}
				
				// now, we have the tmptp at the position ...
				if (tmptp->pflag == TagParameter::SETBYNAME)
				{
					// then, we just ignore whatever is set here (name overrides position)
					GuidoWarn("Tag parameter already set by name: ", tp->name.c_str() );
				}
				else if (tmptp->pflag == TagParameter::SETBYPOSITION)
				{
					assert(false);			// this cannot be ... Position can only be set once really
				}
				else
				{
					// ok, the tag is not set already ... then, we try to copy the value
					if (!tmptp->copyValue(tp))
					{
						error = true;			// copy fails if the type is not correct.
						GuidoWarn("Named Parameter has wrong type: ", tp->name.c_str() );
						break;
					}
					tmptp->pflag = TagParameter::SETBYPOSITION;
				}
			} // else notnamed 
		} // if tp
		++cnt;
	}
	
	// now we need to check, if all required parameters have been set.
	if (error || !checkRequired (result))
	{
		delete result;
		result = NULL;
	}	
	return result;
}
