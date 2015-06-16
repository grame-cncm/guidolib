/*
   Copyright (C) 2010 Sony CSL Paris
   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright
         notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
         notice, this list of conditions and the following disclaimer in the
         documentation and/or other materials provided with the distribution.
 * Neither the name of Sony CSL Paris nor the names of its contributors 
         may be used to endorse or promote products derived from this software 
         without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
   EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package guidoengine;

/**
 * The main score API. <br>
 * A guido score has an internal abstract representation (AR) that is converted
 * into a graphic representation (GR). The guidoscore reflects this architecture
 * and provices the method to convert an AR representation to GR representation.
 */
public class guidoscorebase {
	//public static final int kNoBB = 0; // Doesn't exist in GUIDOScoreMap.h
	public static final int kPageBB = 0;
	public static final int kSystemsBB = 1;
	public static final int kSystemsSliceBB = 2;
	public static final int kStavesBB = 3;
	public static final int kBarlinesBB = 4;
	//public static final int kMeasureBB = 4; // Commented in GUIDOScoreMap.h
	public static final int kEventsBB = 5;

	public final long fARHandler;
	public final long fGRHandler;

	private long fGuidoStream;
	private long fGuidoParser;

	private parserError fParserError;

	/**
	 * Get the reference to an Abstract Representation.
	 * 
	 * @return the current ARHandler
	 */
	public long getARHandler() {
		return fARHandler;
	}

	/**
	 * Parse a guido file
	 * 
	 * On output, {@code fARHandler} contains a handler to the Guido AR
	 * representation.
	 * 
	 * @param filename
	 *            the file name
	 * @return an error code.
	 * @deprecated replaced by {@link guidoengine.guidoparser#File2AR()}
	 */
	@Deprecated
	public native final synchronized int ParseFile(String filename);

	/**
	 * Parse a guido string
	 * 
	 * On output, {@code fARHandler} contains a handler to the Guido AR
	 * representation.
	 * 
	 * @param gmn
	 *            a string containing GMN code
	 * @return an error code.
	 * @see "The GUIDO Music Notation Format"
	 * @deprecated replaced by {@link guidoengine.guidoparser#String2AR()}
	 */
	@Deprecated
	public native final synchronized int ParseString(String gmn);

	/**
	 * Converts an AR representation into a GR representation
	 * 
	 * On output, {@code fGRHandler} contains a handler to the Guido GR
	 * representation.
	 * 
	 * @return an error code.
	 */
	public native final synchronized int AR2GR();

	/**
	 * Converts an AR representation into a GR representation
	 * 
	 * Makes use of the Guido Engine settings given as argument. On output,
	 * {@code fGRHandler} contains a handler to the Guido GR representation.
	 * 
	 * @param layout
	 *            layout settings
	 * @return an error code.
	 * @see guidolayout
	 */
	public native final synchronized int AR2GR(guidolayout layout);

	/**
	 * Exports an AR representation to a MIDI file
	 * 
	 * @param filename
	 *            the MIDI file name
	 * @param params
	 *            MIDI export settings
	 * @return an error code.
	 * @see guido2midiparams
	 */
	public native final synchronized int AR2MIDIFile(String filename, guido2midiparams params);

	/**
	 * Exports a graphic representation to a abstract representation format.
	 * 
	 * @param pagenum
	 *            the page to be exported
	 * @return the String with the draw command and their parameters.
	 */
	public native final synchronized String AbstractExport(int pagenum);

	/**
	 * Exports a graphic representation to internal binary format. The byte
	 * array can be read with guidobinaryparser.
	 * 
	 * @param pagenum
	 *            the page to be exported
	 * @return a byte array with the draw command and the parameters or a null
	 *         object if an error occurs.
	 */
	public native final synchronized byte[] BinaryExport(int pagenum);

	/**
	 * Exports one page of score to SVG.
	 * 
	 * @param page
	 *            the page number.
	 * @param embedFont
	 *            if true, it use a embbed guido font and add it to the SVG.
	 * @param font
	 *            path of the guido svg font file or an svg font in text
	 *            representation. Can be use if embedFont is set to false else
	 *            it is unnecessary. If the library is compiled with
	 *            INDEPENDENTSVG option, don't use a font file. The library
	 *            cannot calculate font metric. You have better to use embedded
	 *            font.
	 * @param mappingMode
	 *            the mapping mode (see mapping mode enum).
	 * @return a String with the svg code or a null object if an error occurs.
	 */
	public native final synchronized String GR2SVG(int page, boolean embedFont, String font, int mappingMode);

	/**
	 * Exports a graphic representation to SVG
	 * 
	 * @param pagenum
	 *            the page to be exported
	 * @param fontFile
	 *            an optional Guido SVG font file (could be empty when there is
	 *            no SVG font)
	 * @return a string containing the corresponding SVG code.
	 * @deprecated replaced by {@link #GR2SVG()}
	 */
	@Deprecated
	public native final synchronized String SVGExport(int pagenum, String fontFile);

