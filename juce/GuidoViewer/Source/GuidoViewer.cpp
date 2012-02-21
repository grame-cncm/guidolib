

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
		menu.addCommandItem (commandManager, kPrint);
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
	const CommandID ids[] = { kOpen, kPrint, kExport, kReload };
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

		case kPrint:
			result.setInfo ("Print", "Print the current score", generalCategory, 0);
			result.setTicked (false);
			result.addDefaultKeypress ('p', ModifierKeys::commandModifier);
			break;

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
String GuidoViewer::ChooseGmnFile() const
{
	static File f = File::getSpecialLocation(File::userHomeDirectory);
	String file;
	FileChooser chooser("Choose a GMN file", f, "*.gmn");
	if (chooser.browseForFileToOpen()) {
		f = chooser.getResult();
		file = f.getFullPathName();
	}
	return file;
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

		case kPrint:
			break;

		case kExport:
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
}

