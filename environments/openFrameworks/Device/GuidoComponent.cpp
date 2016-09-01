/*
	GUIDO Library
	Copyright (C) 2014	Thomas Coffy / IRCAM

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License (Version 2), 
	as published by the Free Software Foundation.
	A copy of the license can be found online at www.gnu.org/licenses.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.
*/

#include <iostream>
#include <sstream>
#include <string>

#include "openFrameworksDevice.h"
#include "openFrameworksSystem.h"
#include "GuidoComponent.h"
#include "GUIDOParse.h"

using namespace std;


static openFrameworksSystem gSystem;
static openFrameworksDevice gDevice(&gSystem);

openFrameworksDevice* GuidoComponent::getDevice() {
	return &gDevice;
}

GuidoComponent::GuidoComponent () 
	: fARHandler(0), fGRHandler(0), fPage(0), fResizeToMusic(false), fScoreColor(0,0,0)
{
	GuidoGetDefaultLayoutSettings (&fSettings);
}

GuidoComponent::~GuidoComponent()
{
	if (fGRHandler) GuidoFreeGR (fGRHandler);
	if (fARHandler) GuidoFreeAR (fARHandler);
}


//==============================================================================
GuidoErrCode GuidoComponent::GuidoInit (const char* textfont, const char* guidofont)
{
	GuidoInitDesc desc;
	desc.graphicDevice = &gDevice;
	desc.musicFont	= guidofont;
	desc.textFont	= textfont;
	return  ::GuidoInit(&desc);
}

//==============================================================================
GuidoErrCode GuidoComponent::setGMNFile (const char* file )
{
	ARHandler arh;
	GuidoErrCode err = GuidoParseFile (file, &arh);
	if (err != guidoNoErr) return err;
	
	err = setARHandler (arh);
	if (err != guidoNoErr) GuidoFreeAR (arh);
	else {
		//File f(file);
		//fGMNCode = f.loadFileAsString ();
		ifstream outfile;
		outfile.open (file);
		outfile >> fGMNCode;
		outfile.close();
		fGMNFile = file;
	}
	return err;
}

//==============================================================================
GuidoErrCode GuidoComponent::setGMNCode (const char* gmnCode )
{
	ARHandler arh;

	GuidoParser* parser = GuidoOpenParser();
	arh = GuidoString2AR(parser, gmnCode);
	int l, c;
	const char *errstr;
	GuidoErrCode err = GuidoParserGetErrorCode(parser, l, c, &errstr);

	if (err != guidoNoErr) GuidoFreeAR (arh);
	else {
		fGMNCode = gmnCode;
		err = setARHandler (arh);
	}
	GuidoCloseParser(parser);
	return err;
}

//==============================================================================
GuidoErrCode GuidoComponent::setARHandler(ARHandler ar)
{
	GRHandler grh;
	GuidoErrCode err = GuidoAR2GR (ar, &fSettings, &grh);
	if (err == guidoNoErr) {
		if (fGRHandler) GuidoFreeGR (fGRHandler);
		fGRHandler = grh;
		fARHandler = ar;
		fPage = 1;
		if (fResizeToMusic) GuidoResizePageToMusic (fGRHandler);
		//draw();
	}
	else GuidoFreeAR (ar);
	return err;
}

//==============================================================================
void GuidoComponent::setScoreColor(const ofColor& color)
{
	if (fScoreColor != color) {
		fScoreColor = color;
		//draw();
	}
}

//==============================================================================
void GuidoComponent::setGuidoLayoutSettings(const GuidoLayoutSettings& layoutSettings)
{
	fSettings = layoutSettings;
	if (fGRHandler) {
		GuidoUpdateGR (fGRHandler, &fSettings);
		//draw();
	}
}

//==============================================================================
void GuidoComponent::setResizePageToMusic(bool isOn)
{
	if (fResizeToMusic != isOn) {
		fResizeToMusic = isOn;
		setARHandler (fARHandler);
	}
}

//==============================================================================
int GuidoComponent::pageCount() const
{
	return fGRHandler ? GuidoGetPageCount (fGRHandler) : 0;
}

//==============================================================================
int GuidoComponent::voiceCount() const
{
	return fARHandler ? GuidoCountVoices (fARHandler) : 0;
}

void GuidoComponent::setWidth(int w) {
	width = w;
}

void GuidoComponent::setHeight(int h) {
	height = h;
}

int GuidoComponent::getWidth() {
	return width;
}

int GuidoComponent::getHeight() {
	return height;
}



//==============================================================================
void GuidoComponent::draw (int x, int y, int w, int h)
{
	if (!fGRHandler) return;

	ofSetColor(fScoreColor);
	GuidoOnDrawDesc desc;
	desc.handle = fGRHandler;
	desc.hdc = &gDevice;
	desc.page = fPage;
	desc.scrollx = 0;
	desc.scrolly = 0;
	desc.isprint = false;
	desc.updateRegion.left = x;
	desc.updateRegion.top = y;
	desc.updateRegion.right = w;
	desc.updateRegion.bottom = h;
	desc.updateRegion.erase = true;
	desc.sizex = w;
	desc.sizey = h;

	//GuidoUpdateGR(fGRHandler, &fSettings);
	GuidoErrCode err = GuidoOnDraw (&desc);
	if (err != guidoNoErr)
		cerr << "error while painting: " << GuidoGetErrorString(err) << endl;
}

