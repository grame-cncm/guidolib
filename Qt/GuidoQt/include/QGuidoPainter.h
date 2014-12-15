/*
 * QGuidoPainter.h
 *
 * Created by Christophe Daudin on 12/05/09.
 * Copyright 2009 Grame. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.

 * Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 * research@grame.fr
 */
#ifndef GUIDO_PAINTER_H
#define GUIDO_PAINTER_H

#include "GUIDOEngine.h"
#include "GUIDOParse.h"
#include "GUIDOPianoRoll.h"

#include <QString>
#include <QPainter>
#include <QSizeF>
#include <QRect>
#include <QColor>
 
class GSystemQt;
class VGDevice;

/**
*	\brief The QGuidoPainter object is a Qt encapsulation of the Guido Engine, 
*	basically allowing you to draw a Guido Score with a QPainter.
*
*	You first specifie the Guido Score file with the setGMNFile or setGMNCode methods, and then 
*	just call the draw method, specifying a QPainter, draw bounding rect, and a page index.
*	
*	\note	QGuidoPainter constructor and destructor are protected. You must use the factory function
*			createGuidoPainter to build one, and destroyGuidoPainter to destroy one.
*	\note	You must call startGuidoEngine and destroyGuidoEngine at the beginning and the end of your
*			application. 
*	\warning You can NOT re-start the GuidoEngine once you've already stopped it.
*/
class QGuidoPainter
{
	
// ------- Static members -----------------------
	public:

		/** \brief Initialize the GUIDO score engine.
		*
		*	You must call this function to be able to instanciate 
		*	QGuidoPainter objects, or else the createGuidoPainter
		*	function will return NULL.
		*
		*	\note Calling this function more than once doesn't affect
		*	the score engine.
		*/
		static void				startGuidoEngine();

		/** \brief Creates a new QGuidoPainter object.
		*
		*	\return a pointer to the new QGuidoPainter object, or
		*	NULL if you didn't previously called the startGuidoEngine function.
		*/
		static QGuidoPainter *	createGuidoPainter();

		/** \brief Destroys the specified QGuidoPainter.
		*
		*	If the specified QGuidoPainter is NULL, does nothing.
		*
		*	\note You mustn't call "delete" in you own application ;
		*	you have to use the destroyGuidoPainter function to
		*	avoid shared-library memory problems.
		*/
		static void				destroyGuidoPainter(QGuidoPainter * painter);

		/** \brief Stops the GUIDO score engine.
		*
		*	\note You must call the function at the end of your application
		*	to free the internal Guido score engine objects.
		*	\warning You mustn't call this function before every QGuidoPainter 
		* 	objects have been destroyed.
		*/
		static void				stopGuidoEngine();

	protected:

		/**	\brief Returns the GuidoEngine state : started or not.
		*
		*/
		static bool				isGuidoEngineStarted();
		
		static GSystemQt * mSys;
		static VGDevice * mDev;
	
// ------- Non-Static members -------------------
	public :
		enum { kDefaultSystemDistance=75, kMinSystemDistance=1 };
		
		/**	\brief Sets the current Guido code to draw with the content of the file.
		*	\param fileName Full path to the Guido Score Notation file.
		*	\return true if the file is a valid Guido Score file.
		*/
		bool setGMNFile(const QString& fileName);
		
		/**	\brief Returns the last file loaded with setFile.
		*/
		const QString& fileName() const;
		
		/**	\brief Sets the current Guido code to draw.
		*	\param gmnCode The Guido Music Notation code
		*	\return true if the GMN code is valid.
		*/
		bool setGMNCode( const QString& gmnCode, const char* datapath=0 );
    
        bool setGMNStream( GuidoStream * gmnStream);
    
		/**	\brief Returns the current Guido code.
		*/
		QString gmnCode() const;
		
        /** \brief Neck to convert AR to GR and get conversion time
		*/
		GuidoErrCode convertAR2GR(ARHandler ar, const GuidoLayoutSettings* settings, GRHandler* gr);

		/** \brief Returns the validity of the last GMN code loaded with setGMNCode or setGMNFile
		*/
		bool isGMNValid() const;

		/**
		*	\brief Returns the number of page of the current Guido Score.
		*/
		int pageCount() const;
		
