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

#include "GuidoViewer.h"
#include "MainWindow.h"

using namespace std;
using namespace juce;

//==============================================================================
GuidoViewer::GuidoViewer (MainAppWindow* w) 
	: fWindow (w), fDragEntered(false)
{
	setResizePageToMusic (true);
	fFileLocation = File::getSpecialLocation(File::userHomeDirectory);
}

//==============================================================================
const StringArray GuidoViewer::getMenuBarNames()
{
	const char* const names[] = { "File", 0 };
	return StringArray (names);
}

//-------------------------------------------------------------------------------
const PopupMenu GuidoViewer::getMenuForIndex (int menuIndex, const String& /*menuName*/)
{
	ApplicationCommandManager* commandManager = &(fWindow->commandManager);

	PopupMenu menu;
	if (menuIndex == 0)
	{
		menu.addCommandItem (commandManager, kOpen);
//		menu.addCommandItem (commandManager, kPrint);
		menu.addCommandItem (commandManager, kExport);
		menu.addCommandItem (commandManager, kReload);
//		menu.addSeparator();
	}
	return menu;
}

//-------------------------------------------------------------------------------
void GuidoViewer::menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/)
{
	// most of our menu items are invoked automatically as commands, but we can handle the
	// other special cases here..
	if (menuItemID >= 5001 && menuItemID < 5010)
		getPeer()->setCurrentRenderingEngine (menuItemID - 5001);
}

//==============================================================================
// The following methods implement the ApplicationCommandTarget interface, allowing
// this window to publish a set of actions it can perform, and which can be mapped
// onto menus, keypresses, etc.
ApplicationCommandTarget* GuidoViewer::getNextCommandTarget()
{
	// this will return the next parent component that is an ApplicationCommandTarget (in this
	// case, there probably isn't one, but it's best to use this method in your own apps).
	return findFirstTargetParentComponent();
}

//-------------------------------------------------------------------------------
void GuidoViewer::getAllCommands (Array <CommandID>& commands)
{
	// this returns the set of all commands that this target can perform..
	const CommandID ids[] = { kOpen, /*kPrint,*/ kExport, kReload };
	commands.addArray (ids, numElementsInArray (ids));
}


//-------------------------------------------------------------------------------
// This method is used when something needs to find out the details about one of the commands
// that this object can perform..
void GuidoViewer::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
	const String generalCategory ("General");
	switch (commandID)
	{
		case kOpen:
			result.setInfo ("Open", "Open a GMN file", generalCategory, 0);
			result.setTicked (false);
			result.addDefaultKeypress ('o', ModifierKeys::commandModifier);
			break;

//		case kPrint:
//			result.setInfo ("Print", "Print the current score", generalCategory, 0);
//			result.setTicked (false);
//			result.addDefaultKeypress ('p', ModifierKeys::commandModifier);
//			break;

		case kExport:
			result.setInfo ("Export", "Export the current score", generalCategory, 0);
			result.setTicked (false);
			result.addDefaultKeypress ('e', ModifierKeys::commandModifier);
			break;

		case kReload:
			result.setInfo ("Reload", "Reload the current file", generalCategory, 0);
			result.setTicked (false);
			result.addDefaultKeypress ('r', ModifierKeys::commandModifier);
			break;

		default:
			break;
	};
}


//-------------------------------------------------------------------------------
String GuidoViewer::ChooseGmnFile()
{
	String file;
	FileChooser chooser("Choose a GMN file", fFileLocation, "*.gmn");
	if (chooser.browseForFileToOpen()) {
		fFileLocation = chooser.getResult();
		file = fFileLocation.getFullPathName();
	}
	return file;
}

//-------------------------------------------------------------------------------
bool GuidoViewer::ChooseExportFile(File& file)
{
	FileChooser chooser("Export to", fFileLocation);
	if (chooser.browseForFileToSave(true)) {
		fFileLocation = chooser.getResult();
		file = fFileLocation;
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------
ImageFileFormat* GuidoViewer::File2ImageFileFormat (const File& file)
{
	ImageFileFormat* iff = 0;
	if (file.hasFileExtension ("png"))
		iff = new PNGImageFormat();
	else if (file.hasFileExtension ("jpg;jpeg"))
		iff = new JPEGImageFormat();
	else {
		String msg = "Can't guess a valid file format from " + file.getFileName() + " extension.\n";
		msg += "Retry using one of .png .jpg .jpeg";
		AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Export error", msg, "OK", this);
	}
	return iff;
}

//-------------------------------------------------------------------------------
void GuidoViewer::export2Image (const File& file, ImageFileFormat* format)
{
	if (!format) return;
	Image::PixelFormat pf = Image::RGB;
	if (format->getFormatName() == "PNG")
		pf = Image::ARGB;
	
	SoftwareImageType type;
	Image img (pf, getWidth(), getHeight(), true, type);
	Graphics g (img);
	if (pf == Image::RGB) {
		g.setColour (Colours::white);
		g.fillRect (0, 0, getWidth(), getHeight());
	}
	FileOutputStream outfile (file);
	GuidoComponent::paint(g);
	format->writeImageToStream (img, outfile);
	delete format;
}

//-------------------------------------------------------------------------------
void GuidoViewer::setFile (String file)
{
	if (file.length()) {
		GuidoErrCode err = setGMNFile(file.getCharPointer());
		if (err == guidoNoErr) 
			fWindow->setName (file);
		else {
			String msg = GuidoGetErrorString (err);
			if (err == guidoErrParse) {
				msg += " line ";
				msg += GuidoGetParseErrorLine();
			}
			AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "Error", msg, "OK", this);
		}
	}
}

//-------------------------------------------------------------------------------
// this is the ApplicationCommandTarget method that is used to actually perform one of our commands..
bool GuidoViewer::perform (const InvocationInfo& info)
{
	switch (info.commandID)
	{
		case kOpen:
			setFile( ChooseGmnFile());
			break;

//		case kPrint:
//			break;

		case kExport: {
				File f;
				if (ChooseExportFile(f)) 
					export2Image (f, File2ImageFileFormat (f));
			}
			break;

		case kReload: {
			const String& file = fWindow->getName();
			if (file.length())
				setFile (file);
			}
			break;

		default:
			return false;
	};
	return true;
}

//-------------------------------------------------------------------------------
void GuidoViewer::paint (Graphics& g)
{
	GuidoComponent::paint(g);
	if (fDragEntered) {
		float gray = 0.3f;
		float border = 0.f;
		float alpha = 0.4f;
		for (int i=0; i<4; i++, alpha-=0.1f, border+=1.f) {
			g.setColour( Colour(gray, gray, gray, alpha));
			g.drawRect (border, border, getWidth()-(border*2), getHeight()-(border*2), 1.0f);
		}
	}
}

//-------------------------------------------------------------------------------
bool GuidoViewer::isInterestedInFileDrag (const StringArray&)
{
	return true;
}

void GuidoViewer::fileDragEnter (const StringArray&, int , int)
{
	fDragEntered = true;
	repaint();
}

void GuidoViewer::fileDragExit (const StringArray&)
{
	fDragEntered = false;
	repaint();
}

void GuidoViewer::filesDropped (const StringArray &files, int, int )
{
	fDragEntered = false;
	setFile (files[0]);
	fFileLocation = files[0];
}

