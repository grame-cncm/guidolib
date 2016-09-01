#ifndef DATE_TOOLS_H
#define DATE_TOOLS_H

#include "GUIDOEngine.h"

using namespace std;

void stringToDate(string str, GuidoDate &date);
string dateToString(const GuidoDate &date);
float dateToFloat(const GuidoDate &date);
#endif
