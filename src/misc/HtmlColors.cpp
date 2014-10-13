
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
#include "Colors.h"

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

    get(val, c);

    return true;
}

bool HtmlColor::get (long color, unsigned char* c) 
{
	c[2] = (unsigned char)color & 0xff;
	c[1] = (unsigned char)((color & 0xff00) >> 8);
	c[0] = (unsigned char)((color & 0xff0000) >> 16);
	c[3] = 255;

	return true;
}

void HtmlColor::init() 
{
	if (fColorsMap.size()) return;

	fColorsMap["aliceblue"] = kaliceblue;
	fColorsMap["antiquewhite"] = kantiquewhite;
    fColorsMap["aqua"] = kaqua;
	fColorsMap["aquamarine"] = kaquamarine;
	fColorsMap["azure"] = kazure;
	fColorsMap["beige"] = kbeige;
	fColorsMap["bisque"] = kbisque;
	fColorsMap["black"] = kblack;
	fColorsMap["blanchedalmond"] = kblanchedalmond;
	fColorsMap["blue"] = kblue;
	fColorsMap["blueviolet"] = kblueviolet;
	fColorsMap["brown"] = kbrown;
	fColorsMap["burlywood"] = kburlywood;
	fColorsMap["cadetblue"] = kcadetblue;
	fColorsMap["chartreuse"] = kchartreuse;
	fColorsMap["chocolate"] = kchocolate;
	fColorsMap["coral"] = kcoral;
	fColorsMap["cornflowerblue"] = kcornflowerblue;
	fColorsMap["cornsilk"] = kcornsilk;
	fColorsMap["crimson"] = kcrimson;
	fColorsMap["cyan"] = kcyan;
	fColorsMap["darkblue"] = kdarkblue;
	fColorsMap["darkcyan"] = kdarkcyan;
	fColorsMap["darkgoldenrod"] = kdarkgoldenrod;
	fColorsMap["darkgray"] = kdarkgray;
	fColorsMap["darkgrey"] = kdarkgrey;
	fColorsMap["darkgreen"] = kdarkgreen;
	fColorsMap["darkkhaki"] = kdarkkhaki;
	fColorsMap["darkmagenta"] = kdarkmagenta;
	fColorsMap["darkolivegreen"] = kdarkolivegreen;
	fColorsMap["darkorange"] = kdarkorange;
	fColorsMap["darkorchid"] = kdarkorchid;
	fColorsMap["darkred"] = kdarkred;
	fColorsMap["darksalmon"] = kdarksalmon;
	fColorsMap["darkseagreen"] = kdarkseagreen;
	fColorsMap["darkslateblue"] = kdarkslateblue;
	fColorsMap["darkslategray"] = kdarkslategray;
	fColorsMap["darkslategrey"] = kdarkslategrey;
	fColorsMap["darkturquoise"] = kdarkturquoise;
	fColorsMap["darkviolet"] = kdarkviolet;
	fColorsMap["deeppink"] = kdeeppink;
	fColorsMap["deepskyblue"] = kdeepskyblue;
	fColorsMap["dimgray"] = kdimgray;
	fColorsMap["dimgrey"] = kdimgrey;
	fColorsMap["dodgerblue"] = kdodgerblue;
	fColorsMap["firebrick"] = kfirebrick;
	fColorsMap["floralwhite"] = kfloralwhite;
	fColorsMap["forestgreen"] = kforestgreen;
	fColorsMap["fuchsia"] = kfuchsia;
	fColorsMap["gainsboro"] = kgainsboro;
	fColorsMap["ghostwhite"] = kghostwhite;
	fColorsMap["gold"] = kgold;
	fColorsMap["goldenrod"] = kgoldenrod;
	fColorsMap["gray"] = kgray;
	fColorsMap["grey"] = kgrey;
	fColorsMap["green"]                = kgreen;
	fColorsMap["greenyellow"]          = kgreenyellow;
	fColorsMap["honeydew"]             = khoneydew;
	fColorsMap["hotpink"]              = khotpink;
	fColorsMap["indianred"]            = kindianred;
	fColorsMap["indigo"]               = kindigo;
	fColorsMap["ivory"]                = kivory;
	fColorsMap["khaki"]                = kkhaki;
	fColorsMap["lavender"]             = klavender;
	fColorsMap["lavenderblush"]        = klavenderblush;
	fColorsMap["lawngreen"]            = klawngreen;
	fColorsMap["lemonchiffon"]         = klemonchiffon;
	fColorsMap["lightblue"]            = klightblue;
	fColorsMap["lightcoral"]           = klightcoral;
	fColorsMap["lightcyan"]            = klightcyan;
	fColorsMap["lightgoldenrodyellow"] = klightgoldenrodyellow;
	fColorsMap["lightgray"]            = klightgray;
	fColorsMap["lightgrey"]            = klightgrey;
	fColorsMap["lightgreen"]           = klightgreen;
	fColorsMap["lightpink"] = klightpink;
	fColorsMap["lightsalmon"] = klightsalmon;
	fColorsMap["lightseagreen"] = klightseagreen;
	fColorsMap["lightskyblue"] = klightskyblue;
	fColorsMap["lightslategray"] = klightslategray;
	fColorsMap["lightslategrey"] = klightslategrey;
	fColorsMap["lightsteelblue"] = klightsteelblue;
	fColorsMap["lightyellow"] = klightyellow;
	fColorsMap["lime"] = klime;
	fColorsMap["limegreen"] = klimegreen;
	fColorsMap["linen"] = klinen;
	fColorsMap["magenta"] = kmagenta;
	fColorsMap["maroon"] = kmaroon;
	fColorsMap["mediumaquamarine"] = kmediumaquamarine;
	fColorsMap["mediumblue"] = kmediumblue;
	fColorsMap["mediumorchid"] = kmediumorchid;
	fColorsMap["mediumpurple"] = kmediumpurple;
	fColorsMap["mediumseagreen"] = kmediumseagreen;
	fColorsMap["mediumslateblue"] = kmediumslateblue;
	fColorsMap["mediumspringgreen"] = kmediumspringgreen;
	fColorsMap["mediumturquoise"] = kmediumturquoise;
	fColorsMap["mediumvioletred"] = kmediumvioletred;
	fColorsMap["midnightblue"] = kmidnightblue;
	fColorsMap["mintcream"] = kmintcream;
	fColorsMap["mistyrose"] = kmistyrose;
	fColorsMap["moccasin"] = kmoccasin;
	fColorsMap["navajowhite"] = knavajowhite;
	fColorsMap["navy"] = knavy;
	fColorsMap["oldlace"] = koldlace;
	fColorsMap["olive"] = kolive;
	fColorsMap["olivedrab"] = kolivedrab;
	fColorsMap["orange"] = korange;
	fColorsMap["orangered"] = korangered;
	fColorsMap["orchid"] = korchid;
	fColorsMap["palegoldenrod"] = kpalegoldenrod;
	fColorsMap["palegreen"] = kpalegreen;
	fColorsMap["paleturquoise"] = kpaleturquoise;
	fColorsMap["palevioletred"] = kpalevioletred;
	fColorsMap["papayawhip"] = kpapayawhip;
	fColorsMap["peachpuff"] = kpeachpuff;
	fColorsMap["peru"] = kperu;
	fColorsMap["pink"] = kpink;
	fColorsMap["plum"] = kplum;
	fColorsMap["powderblue"] = kpowderblue;
	fColorsMap["purple"] = kpurple;
	fColorsMap["red"] = kred;
	fColorsMap["rosybrown"] = krosybrown;
	fColorsMap["royalblue"] = kroyalblue;
	fColorsMap["saddlebrown"] = ksaddlebrown;
	fColorsMap["salmon"] = ksalmon;
	fColorsMap["sandybrown"] = ksandybrown;
	fColorsMap["seagreen"] = kseagreen;
	fColorsMap["seashell"] = kseashell;
	fColorsMap["sienna"] = ksienna;
	fColorsMap["silver"] = ksilver;
	fColorsMap["skyblue"] = kskyblue;
	fColorsMap["slateblue"] = kslateblue;
	fColorsMap["slategray"] = kslategray;
	fColorsMap["slategrey"] = kslategrey;
	fColorsMap["snow"] = ksnow;
	fColorsMap["springgreen"] = kspringgreen;
	fColorsMap["steelblue"] = ksteelblue;
	fColorsMap["tan"] = ktan;
	fColorsMap["teal"] = kteal;
	fColorsMap["thistle"] = kthistle;
	fColorsMap["tomato"] = ktomato;
	fColorsMap["turquoise"] = kturquoise;
	fColorsMap["violet"] = kviolet;
	fColorsMap["wheat"] = kwheat;
	fColorsMap["white"] = kwhite;
	fColorsMap["whitesmoke"] = kwhitesmoke;
	fColorsMap["yellow"] = kyellow;
	fColorsMap["yellowgreen"] = kyellowgreen;
}
