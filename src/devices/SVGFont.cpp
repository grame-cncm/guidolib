/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "SVGFont.h"

#ifdef INDEPENDENTSVG
#include "nanosvg.h"
#include <map>
#elif __APPLE__
#include "GFontOSX.h"
#include "GSystemOSX.h"
GSystemOSX gSystem (0, 0);

#elif defined (WIN32)
#include "GFontWin32.h"
#include "GSystemWin32.h"
GSystemWin32 gSystem (0, 0);

#elif linux
#include "CairoSystem.h"
CairoSystem gSystem (0);

#else
#error "unknown system for compiling SVGFont"

#endif



//______________________________________________________________________________
SVGFont::SVGFont(const char * name, int size, int properties, const char * guidofontfile, const char * guidofontspec) :
	fGuidoFontFile(guidofontfile), fGuidoFontSpec(guidofontspec), fSize(size), fName(name), fProperties(properties)
{
#ifdef INDEPENDENTSVG
	fNSVGimage = 0;
	if (!fGuidoFontFile.empty())
	  fNSVGimage = nsvgParseFromFile(fGuidoFontFile.c_str(), "px", size);
	else if (!fGuidoFontSpec.empty())
	  fNSVGimage = nsvgParse(const_cast<char *>(fGuidoFontSpec.c_str()), "px", size);
	else
	  fNSVGimage = NULL;
#else
	fDevice = gSystem.CreateMemoryDevice (10,10);
	fFont = gSystem.CreateVGFont (name, size, properties);
#endif
}

SVGFont::~SVGFont() {
#ifndef INDEPENDENTSVG
delete fFont; delete fDevice;
#endif
}

//______________________________________________________________________________
void SVGFont::GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const
{
#ifdef INDEPENDENTSVG
	*outWidth = 0;
	*outHeight = 0;
	if (fNSVGimage) {
          struct NSVGfont* font;
	  struct NSVGshape* shape;
	  for (font = fNSVGimage->fonts; font != NULL; font = font->next) {
	    if (strcmp(fName.c_str(), font->fontFamily) == 0) {
	      for (int i = 0; i < inCharCount; i++) {
	        for (shape = font->shapes; shape != NULL; shape = shape->next) {
	          std::map<std::string, std::string>::const_iterator it;
	          it = hexToCharMap.find(std::string(shape->unicode));
	          std::string ucodeTransform = it == hexToCharMap.end() ? shape->unicode : it->second;
		  if ((std::string(1, s[i]) == std::string(shape->unicode)) || ucodeTransform == std::string(1, s[i])) {
		    // use horizontal space unless it is the last character
		    float x_len = i == inCharCount - 1 ? shape->bounds[2] - shape->bounds[0] : shape->horizAdvX;
		    float x = x_len * fSize / font->unitsPerEm;
		    float y = (shape->bounds[3] - shape->bounds[1]) * fSize / font->unitsPerEm;;
	            *outWidth += x;
	            if (y > *outHeight)
	            	*outHeight = y;
		  }
		}
	      }
	    }
	  }
	}
#else
	fFont->GetExtent (s, inCharCount, outWidth, outHeight, fDevice);
#endif
}

//______________________________________________________________________________
void SVGFont::GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * context ) const
{
#ifdef INDEPENDENTSVG
	*outWidth = 0;
	*outHeight = 0;
        char cs[2] = {0};
        cs[0] = c;
        cs[1] = '\0';
	if (fNSVGimage) {
          struct NSVGfont* font;
	  struct NSVGshape* shape;
	  for (font = fNSVGimage->fonts; font != NULL; font = font->next) {
	    if (strcmp(fName.c_str(), font->fontFamily) == 0) {
              for (shape = font->shapes; shape != NULL; shape = shape->next) {
		std::map<std::string, std::string>::const_iterator it;
		it = hexToCharMap.find(std::string(shape->unicode));
		std::string ucodeTransform = (it == hexToCharMap.end()) ? shape->unicode : it->second;
                if ((strcmp(cs, shape->unicode) == 0) || ucodeTransform == std::string(cs)) {	
                  *outWidth = (shape->bounds[2] - shape->bounds[0]) * fSize / font->unitsPerEm;
                  *outHeight = (shape->bounds[3] - shape->bounds[1]) * fSize / font->unitsPerEm;
                }
              }
	    }
	  }
	}
#else
	fFont->GetExtent( c, outWidth, outHeight, fDevice );
#endif
}

