
#include "GUIDOFactory.h"

// ---------------------------------------------------------------------------
//	Guido Factory example of use
//  Take care that for simplicity, this example doesn't check return codes
// ---------------------------------------------------------------------------
void TestGuidoFactory() 
{
    ARHandler ar; GRHandler gr;
    ARFactoryHandler *f;
	
	GuidoFactoryOpen(&f);               // open the GUIDO Factory first
	
	GuidoFactoryOpenMusic(f);           // and create a new score	
    GuidoFactoryOpenVoice(f);           // open a new voice

	GuidoFactoryOpenEvent(f, "c" );     // open a new note
	GuidoFactoryCloseEvent(f);          // close the note which is now part of the opened voice
	
	GuidoFactoryOpenEvent(f, "e" );     // open another new note
	GuidoFactoryAddSharp(f);            // add a sharp to the opened note
	GuidoFactoryCloseEvent(f);          // and close the note which is now part of the opened voice

	GuidoFactoryCloseVoice(f);          // close the voice which is now part of the opened score

	
	GuidoFactoryOpenVoice(f);           // open a second voice
	GuidoFactoryOpenEvent(f, "a" );     // open a new voice
	GuidoFactoryAddFlat(f);             // add a flat to the opened note
	GuidoFactoryCloseEvent(f);          // and close the note which is now part of the second voice

	GuidoFactoryOpenEvent(f, "d" );     // open another new note
	GuidoFactoryCloseEvent(f);          // and close the note which is now part of the second voice

	GuidoFactoryCloseVoice(f);          // close the voice which is now part of the opened score
	
	// - Extract AR and GR
	ar = GuidoFactoryCloseMusic(f);     // extract the GAR handle
	GuidoAR2GR( ar, 0, &gr );           // and the GGR handle
	GuidoFactoryClose(f);               // close the GUIDO Factory which won't be used any more

	if( gr )                            // check for the handle validity
	{
		// ... Do something with the GGR handle (i.e: draw)
	}
}
