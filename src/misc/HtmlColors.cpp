
/*
  GUIDO Library
  Copyright (C) 2012	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <ctype.h>

#include "HtmlColors.h"

using namespace std;

map<string, long>	HtmlColor::fColorsMap;

static HtmlColor gHtmlColors;		// to call the init method only
HtmlColor::HtmlColor ()		{ init(); }

bool HtmlColor::get (const char* name, unsigned char* c) 
{
	string color;
	while (*name) color += tolower(*name++);
	map<string, long>::const_iterator i = fColorsMap.find(color);
	if (i == fColorsMap.end()) return false;
	long val = i->second;
	c[2] = (unsigned char)val & 0xff;
	c[1] = (unsigned char)((val & 0xff00) >> 8);
	c[0] = (unsigned char)((val & 0xff0000) >> 16);
	c[3] = 255;
	return true;
}

void HtmlColor::init() 
{
	if (fColorsMap.size()) return;

	fColorsMap["aliceblue"] = 0xf0f8ff;
	fColorsMap["antiquewhite"] = 0xfaebd7;
	fColorsMap["aqua"] = 0x00ffff;
	fColorsMap["aquamarine"] = 0x7fffd4;
	fColorsMap["azure"] = 0xf0ffff;
	fColorsMap["beige"] = 0xf5f5dc;
	fColorsMap["bisque"] = 0xffe4c4;
	fColorsMap["black"] = 0x000000;
	fColorsMap["blanchedalmond"] = 0xffebcd;
	fColorsMap["blue"] = 0x0000ff;
	fColorsMap["blueviolet"] = 0x8a2be2;
	fColorsMap["brown"] = 0xa52a2a;
	fColorsMap["burlywood"] = 0xdeb887;
	fColorsMap["cadetblue"] = 0x5f9ea0;
	fColorsMap["chartreuse"] = 0x7fff00;
	fColorsMap["chocolate"] = 0xd2691e;
	fColorsMap["coral"] = 0xff7f50;
	fColorsMap["cornflowerblue"] = 0x6495ed;
	fColorsMap["cornsilk"] = 0xfff8dc;
	fColorsMap["crimson"] = 0xdc143c;
	fColorsMap["cyan"] = 0x00ffff;
	fColorsMap["darkblue"] = 0x00008b;
	fColorsMap["darkcyan"] = 0x008b8b;
	fColorsMap["darkgoldenrod"] = 0xb8860b;
	fColorsMap["darkgray"] = 0xa9a9a9;
	fColorsMap["darkgrey"] = 0xa9a9a9;
	fColorsMap["darkgreen"] = 0x006400;
	fColorsMap["darkkhaki"] = 0xbdb76b;
	fColorsMap["darkmagenta"] = 0x8b008b;
	fColorsMap["darkolivegreen"] = 0x556b2f;
	fColorsMap["darkorange"] = 0xff8c00;
	fColorsMap["darkorchid"] = 0x9932cc;
	fColorsMap["darkred"] = 0x8b0000;
	fColorsMap["darksalmon"] = 0xe9967a;
	fColorsMap["darkseagreen"] = 0x8fbc8f;
	fColorsMap["darkslateblue"] = 0x483d8b;
	fColorsMap["darkslategray"] = 0x2f4f4f;
	fColorsMap["darkslategrey"] = 0x2f4f4f;
	fColorsMap["darkturquoise"] = 0x00ced1;
	fColorsMap["darkviolet"] = 0x9400d3;
	fColorsMap["deeppink"] = 0xff1493;
	fColorsMap["deepskyblue"] = 0x00bfff;
	fColorsMap["dimgray"] = 0x696969;
	fColorsMap["dimgrey"] = 0x696969;
	fColorsMap["dodgerblue"] = 0x1e90ff;
	fColorsMap["firebrick"] = 0xb22222;
	fColorsMap["floralwhite"] = 0xfffaf0;
	fColorsMap["forestgreen"] = 0x228b22;
	fColorsMap["fuchsia"] = 0xff00ff;
	fColorsMap["gainsboro"] = 0xdcdcdc;
	fColorsMap["ghostwhite"] = 0xf8f8ff;
	fColorsMap["gold"] = 0xffd700;
	fColorsMap["goldenrod"] = 0xdaa520;
	fColorsMap["gray"] = 0x808080;
	fColorsMap["grey"] = 0x808080;
	fColorsMap["green"] = 0x008000;
	fColorsMap["greenyellow"] = 0xadff2f;
	fColorsMap["honeydew"] = 0xf0fff0;
	fColorsMap["hotpink"] = 0xff69b4;
	fColorsMap["indianred"] = 0xcd5c5c;
	fColorsMap["indigo"] = 0x4b0082;
	fColorsMap["ivory"] = 0xfffff0;
	fColorsMap["khaki"] = 0xf0e68c;
	fColorsMap["lavender"] = 0xe6e6fa;
	fColorsMap["lavenderblush"] = 0xfff0f5;
	fColorsMap["lawngreen"] = 0x7cfc00;
	fColorsMap["lemonchiffon"] = 0xfffacd;
	fColorsMap["lightblue"] = 0xadd8e6;
	fColorsMap["lightcoral"] = 0xf08080;
	fColorsMap["lightcyan"] = 0xe0ffff;
	fColorsMap["lightgoldenrodyellow"] = 0xfafad2;
	fColorsMap["lightgray"] = 0xd3d3d3;
	fColorsMap["lightgrey"] = 0xd3d3d3;
	fColorsMap["lightgreen"] = 0x90ee90;
	fColorsMap["lightpink"] = 0xffb6c1;
	fColorsMap["lightsalmon"] = 0xffa07a;
	fColorsMap["lightseagreen"] = 0x20b2aa;
	fColorsMap["lightskyblue"] = 0x87cefa;
	fColorsMap["lightslategray"] = 0x778899;
	fColorsMap["lightslategrey"] = 0x778899;
	fColorsMap["lightsteelblue"] = 0xb0c4de;
	fColorsMap["lightyellow"] = 0xffffe0;
	fColorsMap["lime"] = 0x00ff00;
	fColorsMap["limegreen"] = 0x32cd32;
	fColorsMap["linen"] = 0xfaf0e6;
	fColorsMap["magenta"] = 0xff00ff;
	fColorsMap["maroon"] = 0x800000;
	fColorsMap["mediumaquamarine"] = 0x66cdaa;
	fColorsMap["mediumblue"] = 0x0000cd;
	fColorsMap["mediumorchid"] = 0xba55d3;
	fColorsMap["mediumpurple"] = 0x9370d8;
	fColorsMap["mediumseagreen"] = 0x3cb371;
	fColorsMap["mediumslateblue"] = 0x7b68ee;
	fColorsMap["mediumspringgreen"] = 0x00fa9a;
	fColorsMap["mediumturquoise"] = 0x48d1cc;
	fColorsMap["mediumvioletred"] = 0xc71585;
	fColorsMap["midnightblue"] = 0x191970;
	fColorsMap["mintcream"] = 0xf5fffa;
	fColorsMap["mistyrose"] = 0xffe4e1;
	fColorsMap["moccasin"] = 0xffe4b5;
	fColorsMap["navajowhite"] = 0xffdead;
	fColorsMap["navy"] = 0x000080;
	fColorsMap["oldlace"] = 0xfdf5e6;
	fColorsMap["olive"] = 0x808000;
	fColorsMap["olivedrab"] = 0x6b8e23;
	fColorsMap["orange"] = 0xffa500;
	fColorsMap["orangered"] = 0xff4500;
	fColorsMap["orchid"] = 0xda70d6;
	fColorsMap["palegoldenrod"] = 0xeee8aa;
	fColorsMap["palegreen"] = 0x98fb98;
	fColorsMap["paleturquoise"] = 0xafeeee;
	fColorsMap["palevioletred"] = 0xd87093;
	fColorsMap["papayawhip"] = 0xffefd5;
	fColorsMap["peachpuff"] = 0xffdab9;
	fColorsMap["peru"] = 0xcd853f;
	fColorsMap["pink"] = 0xffc0cb;
	fColorsMap["plum"] = 0xdda0dd;
	fColorsMap["powderblue"] = 0xb0e0e6;
	fColorsMap["purple"] = 0x800080;
	fColorsMap["red"] = 0xff0000;
	fColorsMap["rosybrown"] = 0xbc8f8f;
	fColorsMap["royalblue"] = 0x4169e1;
	fColorsMap["saddlebrown"] = 0x8b4513;
	fColorsMap["salmon"] = 0xfa8072;
	fColorsMap["sandybrown"] = 0xf4a460;
	fColorsMap["seagreen"] = 0x2e8b57;
	fColorsMap["seashell"] = 0xfff5ee;
	fColorsMap["sienna"] = 0xa0522d;
	fColorsMap["silver"] = 0xc0c0c0;
	fColorsMap["skyblue"] = 0x87ceeb;
	fColorsMap["slateblue"] = 0x6a5acd;
	fColorsMap["slategray"] = 0x708090;
	fColorsMap["slategrey"] = 0x708090;
	fColorsMap["snow"] = 0xfffafa;
	fColorsMap["springgreen"] = 0x00ff7f;
	fColorsMap["steelblue"] = 0x4682b4;
	fColorsMap["tan"] = 0xd2b48c;
	fColorsMap["teal"] = 0x008080;
	fColorsMap["thistle"] = 0xd8bfd8;
	fColorsMap["tomato"] = 0xff6347;
	fColorsMap["turquoise"] = 0x40e0d0;
	fColorsMap["violet"] = 0xee82ee;
	fColorsMap["wheat"] = 0xf5deb3;
	fColorsMap["white"] = 0xffffff;
	fColorsMap["whitesmoke"] = 0xf5f5f5;
	fColorsMap["yellow"] = 0xffff00;
	fColorsMap["yellowgreen"] = 0x9acd32;
}