	/**
	 * Exports a graphic representation to SVG with a font spec
	 * 
	 * @param pagenum
	 *            the page to be exported
	 * @param fontFile
	 *            an optional Guido SVG font file (could be empty when there is
	 *            no SVG font)
	 * @param fontSpec
	 *            an optional Guido SVG font spec, meaning a SVG file read into
	 *            the string...will be big (could be empty when there is no SVG
	 *            font)
	 * @return a string containing the corresponding SVG code.
	 * @deprecated replaced by {@link #GR2SVG()}
	 */
	@Deprecated
	public native final synchronized String SVGExportWithFontSpec(int pagenum, String fontFile, String fontSpec);

	/**
	 * Updates a GR representation
	 * 
	 * Should be called for example after changing the default page format.
	 * 
	 * @return an error code.
	 */
	public native final synchronized int UpdateGR();

	/**
	 * Updates a GR representation
	 * 
	 * Makes use of the Guido Engine settings given as argument.
	 * 
	 * @param layout
	 *            layout settings
	 * @return an error code.
	 * @see guidolayout
	 */
	public native final synchronized int UpdateGR(guidolayout layout);

	/**
	 * Resize the page sizes to the music size.
	 * 
	 * @return an error code.
	 */
	public native final synchronized int ResizePageToMusic();

	/**
	 * Tells the engine to release the AR representation handler.
	 */
	public native final synchronized void FreeAR();

	/**
	 * Tells the engine to release the GR representation handler.
	 */
	public native final synchronized void FreeGR();

	/**
	 * Retrieve the format of a given page.
	 * 
	 * @param pagenum
	 *            a page number, starting from 1
	 * @param pf
	 *            on output, the corresponding page format
	 */
	public native final synchronized void GetPageFormat(int pagenum, guidopageformat pf);

	/**
	 * Force the color of all notes of a voice in a given time interval.
	 * 
	 * @param voicenum
	 *            index of the voice to mark, starting from 1
	 * @param date
	 *            the date where the color-marking must begin (whole note = 1)
	 * @param duration
	 *            the duration that must be covered by the color marking.
	 * @param red
	 *            the red component of the marking color, from 0 to 255.
	 * @param green
	 *            green color component.
	 * @param blue
	 *            blue color component.
	 * @return a Guido error code.
	 */
	public native final synchronized int MarkVoice(int voicenum, guidodate date, guidodate duration, int red,
			int green, int blue);

	/**
	 * Give the score pages count.
	 * 
	 * @return the score pages count or an error code when < 0
	 */
	public native final synchronized int GetPageCount();

	/**
	 * Give the system count of a page.
	 * 
	 * @return the system count or an error code when < 0
	 */
	public native final synchronized int GetSystemCount(int page);

	/**
	 * Give the score count of voices.
	 * 
	 * @return the count of voices an error code when < 0
	 */
	public native final synchronized int CountVoices();

	/**
	 * Give the score duration.
	 * 
	 * @param date
	 *            on output, the score duration.
	 * @return an error code.
	 * @see guidodate
	 */
	public native final synchronized int GetDuration(guidodate date);

	/**
	 * Give a page date.
	 * 
	 * @param pagenum
	 *            a guido page number (starting from 1)
	 * @param date
	 *            on output, the page date when the page number is found.
	 * @return an error code.
	 * @see guidodate
	 */
	public native final synchronized int GetPageDate(int pagenum, guidodate date);

	public native final synchronized int FindEventPage(guidodate date);

	/**
	 * Find a page at a given date.
	 * 
	 * @param date
	 *            a guido date
	 * @return a page number (starting from 1) or 0 when no page is found.
	 * @see guidodate
	 */
	public native final synchronized int FindPageAt(guidodate date);

	/**
	 * Retrieves the graphic to time mapping
	 * 
	 * @param page
	 *            a page index, starting from 1.
	 * @param width
	 *            an area width (typically the current drawing zone width).
	 * @param height
	 *            an area height (typically the current drawing zone height).
	 * @param selector
	 *            a filter to focus on specific elements.
	 * @param f
	 *            a mapcollector object that will be called for each selected
	 *            element.
	 * @return an error code.
	 * @see mapcollector
	 */
	public native final synchronized int GetMap(int page, float width, float height, int selector, mapcollector f);

	/**
	 * Retrieves the wrapped to unwrapped time mapping
	 * 
	 * @param f
	 *            a TimeMapCollector object that will be called for each time
	 *            segment.
	 * @return an error code.
	 * @see timemapcollector
	 */
	public native final synchronized int GetTimeMap(timemapcollector f);

	// -------------------------------------------------------------------------
	// adjusted mappings methods - new in guido lib 1.47 and in guido java 1.20
	// -------------------------------------------------------------------------
	/**
	 * Gives the adjusted page mapping
	 * 
	 * @param page
	 *            a page index, starting from 1.
	 * @param width
	 *            an area width (typically the current drawing zone width).
	 * @param height
	 *            an area height (typically the current drawing zone height).
	 * @param map
	 *            on output, contains the page map.
	 * @return an error code.
	 * @see mapcollector
	 */
	public native final synchronized int GetPageMap(int page, float width, float height, guidoscoremapbase map);

