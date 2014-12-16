
#ifndef __FontDrawer__
#define __FontDrawer__

#include <Carbon/Carbon.h>
#include "GUIDOEngine/VGDevice.h"

class FontDrawer
{
	public:
				 FontDrawer() :fDevice(0) {}
	virtual		~FontDrawer();
	
	// initialize the device
	void init(CGContextRef context);
	// draw the guido font in the view
	void Draw (CGContextRef ref, const HIRect& r);
	void Draw (CGContextRef ref);
	void DrawFont (CGContextRef ref);
	VGDevice* fDevice;
};

#endif TWindow_H_