/*
	GUIDO Library
	Copyright (C) 2012	Grame

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
#include <string>

#include "JuceSystem.h"
#include "JuceDevice.h"
#include "JuceFont.h"
#include "GuidoComponent.h"

using namespace std;

static JuceSystem gSystem(0);
static JuceDevice gDevice(0, &gSystem);

//==============================================================================
GuidoComponent::GuidoComponent () 
	: fARHandler(0), fGRHandler(0), fPage(0), fResizeToMusic(false), fScoreColor(0,0,0)
{
	GuidoGetDefaultLayoutSettings (&fSettings);
	setSize (600, 300);
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
		File f(file);
		fGMNCode = f.loadFileAsString ();
		fGMNFile = file;
	}
	return err;
}

//==============================================================================
GuidoErrCode GuidoComponent::setGMNCode (const char* gmnCode )
{
	ARHandler arh;
    GuidoErrCode err = GuidoParseString (gmnCode, &arh);
	if (err != guidoNoErr) return err;
	
	err = setARHandler (arh);
	if (err != guidoNoErr) GuidoFreeAR (arh);
	else fGMNCode = gmnCode;
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
		repaint();
	}
	else GuidoFreeAR (ar);
	return err;
}

//==============================================================================
void GuidoComponent::setScoreColor(const juce::Colour& color)
{
	if (fScoreColor != color) {
		fScoreColor = color;
		repaint();
	}
}

//==============================================================================
void GuidoComponent::setGuidoLayoutSettings(const GuidoLayoutSettings& layoutSettings)
{
	fSettings = layoutSettings;
	if (fGRHandler) {
		GuidoUpdateGR (fGRHandler, &fSettings);
		repaint();
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

//==============================================================================
void GuidoComponent::paint (Graphics& g)
{
	if (!fGRHandler) return;

	JuceSystem sys (&g);
	JuceDevice dev (&g, &sys);

//	DebugDevice dev (jdev);
	dev.NotifySize(getWidth(), getHeight());

//	float x = 10.0f;
//	float y = 10.0f;
//	float w = getWidth() - (x*2);
//	float h = getHeight() - (y*2);

//	dev.SelectFillColor (VGColor(255,0,0,50));
//	dev.SetScale (0.5, 0.5);
//	dev.Rectangle (x, y, x + w, y + h);
//	dev.SetScale (0.8, 0.8);
//	dev.Rectangle (x, y, x + w, y + h);

	g.setColour (fScoreColor);

//	dev.SetScale (0.5, 0.5);
//	JuceFont font("Guido2", 200, VGFont::kFontBold);
//	dev.SetTextFont (&font);
//	string msg("Hello world!");
//	dev.DrawString (x*2, 150, msg.c_str(), msg.size());
//	dev.SetScale (1.,1.);

	GuidoOnDrawDesc desc;
	desc.handle = fGRHandler;
	desc.hdc = &dev;
	desc.page = fPage;
	desc.scrollx = desc.scrolly = 0;
	desc.isprint = false;
	desc.updateRegion.erase = true;
    desc.sizex = getWidth();
	desc.sizey = getHeight();

	GuidoErrCode err = GuidoOnDraw (&desc);
	if (err != guidoNoErr)
		cerr << "error while painting: " << GuidoGetErrorString(err) << endl;
}

