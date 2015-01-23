
======================================================================
	Guido Quartz Viewer (c) 2003-2004 Grame
======================================================================
Important note: The application must be released with the guido2.ttf file
in its bundle's Resources folder. 
Sources directory:
- GuidoQuartzViewer.mcp	
A rough MacOS X Carbon application. Does not use PowerPlant, 
does not use wxWindows, so it will compile everywhere.
- xcode: a folder containing an xcode project

======================================================================
Todo list:

- Midi export
- Midi import
- Play music.
- Zoom does not change the current scroll position when possible.
- Display the about box at startup if opened without document.
- User can chose the background color for scores.
- Displays current zoom.
- User can change Guido score rendering settings and options
- Gif, EPS, PDF Export
- Standard file hierarchy popup in document windows (Like finder's 
  windows)
- Fullscreen mode with two pages and turn-page features.

======================================================================
Done list:
- Bitmap images export (With Quicktime, so it supports all formats supported by Quicktime)
- Displays alert boxes on error.
- Display parse error lines.
- Window fits score page dimensions.
- Printing.
- Auto-load the musical font from the bundle's resources folder.
- Scrollbars, window resize, zoom.
- Open document windows at the maximum height available on the desktop.
- Continues to work with a file even if it was moved of re-created.
- Multiple documents.
- Reload file.
- User can change the page number.
- Displays the current page in the window's title.
- Displays the file name in the window's title.
- Does not open several windows for the same file. When the user loads a file
 that is already opened, the viewer refresh (reload) it.


