/*

 Copyright (C) 2011 Grame

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
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
 research@grame.fr

 */

#include "jsonhelper.h"
#include "json.h"
#include <string.h>
#include <sstream>

namespace guidohttpd
{
void GUIDOjson_print_date (json_printer *printer, const char* key, int num, int denom)
{
    const char* curstate;
    stringstream buffer;
    json_print_pretty(printer, JSON_KEY, key, 1);
    json_print_pretty(printer, JSON_OBJECT_BEGIN, NULL, 0);
    json_print_pretty(printer, JSON_KEY, "num", 1);
    buffer.str("");
    buffer << num;
    curstate = buffer.str().c_str();
    json_print_pretty(printer, JSON_INT, curstate, strlen(curstate));
    json_print_pretty(printer, JSON_KEY, "denom", 1);
    buffer.str("");
    buffer << denom;
    curstate = buffer.str().c_str();
    json_print_pretty(printer, JSON_INT, curstate, strlen(curstate));
    json_print_pretty(printer, JSON_OBJECT_END, NULL, 0);
}

void GUIDOjson_print_float_rect (json_printer *printer, const char* key, float left, float right, float top, float bottom)
{
    const char* curstate;
    stringstream buffer;
    json_print_pretty(printer, JSON_KEY, key, 1);
    json_print_pretty(printer, JSON_OBJECT_BEGIN, NULL, 0);
    json_print_pretty(printer, JSON_KEY, "left", 1);
    buffer.str("");
    buffer << left;
    curstate = buffer.str().c_str();
    json_print_pretty(printer, JSON_FLOAT, curstate, strlen(curstate));
    json_print_pretty(printer, JSON_KEY, "right", 1);
    buffer.str("");
    buffer << right;
    curstate = buffer.str().c_str();
    json_print_pretty(printer, JSON_FLOAT, curstate, strlen(curstate));
    json_print_pretty(printer, JSON_KEY, "top", 1);
    buffer.str("");
    buffer << top;
    curstate = buffer.str().c_str();
    json_print_pretty(printer, JSON_FLOAT, curstate, strlen(curstate));
    json_print_pretty(printer, JSON_KEY, "bottom", 1);
    buffer.str("");
    buffer << bottom;
    curstate = buffer.str().c_str();
    json_print_pretty(printer, JSON_FLOAT, curstate, strlen(curstate));
    json_print_pretty(printer, JSON_OBJECT_END, NULL, 0);
}
}