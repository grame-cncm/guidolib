#include "GuidoFont.h"

GuidoFont::GuidoFont(const char *name, int properties) : name(name), properties(properties)
{
}

GuidoFont::~GuidoFont()
{
}

inline const char * GuidoFont::GetName() const
{
    return name.c_str();
}

inline int GuidoFont::GetSize() const
{
    return kFontSize;
}

inline int GuidoFont::GetProperties() const
{
    return properties;
}

void GuidoFont::GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const
{
	*outWidth = 0;
	*outHeight = 0;
    for(int i = 0; i < inCharCount; i++)
    {
		*outWidth += kGuidoFontWidth[s[inCharCount]];
		*outHeight += kGuidoFontHeight[s[inCharCount]];
    }
}

void GuidoFont::GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * context ) const
{
    *outWidth = kGuidoFontWidth[c];
    *outHeight = kGuidoFontHeight[c];
}
