/*
	GUIDO Library
	Copyright (C) 2012	Grame

	openFrameworks Guido interface by Thomas Coffy (c) IRCAM 2014

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

#include "GUIDOEngine.h"
#include "openFrameworksDevice.h"
#include <ofMain.h>

class GuidoComponent  //: public Component
{
	
public:
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
		const string& getGMNCode() const	{ return fGMNCode; }	
		
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
		void setScoreColor(const ofColor& color);
		/// \brief returns the color used to draw the score
		const ofColor& getScoreColor() const		{ return fScoreColor; }

		void setHeight(int h);
		void setWidth(int w);
		int getHeight();
		int getWidth();
		void setSize(int w, int h) { width = w; height = h; }

		void draw(int x, int y, int w, int h);
//		void resized();

		openFrameworksDevice* getDevice();
	/**
	*	\brief Guido Engine initialization.
	
		\param textfont the text font - uses default font when null
		\param guidofont the guido font - uses default font when null
		\return an error code
	*/
	static GuidoErrCode GuidoInit(const char* textfont=0, const char* guidofont=0);

private:
	ARHandler	fARHandler;
	GRHandler	fGRHandler;
	int		fPage;
	bool		fResizeToMusic;
	ofColor		fScoreColor;
	
	string		fGMNCode;
	string		fGMNFile;
	GuidoLayoutSettings	fSettings;
	int width, height;

    // (prevent copy constructor and operator= being generated..)
    GuidoComponent (const GuidoComponent&);
    const GuidoComponent& operator= (const GuidoComponent&);
};


#endif
