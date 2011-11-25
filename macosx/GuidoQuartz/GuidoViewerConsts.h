
#ifndef GuidoViewerConsts_H
#define GuidoViewerConsts_H


const CFStringRef kGuidoURLString = CFSTR("http://guidolib.sourceforge.net/");

const CFStringRef kMainNibFileName = CFSTR("GuidoQuartzViewer");
const CFStringRef kMenuBarNibName  = CFSTR("MainMenu");
const CFStringRef kGuidoWindowNibName = CFSTR("MainWindow");
const CFStringRef kExportDialogNibName = CFSTR("GfxExportWindow");


const ControlID kGuidoControlID = { 'Gido', 1000 };

const ControlID kScrollBarH = { 'Gido', 1005 };
const ControlID kScrollBarV = { 'Gido', 1006 };


const FourCharCode kCmdZoomIn = 'ZIn ';
const FourCharCode kCmdZoomOut = 'ZOut';
const FourCharCode kCmdZoomDefault = 'ZDef';

enum
{
	kMenuFile = 200,
	kMenuEdit = 201,
	kMenuView = 202
	
};

enum
{
	// - File Menu
	kFileItemNew = 1,
	kFileItemReload = 3,
	kFileItemClose = 5,
	kFileItemExport = 10,
	kFileItemPrint = 13,
	
	// - View menu
	kViewItemPrevPage = 1,
	kViewItemNextPage = 2,
	kViewItemShowMapping = 3,

	kViewItemPitchOnly = 4,	
	kViewItemRythmOnly = 5,	
	kViewItemPitchAndRythm = 6,	// "normal"
	
	kViewItemDefaultZoom = 8,
	kViewItemZoomIn = 9,
	kViewItemZoomOut = 10
};


#endif