		/**
		*	\brief Draws the current Guido Score using the specified QPainter.
		*
		*	The Guido Score won't be
		*	streched and will keep its width/height ratio.
		*
		*	\param painter The QPainter to be used for the draw.
		*	\param page Index of the score page to draw (starts with 1).
		*	\param drawRectangle Specifies the zone of the QPaintDevice in which to draw.
		*	\param redrawRectangle (optionnal) Specifies the rectangle to be redrawn. A null 
		*	redrawRectangle will redraw everything.
		*
		*	\note drawRectangle and redrawRectangle are in QPainter's 
		*	QPaintDevice coordinates.
		*/
		void draw( QPainter * painter , int page , const QRect& drawRectangle ,  const QRect& redrawRectangle = QRect() );

        /**
		*	\brief Draws the current Guido Score using the specified QPainter, under Piano Roll form.
		*
		*	The Guido Score won't be
		*	streched and will keep its width/height ratio.
		*
		*	\param painter The QPainter to be used for the draw.
		*	\param drawRectangle Specifies the zone of the QPaintDevice in which to draw.
        *   \param pianoRoll the current PianoRoll
		*
		*	\note drawRectangle is in QPainter's QPaintDevice coordinates.
		*/
        GuidoErrCode drawPianoRoll(QPainter * painter, const QRect& drawRectangle, PianoRoll *pianoRoll);
		
		/**
		*	\brief Returns the height corresponding to the specified width for the
		*	specified page, according to the page format.
		*
		*	The page format & size are defined in the Guido Score file.
		*/
		int heightForWidth ( int w , int page ) const;

		/**
		*	\brief Returns the size of the specified page, in millimeters.
		*
		*	The page format & size are defined in the Guido Score file.
		*/
		QSizeF pageSizeMM(int page) const;
		
		/**
		*	\brief Returns a description of the last encountered error.
		*/
		QString getLastErrorMessage() const;
		
		/**
		*	\brief Gets the parse error line/col.
		*/
		void getLastParseErrorLine(int &line, int &col) const;
		
		/// \brief sets the guido layout settings \see GUIDOEngine interface
		void setGuidoLayoutSettings(const GuidoLayoutSettings& layoutSettings);
		/// \brief returns the guido layout settings \see GUIDOEngine interface
		GuidoLayoutSettings guidoLayoutSettings() const;
		
		/// \brief sets the color used to draw the score
		void setScoreColor(const QColor& color);
		/// \brief returns the color used to draw the score
		const QColor& getScoreColor() const;
		
		/// \brief sets the minimum systems distance to its default value
		void  resetSystemsDistance()	{ setSystemsDistance(kDefaultSystemDistance); }
		/// \brief sets the minimum systems distance
		void  setSystemsDistance(float distance);
		/// \brief returns the minimum systems distance
		float getSystemsDistance() const;

		/// \brief Disable/enable automatic ResizePageToMusic
		void setResizePageToMusic(bool isOn);
		/// \brief Returns the state of the automatic ResizePageToMusic mode (enabled or disabled)
        bool isResizePageToMusicOn() const { return mLayoutSettings.resizePage2Music; }

		/// \brief Sets the page format used when no page format is specified by the GMN
		void setGuidoPageFormat(const GuidoPageFormat& pageFormat);
		/// \brief Gets the page format used when no page format is specified by the GMN
		GuidoPageFormat guidoPageFormat() const;

		/**
		*	\brief Gives access to the GRHandler (graphic representation) of the Score in read-only.
		*/
		CGRHandler	getGRHandler() const { return mDesc.handle; }
		GRHandler	getGRHandler()		{ return mDesc.handle; }
		
		/**
		*	\brief Gives access to the ARHandler (abstract representation) of the Score in read-only.
		*/
		CARHandler	getARHandler() const { return mARHandler; }
		
		/**
		*	\brief Directly set the AR handler.
		*/
		void		setARHandler(ARHandler ar);
        GuidoParser* getParser() {return fParser;}

	protected:
	
		QGuidoPainter();
		~QGuidoPainter();

		typedef GuidoErrCode (*GuidoParseFunction)( const char * , ARHandler*);

		bool setGMNData( const QString& data, const char* dataPath=0 );
        bool setGMNDataStream (GuidoStream * guidoStream);
		bool hasValidGR() const			{ return mDesc.handle != 0; }

        void setPathsToARHandler (ARHandler inARHandler, const char* data);
		
		GuidoOnDrawDesc mDesc;
		ARHandler mARHandler;
		QString mFileName;
		QString mGMNCode;
        GuidoStream * mGMNStream;
		GuidoErrCode mLastErr;

		GuidoLayoutSettings mLayoutSettings;		
		GuidoPageFormat mPageFormat;

		QColor fCurrentColor;		// the color for drawing the score
        
        GuidoParser *fParser;
};

#endif
