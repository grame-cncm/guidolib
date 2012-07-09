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

#include "MainWindow.h"
#include "GuidoViewer.h"


//==============================================================================
MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colours::lightgrey,
                      DocumentWindow::allButtons)
{
    setBackgroundColour (Colour(255,255,255));
	setUsingNativeTitleBar (true);
	setResizable (true, true);
	centreWithSize (500, 600);

    GuidoViewer* gmnview = new GuidoViewer (this);

    commandManager.registerAllCommandsForTarget (gmnview);
    commandManager.registerAllCommandsForTarget (JUCEApplication::getInstance());
 
	// this lets the command manager use keypresses that arrive in our window to send out commands
    addKeyListener (commandManager.getKeyMappings());

    // sets the main content component for the window to be this tabbed
    // panel. This will be deleted when the window is deleted.
    setContentOwned (gmnview, false);

    // this tells the DocumentWindow to automatically create and manage a MenuBarComponent
    // which uses our contentComp as its MenuBarModel
#if JUCE_MAC
	MenuBarModel::setMacMainMenu (gmnview);
#else
    setMenuBar (gmnview);
#endif

    // tells our menu bar model that it should watch this command manager for
    // changes, and send change messages accordingly.
    gmnview->setApplicationCommandManagerToWatch (&commandManager);
    setVisible (true);

	Font::setFallbackFontName (Font::getDefaultSansSerifFontName());
	GuidoComponent::GuidoInit();
}

MainAppWindow::~MainAppWindow()
{
	GuidoShutdown();

    // because we've set the content comp to be used as our menu bar model, we
    // have to switch this off before deleting the content comp..
    setMenuBar (0);

  #if JUCE_MAC  // ..and also the main bar if we're using that on a Mac...
    MenuBarModel::setMacMainMenu (0);
  #endif

    // clearing the content component will delete the current one, and
    // that will in turn delete all its child components. You don't always
    // have to do this explicitly, because the base class's destructor will
    // also delete the content component, but in this case we need to
    // make sure our content comp has gone away before deleting our command
    // manager.
    clearContentComponent();
}

void MainAppWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}