	/**
	 * Gives the adjusted staff mapping
	 * 
	 * @param page
	 *            a page index, starting from 1.
	 * @param width
	 *            an area width (typically the current drawing zone width).
	 * @param height
	 *            an area height (typically the current drawing zone height).
	 * @param staff
	 *            the staff index (starting from 1).
	 * @param map
	 *            on output, contains the staff map.
	 * @return an error code.
	 * @see mapcollector
	 */
	public native final synchronized int GetStaffMap(int page, float width, float height, int staff, guidoscoremapbase map);

	/**
	 * Gives the adjusted voice mapping
	 * 
	 * @param page
	 *            a page index, starting from 1.
	 * @param width
	 *            an area width (typically the current drawing zone width).
	 * @param height
	 *            an area height (typically the current drawing zone height).
	 * @param voice
	 *            the voice index (starting from 1).
	 * @param map
	 *            on output, contains the staff map.
	 * @return an error code.
	 * @see mapcollector
	 */
	public native final synchronized int GetVoiceMap(int page, float width, float height, int voice, guidoscoremapbase map);

	/**
	 * Gives the adjusted system mapping
	 * 
	 * @param page
	 *            a page index, starting from 1.
	 * @param width
	 *            an area width (typically the current drawing zone width).
	 * @param height
	 *            an area height (typically the current drawing zone height).
	 * @param map
	 *            on output, contains the staff map.
	 * @return an error code.
	 * @see mapcollector
	 */
	public native final synchronized int GetSystemMap(int page, float width, float height, guidoscoremapbase map);

	/**
	 * close a score
	 * 
	 * The close method must be called to notify the Guido Engine that the
	 * associated ressources can be released.
	 */
	public void close() {
		FreeGR();
		FreeAR();
	}

	public guidoscorebase() {
		this.fARHandler = 0;
		this.fGRHandler = 0;
		this.fGuidoParser = 0;
		this.fGuidoStream = 0;
	}

	public guidoscorebase(long ar) {
		this.fARHandler = ar;
		this.fGRHandler = 0;
		this.fGuidoParser = 0;
		this.fGuidoStream = 0;
	}

	/**
	 * Control bounding boxes drawing.
	 * 
	 * Bounding boxes are internal to the layout engine. This API is for the
	 * layout engine debugging purpose.
	 * 
	 * @param bbMap
	 *            a bits field indicating the set of bounding boxes to draw
	 *            (default to none).
	 */
	public native final synchronized void DrawBoundingBoxes(int bbMap);

	/**
	 * Gives the drawn bounding boxes set.
	 * 
	 * This API is for the layout engine debugging purpose.
	 * 
	 * @return a bits field indicating the set of bounding boxes.
	 */
	public native final synchronized int GetDrawBoundingBoxes();

	/**
	 * Open a new Parser. A parser have to be closed with CloseParser().
	 */
	public native final synchronized void OpenParser();

	/**
	 * Close the parser.
	 * 
	 * @return an error code.
	 */
	public native final synchronized int CloseParser();

	/**
	 * Get the content of the guidoStream.
	 * 
	 * @return The string int the GuidoStream
	 */
	public native final synchronized String GetStream();

	/**
	 * Parse a file and create the corresponding Abstract Representation. The ar
	 * is stocked internally and have to be freed with FreeAR(). <br/>
	 * To use this method you have to open a parser with OpenParser().
	 * 
	 * @param file
	 *            the name of the file to be parsed.
	 * @return a error code.
	 */
	public native final synchronized int File2AR(String file);

	/**
	 * Parse the gmnCode and create the corresponding Abstract Representation.
	 * The ar is stocked internally and have to be freed with FreeAR(). <br/>
	 * To use this method you have to open a parser with OpenParser().
	 * 
	 * @param gmnCode
	 *            the code to be parsed.
	 * @return a error code.
	 */
	public native final synchronized int String2AR(String gmnCode);

	/**
	 * Parse the gmnCode in the stream (@see OpenStream()) and create the
	 * corresponding Abstract Representation. The ar is stocked internally and
	 * have to be freed with FreeAR(). <br/>
	 * To use this method you have to open a parser with OpenParser().
	 * 
	 * @return a error code.
	 */
	public native final synchronized int Stream2AR();

	/**
	 * Get a parser error if an error occurs after use File2AR, String2AR or
	 * Stream2AR.
	 */
	public native final synchronized parserError ParserGetErrorCode();

	/**
	 * Open a new stream. The stream have to be closed with
	 * {@link #CloseStream()}.
	 */
	public native final synchronized void OpenStream();

	/**
	 * Close the stream.
	 */
	public native final synchronized int CloseStream();

	/**
	 * Write a string in the stream.
	 * 
	 * @param gmnCode
	 */
	public native final synchronized int WriteStream(String gmnCode);

	/**
	 * Reset the content of the stream.
	 */
	public native final synchronized int ResetStream();

	/**
	 * Internal jni initialization method. Automatically called at package init.
	 */
	protected static native void Init();
}
