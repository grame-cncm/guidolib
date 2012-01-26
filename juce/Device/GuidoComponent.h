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


#ifndef __GuidoComponent__
#define __GuidoComponent__

#include "JuceHeader.h"
#include "GUIDOEngine.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GuidoComponent  : public Component
{
	
public:
    //==============================================================================
			 GuidoComponent();
	virtual ~GuidoComponent();

		/**	\brief Sets the current Guido code to draw.
		*	\param file a Guido Music Notation file
		*	\return a guido error code
		*/
		GuidoErrCode setGMNFile( const char* file );

		/**	\brief Sets the current Guido code to draw.
		*	\param gmnCode The Guido Music Notation code
		*	\return true if the GMN code is valid.
		*/
		GuidoErrCode setGMNCode( const char* gmnCode );
		
		/**	\brief Returns the current Guido code.
		*/		
		const String& getGMNCode() const	{ return fGMNCode; }	
		
		/**
		*	\brief Returns the number of pages of the current Guido Score.
		*/
		int pageCount() const;
		
		/**
		*	\brief Returns the number of voices of the current Guido Score.
		*/
		int voiceCount() const;

		/**
		*	\brief Sets the first displayed page of the Guido Score.
		*
		*	\return True if the pageIndex is valid, false else.
		*/
		bool setPage(int pageIndex);
		
		/**
		*	\brief Returns the current page index.
		*/
		int getPage() const			{ return fPage; }
		
		/**
		*	\brief Sets the Guido layout settings used to draw with this QGuidoPainter
		*/
		void setGuidoLayoutSettings(const GuidoLayoutSettings& layoutSettings);

		/**
		*	\brief Returns the Guido layout settings of the QGuidoPainter.
		*/
		const GuidoLayoutSettings& getGuidoLayoutSettings() const	{ return fSettings; }

		/// \brief sets the minimum systems distance to its default value
		void  resetSystemsDistance();
		/// \brief sets the minimum systems distance
		void  setSystemsDistance(float distance);
		/// \brief returns the minimum systems distance
		float getSystemsDistance() const;		
		/// \brief Disable/enable automatic ResizePageToMusic
		void setResizePageToMusic(bool isOn);
		/// \brief Returns the state of the automatic ResizePageToMusic mode (enabled or disabled)
		bool isResizePageToMusic() const		{ return fResizeToMusic; }

		/// \brief Sets the page format used when no page format is specified by the GMN
		void setGuidoPageFormat(const GuidoPageFormat& pageFormat);
		/// \brief Gets the page format used when no page format is specified by the GMN
		GuidoPageFormat guidoPageFormat() const;
		
		/**
		*	\brief Gives access to the GRHandler (graphic representation) of the Score in read-only.
		*/
        GRHandler   getGRHandler() const { return fGRHandler; }

		/**
		*	\brief Gives access to the ARHandler (abstract representation) of the Score in read-only.
		*/		
        ARHandler   getARHandler() const { return fARHandler; }
		
		/**
		*	\brief Directly set the AR handler .
		*/
		GuidoErrCode setARHandler(ARHandler ar);

		/// \brief sets the color used to draw the score
		void setScoreColor(const juce::Colour& color);
		/// \brief returns the color used to draw the score
		const juce::Colour& getScoreColor() const		{ return fScoreColor; }


		void paint (Graphics& g);
//		void resized();

	/**
	*	\brief Guido Engine initialization.
	
		\param textfont the text font - uses default font when null
		\param guidofont the guido font - uses default font when null
		\return an error code
	*/
	static GuidoErrCode GuidoInit(const char* textfont=0, const char* guidofont=0);

    //==============================================================================
    juce_UseDebuggingNewOperator

private:
	ARHandler	fARHandler;
	GRHandler	fGRHandler;
	int			fPage;
	bool		fResizeToMusic;
	juce::Colour fScoreColor;
	
	String				fGMNCode;
	String				fGMNFile;
	GuidoLayoutSettings	fSettings;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    GuidoComponent (const GuidoComponent&);
    const GuidoComponent& operator= (const GuidoComponent&);
};


#endif
