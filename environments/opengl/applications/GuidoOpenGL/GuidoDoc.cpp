
/*
	GUIDO Library
	Copyright (C) 2005	Grame

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

#include "GuidoDoc.h"
using namespace std;

// ---------------------------------------------------------------------------
GuidoDoc::GuidoDoc()
{
	fGR = 0;
	fZoom = 1;
	fZoomFactor = float(5) / float(4);
	fPageNum = 1;
	ClearSettings();
}

// ---------------------------------------------------------------------------
GuidoDoc::~GuidoDoc()
{
//	if (fGR) GuidoFreeGR( fGR );
	fGR = 0;
}

// ---------------------------------------------------------------------------
GuidoErrCode GuidoDoc::Read ( const char * inFilePath ) 
{
	GuidoLayoutSettings * settings = GetSettings();
	ARHandler ar = 0;
	GRHandler gr = 0;
	GuidoErrCode err = GuidoParseFile( inFilePath, &ar );
	if( err == guidoNoErr )
		err = GuidoAR2GR( ar, settings, &gr );
	if (ar) GuidoFreeAR( ar );
	if (gr) {
		if (fGR) GuidoFreeGR (fGR);
		fGR= gr;
		fFile = inFilePath;
	}
	return err;
}

// ---------------------------------------------------------------------------
string GuidoDoc::File() const
{
	int pos = fFile.rfind('/', fFile.size());
	return (pos==string::npos) ? fFile : fFile.substr(pos+1, fFile.size()-pos-1);
}

// ---------------------------------------------------------------------------
void GuidoDoc::SetSettings (GuidoLayoutSettings * settings)
{
	fGuidoSettings = *settings;
	GuidoUpdateGR( GetGR(), settings );
}

// ---------------------------------------------------------------------------
void GuidoDoc::ClearSettings()
{
	GuidoGetDefaultLayoutSettings( &fGuidoSettings );
}

// ---------------------------------------------------------------------------
bool GuidoDoc::SetPageNum( int num )
{
	int maxPage = PageCount();
	if( maxPage < 1 ) maxPage = 1;

	if( num > maxPage )			num = maxPage;	
	else if( num < 1 )			num = 1;

	if( num == fPageNum )
		return false;
	fPageNum = num;
	return true;
}

// ---------------------------------------------------------------------------
void GuidoDoc::ZoomIn()
{
	SetZoom( GetZoom() * fZoomFactor );
}

// ---------------------------------------------------------------------------
void GuidoDoc::ZoomOut()
{
	SetZoom( GetZoom() / fZoomFactor );
}

// ---------------------------------------------------------------------------
void GuidoDoc::SetZoom( float zoom )
{
	if( zoom <= 0.001f )	// min zoom: 1 / 1000
		zoom = 0.001f;
	else if( zoom > 1000 )	// max zoom: 1000
		zoom = 1000;
	else if( zoom >= 0.99f && zoom <= 1.01f )	// normalize
		zoom = 1;
	fZoom = zoom;
}
