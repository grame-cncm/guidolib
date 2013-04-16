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
#include "parseutils.h"
#include "string.h"

//general parsing support:
int isCharInString(char c, char * s)
{
	int pos=0;
	while(s[pos]!=0){
		if(s[pos]==c){return 1;}
		pos++;
	}
	return 0;
}

int isBlanks(char* c, int count){
	for(int i=0;i<count;i++){
		if(c[i]!=' '){
			return 0;
		}
	}
	return 1;
}

//parsing support for note-records
int parsePitchInformation(char *s, EPitch *p, int *c, int *o)
{
	//1.: Parse Pitch-code
	if(isCharInString(s[0],"ABCDEFGr")){
		*p=EPitch(s[0]);
		if(s[0]=='r'){
			if(strncmp(s,"rest ",5)!=0){
				return -1;
			}else{
				return 0;
			}
		}
	} else {
		return -1;
	}

	//2.: Parse Chromatics
	*c=0;
	int cl=0;
	if(s[1]=='#'){
		if(s[2]=='#'){
			*c=2;cl=2;
		} else {
			*c=1;cl=1;
		}
	} else {
		if(s[1]=='f'){
			if(s[2]=='f'){
				*c=-2;cl=2;
			} else {
				*c=-1;cl=1;
			}
		}
	}
	
	//3.: Parse Octave
	if(isCharInString(s[1+cl],"0123456789")){
		*o=(s[1+cl]-'0');
	} else {
		return -1;
	}

	//4. check for following blanks
	for(int i=cl;i<2;i++){
		if(s[2+i]!=' '){return -1;}
	}
	return 0;
}

int parseDurationInformation(char * s, int*d){
	int pos=0;
	//search beginning of number
	while(pos<3){
		if(s[pos]!=' '){break;}
		pos++;
	}
	if(pos==3){return -1;}
	//now parse the number
	*d=0;
	for(int i=pos;i<3;i++){
		*d=*d*10;
		if(!isCharInString(s[i],"0123456789")){return -1;}
		*d=*d+s[i]-'0';
	}
	return 0;
}


//parsing support for measure-records
int parseBarNumber(char *s, int *bn){
	int pos=0;
	*bn=0;
	while(pos<4){
		if(isCharInString(s[pos],"0123456789")){
			*bn=*bn*10;
			*bn=*bn+s[pos]-'0';
		} else {
			if(s[pos]==' '){
				break;
			} else {
				return -1;
			}
		}
		pos++;
	}
	if(pos==0){*bn=-1;}//no bar number!!
	//check for blanks
	while(pos<4){
		if(s[pos]!=' '){
			return -1;
		}
		pos++;
	}
	return 0;
}