//______________________________________________________________________________
#ifdef INDEPENDENTSVG
std::map<std::string, std::string> SVGFont::makeHexToCharMap()
{
	std::map<std::string, std::string> fontmap;
	fontmap["&#x09;"] = " ";
	fontmap["&#x10;"] = " ";
	fontmap["&#x20;"] = " ";
	fontmap["&#x21;"] = "!";
	fontmap["&#x22;"] = "\"";
	fontmap["&#x23;"] = "#";
	fontmap["&#x24;"] = "$";
	fontmap["&#x25;"] = "%";
	fontmap["&#x26;"] = "&";
	fontmap["&#x27;"] = "'";
	fontmap["&#x28;"] = "(";
	fontmap["&#x29;"] = ")";
	fontmap["&#x2A;"] = "*";
	fontmap["&#x2B;"] = "+";
	fontmap["&#x2C;"] = ",";
	fontmap["&#x2D;"] = "-";
	fontmap["&#x2E;"] = ".";
	fontmap["&#x2F;"] = "/";
	fontmap["&#x30;"] = "0";
	fontmap["&#x31;"] = "1";
	fontmap["&#x32;"] = "2";
	fontmap["&#x33;"] = "3";
	fontmap["&#x34;"] = "4";
	fontmap["&#x35;"] = "5";
	fontmap["&#x36;"] = "6";
	fontmap["&#x37;"] = "7";
	fontmap["&#x38;"] = "8";
	fontmap["&#x39;"] = "9";
	fontmap["&#x3A;"] = ":";
	fontmap["&#x3B;"] = ";";
	fontmap["&#x3C;"] = "<";
	fontmap["&#x3D;"] = "=";
	fontmap["&#x3E;"] = ">";
	fontmap["&#x3F;"] = "?";
	fontmap["&#x40;"] = "@";
	fontmap["&#x41;"] = "A";
	fontmap["&#x42;"] = "B";
	fontmap["&#x43;"] = "C";
	fontmap["&#x44;"] = "D";
	fontmap["&#x45;"] = "E";
	fontmap["&#x46;"] = "F";
	fontmap["&#x47;"] = "G";
	fontmap["&#x48;"] = "H";
	fontmap["&#x49;"] = "I";
	fontmap["&#x4A;"] = "J";
	fontmap["&#x4B;"] = "K";
	fontmap["&#x4C;"] = "L";
	fontmap["&#x4D;"] = "M";
	fontmap["&#x4E;"] = "N";
	fontmap["&#x4F;"] = "O";
	fontmap["&#x50;"] = "P";
	fontmap["&#x51;"] = "Q";
	fontmap["&#x52;"] = "R";
	fontmap["&#x53;"] = "S";
	fontmap["&#x54;"] = "T";
	fontmap["&#x55;"] = "U";
	fontmap["&#x56;"] = "V";
	fontmap["&#x57;"] = "W";
	fontmap["&#x58;"] = "X";
	fontmap["&#x59;"] = "Y";
	fontmap["&#x5A;"] = "Z";
	fontmap["&#x5B;"] = "[";
	fontmap["&#x5C;"] = "\\";
	fontmap["&#x5D;"] = "]";
	fontmap["&#x5E;"] = "^";
	fontmap["&#x5F;"] = "_";
	fontmap["&#x60;"] = "`";
	fontmap["&#x61;"] = "a";
	fontmap["&#x62;"] = "b";
	fontmap["&#x63;"] = "c";
	fontmap["&#x64;"] = "d";
	fontmap["&#x65;"] = "e";
	fontmap["&#x66;"] = "f";
	fontmap["&#x67;"] = "g";
	fontmap["&#x68;"] = "h";
	fontmap["&#x69;"] = "i";
	fontmap["&#x6A;"] = "j";
	fontmap["&#x6B;"] = "k";
	fontmap["&#x6C;"] = "l";
	fontmap["&#x6D;"] = "m";
	fontmap["&#x6E;"] = "n";
	fontmap["&#x6F;"] = "o";
	fontmap["&#x70;"] = "p";
	fontmap["&#x71;"] = "q";
	fontmap["&#x72;"] = "r";
	fontmap["&#x73;"] = "s";
	fontmap["&#x74;"] = "t";
	fontmap["&#x75;"] = "u";
	fontmap["&#x76;"] = "v";
	fontmap["&#x77;"] = "w";
	fontmap["&#x78;"] = "x";
	fontmap["&#x79;"] = "y";
	fontmap["&#x7A;"] = "z";
	fontmap["&#x7B;"] = "{";
	fontmap["&#x7C;"] = "|";
	fontmap["&#x7D;"] = "}";
	fontmap["&#x7E;"] = "~";
	fontmap["&#x7F;"] = " ";
	fontmap["&#x80;"] = "€";
	fontmap["&#x81; "] = "";
	fontmap["&#x82;"] = "‚";
	fontmap["&#x83;"] = "ƒ";
	fontmap["&#x84;"] = "„";
	fontmap["&#x85;"] = "…";
	fontmap["&#x86;"] = "†";
	fontmap["&#x87;"] = "‡";
	fontmap["&#x88; "] = "ˆ";
	fontmap["&#x89;"] = "‰";
	fontmap["&#x8A;"] = "Š";
	fontmap["&#x8B; "] = "‹";
	fontmap["&#x8C;"] = "Œ";
	fontmap["&#x8D;"] = "";
	fontmap["&#x8E;"] = "Ž";
	fontmap["&#x8F; "] = "";
	fontmap["&#x90; "] = "";
	fontmap["&#x91;"] = "‘";
	fontmap["&#x92;"] = "’";
	fontmap["&#x93;"] = "“";
	fontmap["&#x94;"] = "”";
	fontmap["&#x95;"] = "•";
	fontmap["&#x96;"] = "–";
	fontmap["&#x97;"] = "—";
	fontmap["&#x98; "] = "˜";
	fontmap["&#x99;"] = "™";
	fontmap["&#x9A;"] = "š";
	fontmap["&#x9B; "] = "›";
	fontmap["&#x9C;"] = "œ";
	fontmap["&#x9D;"] = "";
	fontmap["&#x9E; "] = "ž";
	fontmap["&#x9F;"] = "Ÿ";
	fontmap["&#xA0;"] = " ";
	fontmap["&#xA1;"] = "¡";
	fontmap["&#xA2;"] = "¢";
	fontmap["&#xA3;"] = "£";
	fontmap["&#xA4;"] = "¤";
	fontmap["&#xA5;"] = "¥";
	fontmap["&#xA6;"] = "¦";
	fontmap["&#xA7;"] = "§";
	fontmap["&#xA8;"] = "¨";
	fontmap["&#xA9;"] = "©";
	fontmap["&#xAA;"] = "ª";
	fontmap["&#xAB;"] = "«";
	fontmap["&#xAC;"] = "¬";
	fontmap["&#xAD;"] = " ";
	fontmap["&#xAE;"] = "®";
	fontmap["&#xAF;"] = "¯";
	fontmap["&#xB0;"] = "°";
	fontmap["&#xB1;"] = "±";
	fontmap["&#xB2;"] = "²";
	fontmap["&#xB3;"] = "³";
	fontmap["&#xB4;"] = "´";
	fontmap["&#xB5;"] = "µ";
	fontmap["&#xB6;"] = "¶";
	fontmap["&#xB7;"] = "·";
	fontmap["&#xB8;"] = "¸";
	fontmap["&#xB9;"] = "¹";
	fontmap["&#xBA;"] = "º";
	fontmap["&#xBB;"] = "»";
	fontmap["&#xBC;"] = "¼";
	fontmap["&#xBD;"] = "½";
	fontmap["&#xBE;"] = "¾";
	fontmap["&#xBF;"] = "¿";
	fontmap["&#xC0;"] = "À";
	fontmap["&#xC1;"] = "Á";
	fontmap["&#xC2;"] = "Â";
	fontmap["&#xC3;"] = "Ã";
	fontmap["&#xC4;"] = "Ä";
	fontmap["&#xC5;"] = "Å";
	fontmap["&#xC6;"] = "Æ";
	fontmap["&#xC7;"] = "Ç";
	fontmap["&#xC8;"] = "È";
	fontmap["&#xC9;"] = "É";
	fontmap["&#xCA;"] = "Ê";
	fontmap["&#xCB;"] = "Ë";
	fontmap["&#xCC;"] = "Ì";
	fontmap["&#xCD;"] = "Í";
	fontmap["&#xCE;"] = "Î";
	fontmap["&#xCF;"] = "Ï";
	fontmap["&#xD0;"] = "Ð";
	fontmap["&#xD1;"] = "Ñ";
	fontmap["&#xD2;"] = "Ò";
	fontmap["&#xD3;"] = "Ó";
	fontmap["&#xD4;"] = "Ô";
	fontmap["&#xD5;"] = "Õ";
	fontmap["&#xD6;"] = "Ö";
	fontmap["&#xD7;"] = "×";
	fontmap["&#xD8;"] = "Ø";
	fontmap["&#xD9;"] = "Ù";
	fontmap["&#xDA;"] = "Ú";
	fontmap["&#xDB;"] = "Û";
	fontmap["&#xDC;"] = "Ü";
	fontmap["&#xDD;"] = "Ý";
	fontmap["&#xDE;"] = "Þ";
	fontmap["&#xDF;"] = "ß";
	fontmap["&#xE0;"] = "à";
	fontmap["&#xE1;"] = "á";
	fontmap["&#xE2;"] = "â";
	fontmap["&#xE3;"] = "ã";
	fontmap["&#xE4;"] = "ä";
	fontmap["&#xE5;"] = "å";
	fontmap["&#xE6;"] = "æ";
	fontmap["&#xE7;"] = "ç";
	fontmap["&#xE8;"] = "è";
	fontmap["&#xE9;"] = "é";
	fontmap["&#xEA;"] = "ê";
	fontmap["&#xEB;"] = "ë";
	fontmap["&#xEC;"] = "ì";
	fontmap["&#xED;"] = "í";
	fontmap["&#xEE;"] = "î";
	fontmap["&#xEF;"] = "ï";
	fontmap["&#xF0;"] = "ð";
	fontmap["&#xF1;"] = "ñ";
	fontmap["&#xF2;"] = "ò";
	fontmap["&#xF3;"] = "ó";
	fontmap["&#xF4;"] = "ô";
	fontmap["&#xF5;"] = "õ";
	fontmap["&#xF6;"] = "ö";
	fontmap["&#xF7;"] = "÷";
	fontmap["&#xF8;"] = "ø";
	fontmap["&#xF9;"] = "ù";
	fontmap["&#xFA;"] = "ú";
	fontmap["&#xFB;"] = "û";
	fontmap["&#xFC;"] = "ü";
	fontmap["&#xFD;"] = "ý";
	fontmap["&#xFE;"] = "þ";
	fontmap["&#xFF;"] = "ÿ";
	return fontmap;
}

const std::map<std::string, std::string> SVGFont::hexToCharMap = SVGFont::makeHexToCharMap();
#endif
