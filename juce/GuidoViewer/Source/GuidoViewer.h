
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
    const StringArray getMenuBarNames();
    const PopupMenu getMenuForIndex (int menuIndex, const String& /*menuName*/);
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
	
	String	ChooseGmnFile() const;
	void	setFile (String file);

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
