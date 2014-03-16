/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "nanosvg.h"
#include "SVGFont.h"

#ifdef INDEPENDENTSVG
// do nothing
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
SVGFont::SVGFont(const char * name, int size, int properties, const char * guidofontfile) :
	fGuidoFontFile(guidofontfile), fSize(size), fName(name), fProperties(properties)
{
#ifdef INDEPENDENTSVG
	fNSVGimage = !fGuidoFontFile.empty() ? nsvgParseFromFile(fGuidoFontFile.c_str(), "px", size) : NULL;
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
		  if ((std::string(1, s[i]) == std::string(shape->unicode)) || maybeHexToChar(shape->unicode) == std::string(1, s[i])) {
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
                if ((strcmp(cs, shape->unicode) == 0) || maybeHexToChar(shape->unicode) == std::string(cs)) {
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

#ifdef INDEPENDENTSVG
std::string SVGFont::maybeHexToChar(const char* tryMe)
{
	if (strcmp("&#x09;", tryMe) == 0) {
		return " ";
	} else if (strcmp("&#x10;", tryMe) == 0) {
		return " ";
	} else if (strcmp("&#x20;", tryMe) == 0) {
		return " ";
	} else if (strcmp("&#x21;", tryMe) == 0) {
		return "!";
	} else if (strcmp("&#x22;", tryMe) == 0) {
		return "\"";
	} else if (strcmp("&#x23;", tryMe) == 0) {
		return "#";
	} else if (strcmp("&#x24;", tryMe) == 0) {
		return "$";
	} else if (strcmp("&#x25;", tryMe) == 0) {
		return "%";
	} else if (strcmp("&#x26;", tryMe) == 0) {
		return "&";
	} else if (strcmp("&#x27;", tryMe) == 0) {
		return "'";
	} else if (strcmp("&#x28;", tryMe) == 0) {
		return "(";
	} else if (strcmp("&#x29;", tryMe) == 0) {
		return ")";
	} else if (strcmp("&#x2A;", tryMe) == 0) {
		return "*";
	} else if (strcmp("&#x2B;", tryMe) == 0) {
		return "+";
	} else if (strcmp("&#x2C;", tryMe) == 0) {
		return ",";
	} else if (strcmp("&#x2D;", tryMe) == 0) {
		return "-";
	} else if (strcmp("&#x2E;", tryMe) == 0) {
		return ".";
	} else if (strcmp("&#x2F;", tryMe) == 0) {
		return "/";
	} else if (strcmp("&#x30;", tryMe) == 0) {
		return "0";
	} else if (strcmp("&#x31;", tryMe) == 0) {
		return "1";
	} else if (strcmp("&#x32;", tryMe) == 0) {
		return "2";
	} else if (strcmp("&#x33;", tryMe) == 0) {
		return "3";
	} else if (strcmp("&#x34;", tryMe) == 0) {
		return "4";
	} else if (strcmp("&#x35;", tryMe) == 0) {
		return "5";
	} else if (strcmp("&#x36;", tryMe) == 0) {
		return "6";
	} else if (strcmp("&#x37;", tryMe) == 0) {
		return "7";
	} else if (strcmp("&#x38;", tryMe) == 0) {
		return "8";
	} else if (strcmp("&#x39;", tryMe) == 0) {
		return "9";
	} else if (strcmp("&#x3A;", tryMe) == 0) {
		return ":";
	} else if (strcmp("&#x3B;", tryMe) == 0) {
		return ";";
	} else if (strcmp("&#x3C;", tryMe) == 0) {
		return "<";
	} else if (strcmp("&#x3D;", tryMe) == 0) {
		return "=";
	} else if (strcmp("&#x3E;", tryMe) == 0) {
		return ">";
	} else if (strcmp("&#x3F;", tryMe) == 0) {
		return "?";
	} else if (strcmp("&#x40;", tryMe) == 0) {
		return "@";
	} else if (strcmp("&#x41;", tryMe) == 0) {
		return "A";
	} else if (strcmp("&#x42;", tryMe) == 0) {
		return "B";
	} else if (strcmp("&#x43;", tryMe) == 0) {
		return "C";
	} else if (strcmp("&#x44;", tryMe) == 0) {
		return "D";
	} else if (strcmp("&#x45;", tryMe) == 0) {
		return "E";
	} else if (strcmp("&#x46;", tryMe) == 0) {
		return "F";
	} else if (strcmp("&#x47;", tryMe) == 0) {
		return "G";
	} else if (strcmp("&#x48;", tryMe) == 0) {
		return "H";
	} else if (strcmp("&#x49;", tryMe) == 0) {
		return "I";
	} else if (strcmp("&#x4A;", tryMe) == 0) {
		return "J";
	} else if (strcmp("&#x4B;", tryMe) == 0) {
		return "K";
	} else if (strcmp("&#x4C;", tryMe) == 0) {
		return "L";
	} else if (strcmp("&#x4D;", tryMe) == 0) {
		return "M";
	} else if (strcmp("&#x4E;", tryMe) == 0) {
		return "N";
	} else if (strcmp("&#x4F;", tryMe) == 0) {
		return "O";
	} else if (strcmp("&#x50;", tryMe) == 0) {
		return "P";
	} else if (strcmp("&#x51;", tryMe) == 0) {
		return "Q";
	} else if (strcmp("&#x52;", tryMe) == 0) {
		return "R";
	} else if (strcmp("&#x53;", tryMe) == 0) {
		return "S";
	} else if (strcmp("&#x54;", tryMe) == 0) {
		return "T";
	} else if (strcmp("&#x55;", tryMe) == 0) {
		return "U";
	} else if (strcmp("&#x56;", tryMe) == 0) {
		return "V";
	} else if (strcmp("&#x57;", tryMe) == 0) {
		return "W";
	} else if (strcmp("&#x58;", tryMe) == 0) {
		return "X";
	} else if (strcmp("&#x59;", tryMe) == 0) {
		return "Y";
	} else if (strcmp("&#x5A;", tryMe) == 0) {
		return "Z";
	} else if (strcmp("&#x5B;", tryMe) == 0) {
		return "[";
	} else if (strcmp("&#x5C;", tryMe) == 0) {
		return "\\";
	} else if (strcmp("&#x5D;", tryMe) == 0) {
		return "]";
	} else if (strcmp("&#x5E;", tryMe) == 0) {
		return "^";
	} else if (strcmp("&#x5F;", tryMe) == 0) {
		return "_";
	} else if (strcmp("&#x60;", tryMe) == 0) {
		return "`";
	} else if (strcmp("&#x61;", tryMe) == 0) {
		return "a";
	} else if (strcmp("&#x62;", tryMe) == 0) {
		return "b";
	} else if (strcmp("&#x63;", tryMe) == 0) {
		return "c";
	} else if (strcmp("&#x64;", tryMe) == 0) {
		return "d";
	} else if (strcmp("&#x65;", tryMe) == 0) {
		return "e";
	} else if (strcmp("&#x66;", tryMe) == 0) {
		return "f";
	} else if (strcmp("&#x67;", tryMe) == 0) {
		return "g";
	} else if (strcmp("&#x68;", tryMe) == 0) {
		return "h";
	} else if (strcmp("&#x69;", tryMe) == 0) {
		return "i";
	} else if (strcmp("&#x6A;", tryMe) == 0) {
		return "j";
	} else if (strcmp("&#x6B;", tryMe) == 0) {
		return "k";
	} else if (strcmp("&#x6C;", tryMe) == 0) {
		return "l";
	} else if (strcmp("&#x6D;", tryMe) == 0) {
		return "m";
	} else if (strcmp("&#x6E;", tryMe) == 0) {
		return "n";
	} else if (strcmp("&#x6F;", tryMe) == 0) {
		return "o";
	} else if (strcmp("&#x70;", tryMe) == 0) {
		return "p";
	} else if (strcmp("&#x71;", tryMe) == 0) {
		return "q";
	} else if (strcmp("&#x72;", tryMe) == 0) {
		return "r";
	} else if (strcmp("&#x73;", tryMe) == 0) {
		return "s";
	} else if (strcmp("&#x74;", tryMe) == 0) {
		return "t";
	} else if (strcmp("&#x75;", tryMe) == 0) {
		return "u";
	} else if (strcmp("&#x76;", tryMe) == 0) {
		return "v";
	} else if (strcmp("&#x77;", tryMe) == 0) {
		return "w";
	} else if (strcmp("&#x78;", tryMe) == 0) {
		return "x";
	} else if (strcmp("&#x79;", tryMe) == 0) {
		return "y";
	} else if (strcmp("&#x7A;", tryMe) == 0) {
		return "z";
	} else if (strcmp("&#x7B;", tryMe) == 0) {
		return "{";
	} else if (strcmp("&#x7C;", tryMe) == 0) {
		return "|";
	} else if (strcmp("&#x7D;", tryMe) == 0) {
		return "}";
	} else if (strcmp("&#x7E;", tryMe) == 0) {
		return "~";
	} else if (strcmp("&#x7F;", tryMe) == 0) {
		return " ";
	} else if (strcmp("&#x80;", tryMe) == 0) {
		return "€";
	} else if (strcmp("&#x81; ", tryMe) == 0) {
		return "";
	} else if (strcmp("&#x82;", tryMe) == 0) {
		return "‚";
	} else if (strcmp("&#x83;", tryMe) == 0) {
		return "ƒ";
	} else if (strcmp("&#x84;", tryMe) == 0) {
		return "„";
	} else if (strcmp("&#x85;", tryMe) == 0) {
		return "…";
	} else if (strcmp("&#x86;", tryMe) == 0) {
		return "†";
	} else if (strcmp("&#x87;", tryMe) == 0) {
		return "‡";
	} else if (strcmp("&#x88; ", tryMe) == 0) {
		return "ˆ";
	} else if (strcmp("&#x89;", tryMe) == 0) {
		return "‰";
	} else if (strcmp("&#x8A;", tryMe) == 0) {
		return "Š";
	} else if (strcmp("&#x8B; ", tryMe) == 0) {
		return "‹";
	} else if (strcmp("&#x8C;", tryMe) == 0) {
		return "Œ";
	} else if (strcmp("&#x8D;", tryMe) == 0) {
		return "";
	} else if (strcmp("&#x8E;", tryMe) == 0) {
		return "Ž";
	} else if (strcmp("&#x8F; ", tryMe) == 0) {
		return "";
	} else if (strcmp("&#x90; ", tryMe) == 0) {
		return "";
	} else if (strcmp("&#x91;", tryMe) == 0) {
		return "‘";
	} else if (strcmp("&#x92;", tryMe) == 0) {
		return "’";
	} else if (strcmp("&#x93;", tryMe) == 0) {
		return "“";
	} else if (strcmp("&#x94;", tryMe) == 0) {
		return "”";
	} else if (strcmp("&#x95;", tryMe) == 0) {
		return "•";
	} else if (strcmp("&#x96;", tryMe) == 0) {
		return "–";
	} else if (strcmp("&#x97;", tryMe) == 0) {
		return "—";
	} else if (strcmp("&#x98; ", tryMe) == 0) {
		return "˜";
	} else if (strcmp("&#x99;", tryMe) == 0) {
		return "™";
	} else if (strcmp("&#x9A;", tryMe) == 0) {
		return "š";
	} else if (strcmp("&#x9B; ", tryMe) == 0) {
		return "›";
	} else if (strcmp("&#x9C;", tryMe) == 0) {
		return "œ";
	} else if (strcmp("&#x9D;", tryMe) == 0) {
		return "";
	} else if (strcmp("&#x9E; ", tryMe) == 0) {
		return "ž";
	} else if (strcmp("&#x9F;", tryMe) == 0) {
		return "Ÿ";
	} else if (strcmp("&#xA0;", tryMe) == 0) {
		return " ";
	} else if (strcmp("&#xA1;", tryMe) == 0) {
		return "¡";
	} else if (strcmp("&#xA2;", tryMe) == 0) {
		return "¢";
	} else if (strcmp("&#xA3;", tryMe) == 0) {
		return "£";
	} else if (strcmp("&#xA4;", tryMe) == 0) {
		return "¤";
	} else if (strcmp("&#xA5;", tryMe) == 0) {
		return "¥";
	} else if (strcmp("&#xA6;", tryMe) == 0) {
		return "¦";
	} else if (strcmp("&#xA7;", tryMe) == 0) {
		return "§";
	} else if (strcmp("&#xA8;", tryMe) == 0) {
		return "¨";
	} else if (strcmp("&#xA9;", tryMe) == 0) {
		return "©";
	} else if (strcmp("&#xAA;", tryMe) == 0) {
		return "ª";
	} else if (strcmp("&#xAB;", tryMe) == 0) {
		return "«";
	} else if (strcmp("&#xAC;", tryMe) == 0) {
		return "¬";
	} else if (strcmp("&#xAD;", tryMe) == 0) {
		return " ";
	} else if (strcmp("&#xAE;", tryMe) == 0) {
		return "®";
	} else if (strcmp("&#xAF;", tryMe) == 0) {
		return "¯";
	} else if (strcmp("&#xB0;", tryMe) == 0) {
		return "°";
	} else if (strcmp("&#xB1;", tryMe) == 0) {
		return "±";
	} else if (strcmp("&#xB2;", tryMe) == 0) {
		return "²";
	} else if (strcmp("&#xB3;", tryMe) == 0) {
		return "³";
	} else if (strcmp("&#xB4;", tryMe) == 0) {
		return "´";
	} else if (strcmp("&#xB5;", tryMe) == 0) {
		return "µ";
	} else if (strcmp("&#xB6;", tryMe) == 0) {
		return "¶";
	} else if (strcmp("&#xB7;", tryMe) == 0) {
		return "·";
	} else if (strcmp("&#xB8;", tryMe) == 0) {
		return "¸";
	} else if (strcmp("&#xB9;", tryMe) == 0) {
		return "¹";
	} else if (strcmp("&#xBA;", tryMe) == 0) {
		return "º";
	} else if (strcmp("&#xBB;", tryMe) == 0) {
		return "»";
	} else if (strcmp("&#xBC;", tryMe) == 0) {
		return "¼";
	} else if (strcmp("&#xBD;", tryMe) == 0) {
		return "½";
	} else if (strcmp("&#xBE;", tryMe) == 0) {
		return "¾";
	} else if (strcmp("&#xBF;", tryMe) == 0) {
		return "¿";
	} else if (strcmp("&#xC0;", tryMe) == 0) {
		return "À";
	} else if (strcmp("&#xC1;", tryMe) == 0) {
		return "Á";
	} else if (strcmp("&#xC2;", tryMe) == 0) {
		return "Â";
	} else if (strcmp("&#xC3;", tryMe) == 0) {
		return "Ã";
	} else if (strcmp("&#xC4;", tryMe) == 0) {
		return "Ä";
	} else if (strcmp("&#xC5;", tryMe) == 0) {
		return "Å";
	} else if (strcmp("&#xC6;", tryMe) == 0) {
		return "Æ";
	} else if (strcmp("&#xC7;", tryMe) == 0) {
		return "Ç";
	} else if (strcmp("&#xC8;", tryMe) == 0) {
		return "È";
	} else if (strcmp("&#xC9;", tryMe) == 0) {
		return "É";
	} else if (strcmp("&#xCA;", tryMe) == 0) {
		return "Ê";
	} else if (strcmp("&#xCB;", tryMe) == 0) {
		return "Ë";
	} else if (strcmp("&#xCC;", tryMe) == 0) {
		return "Ì";
	} else if (strcmp("&#xCD;", tryMe) == 0) {
		return "Í";
	} else if (strcmp("&#xCE;", tryMe) == 0) {
		return "Î";
	} else if (strcmp("&#xCF;", tryMe) == 0) {
		return "Ï";
	} else if (strcmp("&#xD0;", tryMe) == 0) {
		return "Ð";
	} else if (strcmp("&#xD1;", tryMe) == 0) {
		return "Ñ";
	} else if (strcmp("&#xD2;", tryMe) == 0) {
		return "Ò";
	} else if (strcmp("&#xD3;", tryMe) == 0) {
		return "Ó";
	} else if (strcmp("&#xD4;", tryMe) == 0) {
		return "Ô";
	} else if (strcmp("&#xD5;", tryMe) == 0) {
		return "Õ";
	} else if (strcmp("&#xD6;", tryMe) == 0) {
		return "Ö";
	} else if (strcmp("&#xD7;", tryMe) == 0) {
		return "×";
	} else if (strcmp("&#xD8;", tryMe) == 0) {
		return "Ø";
	} else if (strcmp("&#xD9;", tryMe) == 0) {
		return "Ù";
	} else if (strcmp("&#xDA;", tryMe) == 0) {
		return "Ú";
	} else if (strcmp("&#xDB;", tryMe) == 0) {
		return "Û";
	} else if (strcmp("&#xDC;", tryMe) == 0) {
		return "Ü";
	} else if (strcmp("&#xDD;", tryMe) == 0) {
		return "Ý";
	} else if (strcmp("&#xDE;", tryMe) == 0) {
		return "Þ";
	} else if (strcmp("&#xDF;", tryMe) == 0) {
		return "ß";
	} else if (strcmp("&#xE0;", tryMe) == 0) {
		return "à";
	} else if (strcmp("&#xE1;", tryMe) == 0) {
		return "á";
	} else if (strcmp("&#xE2;", tryMe) == 0) {
		return "â";
	} else if (strcmp("&#xE3;", tryMe) == 0) {
		return "ã";
	} else if (strcmp("&#xE4;", tryMe) == 0) {
		return "ä";
	} else if (strcmp("&#xE5;", tryMe) == 0) {
		return "å";
	} else if (strcmp("&#xE6;", tryMe) == 0) {
		return "æ";
	} else if (strcmp("&#xE7;", tryMe) == 0) {
		return "ç";
	} else if (strcmp("&#xE8;", tryMe) == 0) {
		return "è";
	} else if (strcmp("&#xE9;", tryMe) == 0) {
		return "é";
	} else if (strcmp("&#xEA;", tryMe) == 0) {
		return "ê";
	} else if (strcmp("&#xEB;", tryMe) == 0) {
		return "ë";
	} else if (strcmp("&#xEC;", tryMe) == 0) {
		return "ì";
	} else if (strcmp("&#xED;", tryMe) == 0) {
		return "í";
	} else if (strcmp("&#xEE;", tryMe) == 0) {
		return "î";
	} else if (strcmp("&#xEF;", tryMe) == 0) {
		return "ï";
	} else if (strcmp("&#xF0;", tryMe) == 0) {
		return "ð";
	} else if (strcmp("&#xF1;", tryMe) == 0) {
		return "ñ";
	} else if (strcmp("&#xF2;", tryMe) == 0) {
		return "ò";
	} else if (strcmp("&#xF3;", tryMe) == 0) {
		return "ó";
	} else if (strcmp("&#xF4;", tryMe) == 0) {
		return "ô";
	} else if (strcmp("&#xF5;", tryMe) == 0) {
		return "õ";
	} else if (strcmp("&#xF6;", tryMe) == 0) {
		return "ö";
	} else if (strcmp("&#xF7;", tryMe) == 0) {
		return "÷";
	} else if (strcmp("&#xF8;", tryMe) == 0) {
		return "ø";
	} else if (strcmp("&#xF9;", tryMe) == 0) {
		return "ù";
	} else if (strcmp("&#xFA;", tryMe) == 0) {
		return "ú";
	} else if (strcmp("&#xFB;", tryMe) == 0) {
		return "û";
	} else if (strcmp("&#xFC;", tryMe) == 0) {
		return "ü";
	} else if (strcmp("&#xFD;", tryMe) == 0) {
		return "ý";
	} else if (strcmp("&#xFE;", tryMe) == 0) {
		return "þ";
	} else if (strcmp("&#xFF;", tryMe) == 0) {
		return "ÿ";
	}
	return tryMe;
}
#endif