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

#ifndef __GuidoViewer__
#define __GuidoViewer__


#include "GuidoComponent.h"
#include "JuceHeader.h"

class MainAppWindow;
//==============================================================================
class GuidoViewer  : 
			public GuidoComponent,
			public MenuBarModel,
			public ApplicationCommandTarget,
			public FileDragAndDropTarget
{

public:
    //==============================================================================
			 GuidoViewer (MainAppWindow* w);
	virtual ~GuidoViewer()	{}


    //==============================================================================
    StringArray getMenuBarNames();
    PopupMenu getMenuForIndex (int menuIndex, const String& /*menuName*/);
    void menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/);

    //==============================================================================
    // The following methods implement the ApplicationCommandTarget interface, allowing
    // this window to publish a set of actions it can perform, and which can be mapped
    // onto menus, keypresses, etc.
    ApplicationCommandTarget* getNextCommandTarget();
    void getAllCommands (Array <CommandID>& commands);

    // This method is used when something needs to find out the details about one of the commands
    // that this object can perform..
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);

    // this is the ApplicationCommandTarget method that is used to actually perform one of our commands..
    bool perform (const InvocationInfo& info);

	void paint (Graphics& g);		// paint overrided for drop feedback

    // drop support
	bool 	isInterestedInFileDrag (const StringArray &files);
	void 	fileDragEnter (const StringArray &files, int x, int y);
	void 	fileDragExit (const StringArray &files);
	void 	filesDropped (const StringArray &files, int x, int y);

private:
    //==============================================================================
    MainAppWindow* fWindow;

    TooltipWindow tooltipWindow; // to add tooltips to an application, you
                                 // just need to create one of these and leave it
                                 // there to do its work..
	bool fDragEntered;
	File fFileLocation;
	
	String				ChooseGmnFile();
	bool				ChooseExportFile(File& file);
	ImageFileFormat*	File2ImageFileFormat (const File& file);
	void				setFile (String file);
	void				export2Image (const File& file, ImageFileFormat* format);

    enum CommandIDs
    {
        kOpen		= 0x2000,
        kPrint		= 0x2001,
        kExport		= 0x2002,
        kReload		= 0x2003,
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuidoViewer);
};

#endif
