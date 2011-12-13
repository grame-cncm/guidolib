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
#include <ctype.h>

#include "ARKey.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"

using namespace std;

int gd_noteName2pc(const char *name);


ListOfTPLs ARKey::ltpls(1);

ARKey::ARKey(const TYPE_TIMEPOSITION & timeposition)
	: ARMTParameter(timeposition), mIsFree(false)
{
	keynumber = 0;
	memset(octarray,0,NUMNOTES*(sizeof(int))); // octarray auf 0 setzen;
}


ARKey::ARKey(int p_keynumber) : mIsFree(false)
{
	keynumber = p_keynumber;
	memset(octarray,0,NUMNOTES*(sizeof(int))); // octarray auf 0 setzen;
}

ARKey::ARKey() : mIsFree(false)
{
	keynumber = 0; // no accidentals
	memset(octarray,0,NUMNOTES*(sizeof(int))); // octarray auf 0 setzen;
}

ARKey::ARKey(const ARKey & key)
{
	mIsFree = key.mIsFree;
	keynumber = key.keynumber;

	key.getOctArray((int *) &octarray);
	key.getFreeKeyArray((int *) &mkarray);

}

bool ARKey::operator ==(const ARKey & k) const
{
	if (mIsFree != k.mIsFree)		return false;
	if (keynumber != k.keynumber)	return false;
	for (int i =0;i<NUMNOTES;i++)
	{
		if (mkarray[i] != k.mkarray[i])			return false;
		if (octarray[i] != k.octarray[i])		return false;
	}

	// they are equal!
	return true;
	
}
void ARKey::print() const
{
}

void ARKey::PrintName(std::ostream & os) const
{
	os << "\\key";
}
void ARKey::PrintParameters(std::ostream & os) const
{
	os << "<" << keynumber << "> ";
}

