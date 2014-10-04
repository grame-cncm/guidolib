#ifndef DATE_TOOLS_H
#define DATE_TOOLS_H

#include "GUIDOEngine.h"

void stringToDate(std::string str, GuidoDate &date);
std::string dateToString(GuidoDate &date);
float dateToFloat(const GuidoDate &date);
#endif