void ARKey::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl

		// either a key-string ("G") or a key-number key=3
		lstrs.AddTail(( "S,key,,r"));
		lstrs.AddTail(( "I,key,,r"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if( ret >= 0 && rtpl )
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for the first ParameterList
			// w, h, ml, mt, mr, mb
			GuidoPos pos = rtpl->GetHeadPosition();

			TagParameterString * tps = TagParameterString::cast(rtpl->GetNext(pos));
			assert(tps);

			NVstring name = tps->getValue();

			// ist free-Tag gesetzt?
			if (name.substr(0, 5) == "free=" ) {
				mIsFree = true;
				getKeyArray(name.substr(5, name.length()-5));
				newgetKeyArray (name.substr(5, name.length()-5));
			}
			else {
				mIsFree = false;
				int t = (int)name[0];
				int major = (t == toupper(t));

				t = toupper(t);				
				switch (t) 
				{
					case 'F': 	keynumber = -1;	break;
					case 'C': 	keynumber = 0;	break;
					case 'G': 	keynumber = 1;	break;
					case 'D': 	keynumber = 2;	break;
					case 'A': 	keynumber = 3;	break;
					case 'E': 	keynumber = 4;	break;
					case 'H':
					case 'B':	keynumber = 5;	break;
					default:	keynumber = 0;
				}
				
				if (!major)				keynumber -= 3;		// minus 3 accidentals  (A-Major ->  a-minor ...)				
				if (name.length() > 1)
				{
					t = name[1];
					if (t == '#')		keynumber += 7;
					else if (t == '&')	keynumber -= 7;
				}
			}
		}
		else if (ret == 1)
		{
			// then, we now the match for the first ParameterList
			// w, h, ml, mt, mr, mb
			GuidoPos pos = rtpl->GetHeadPosition();
			TagParameterInt * tpi = TagParameterInt::cast(rtpl->GetNext(pos));
			assert(tpi);
			keynumber = tpi->getValue();
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}


float ARKey::getAccidental (const char*& ptr)
{
	float accidental = 0.f;
	if (*ptr == '[') {
cout << "ARKey::getAccidental float" << endl;
		ptr++;
		string acc;
		while (*ptr && (*ptr != ']')) acc += *ptr++;
		if (*ptr == ']')  accidental = atof (acc.c_str());
	}
	else while (*ptr) {
cout << "ARKey::getAccidental standard" << endl;
		if (*ptr == '#') accidental += 1.f;
		else if (*ptr == '&') accidental -= 1.f;
		else break;
		ptr++;			
	}
	return accidental;
}

int ARKey::getNote (const char*& ptr)
{
	string notename;
	while (isalpha(*ptr)) notename += *ptr++;
cout << "ARKey::getNote " << notename << endl;
	return gd_noteName2pc(notename.c_str());
}

void ARKey::newgetKeyArray(const std::string& inString)
{
cout << "ARKey::newgetKeyArray: " << inString << endl;
	const char* ptr = inString.c_str();
	while (*ptr) {
		int note = getNote (ptr);
		float accidental = getAccidental(ptr);
		cout << "note " << note << " acc: " << accidental << endl;
		if (accidental == 0.f) break;
	}
}

void ARKey::getKeyArray(std::string inString) 
{
	size_t i,j,index;
	int acc = 0;	// can be < 0
	string notename, accidental;

	memset(mkarray, 0, NUMNOTES * sizeof(int)); // mkarray auf 0 setzen.

	while (inString.length()) 
	{
		index = 0;
		for (i = 0; i < inString.length() && isalpha(inString[i]); i++) { }
		notename = inString.substr(0,i);

		for (j=i; j< inString.length() && (inString[j]=='#' || inString[j]=='&'); j++) { }		
		accidental = inString.substr(i,j-i);		// get type of accidental

		acc = (int)accidental.length();
		if (acc > 2) return; // Error !
		if (acc == 2 && (accidental[0] != accidental[1]) )
			return; // accidentals like &# or #& make no sense
		acc *= ((accidental[0] == '#') ? 1 : -1 );

		// if correct notename, store accidental  at correct position in  mkarray
		switch(gd_noteName2pc(notename.c_str())) 
		{
			case NOTE_C: index=0; mkarray[0]=acc; break;
			case NOTE_CIS: break; // do nothing (yet)
			case NOTE_D: index=1; mkarray[1]=acc; break;
			case NOTE_DIS: break; // do nothing (yet)
			case NOTE_E: index=2; mkarray[2]=acc; break;
			case NOTE_F: index=3; mkarray[3]=acc; break;
			case NOTE_FIS: break; // do nothing (yet)
			case NOTE_G: index=4; mkarray[4]=acc; break;
			case NOTE_GIS: break; // do nothing (yet)
			case NOTE_A: index=5; mkarray[5]=acc; break;
			case NOTE_AIS: break; // do nothing (yet)
			case NOTE_H: index=6; mkarray[6]=acc; break;
			default: return; // if neede, do error handling here
		}


		// In which octave should accidental be put
		if (j < inString.length()) 
		{
			if(inString[j] == '+')
			{
				++j;
				if( j < inString.length())
				switch (inString[j]) 
				{
					case '0': octarray[index]=0; ++j; break;
					case '1': octarray[index]=1; ++j; break;
					case '2': octarray[index]=2; ++j; break;
					default: return; // here additional octaves or errors could be handled
				}
			}
			else if(inString[j]=='-')
			{
				++j;
				if( j < inString.length())
				switch (inString[j]) 
				{
					case '0': octarray[index]= 0; ++j; break;
					case '1': octarray[index]=-1; ++j; break;
					case '2': octarray[index]=-2; ++j; break;
					default: return; // here additional octaves or errors could be handled
				}
			}
			else if (isdigit(inString[j]))
			{
				switch (inString[j]) 
				{
					case '0': octarray[index]=0; ++j; break;
					case '1': octarray[index]=1; ++j; break;
					case '2': octarray[index]=2; ++j; break;
					default: return; // here additional octaves or errors could be handled
				}
			}
		}
		inString = inString.substr( j, inString.length()-j);
	}
}

void ARKey::getFreeKeyArray(int * keyArray) const
{
	for (int i=0;i<NUMNOTES;i++) keyArray[i]=mkarray[i];
}

void ARKey::getOctArray(int * OctArray) const
{
	for (int i=0;i<NUMNOTES;i++) OctArray[i]=octarray[i];
}


bool ARKey::IsStateTag() const
{
	return true;
}
