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
	The main Guido Engine class.
	
	Provides basic information about the engine (version, units)
	Defines error codes.
*/
public final class guido {
	
	public static final int kNoBB = 0;
	public static final int kPageBB = 1;
	public static final int kSystemsBB = 2;
	public static final int kSystemsSliceBB = 4;
	public static final int kStavesBB = 8;
	public static final int kMeasureBB = 0x10; 
	public static final int kEventsBB = 0x20;
		
    public static final int guidoNoErr					= 0;
	public static final int guidoErrParse				= -1;
	public static final int guidoErrMemory				= -2;
	public static final int guidoErrFileAccess			= -3;
	public static final int guidoErrUserCancel			= -4;
	public static final int guidoErrNoMusicFont			= -5;
	public static final int guidoErrNoTextFont			= -6;
	public static final int guidoErrBadParameter		= -7;
	public static final int guidoErrInvalidHandle		= -8;
	public static final int guidoErrNotInitialized		= -9;
	public static final int guidoErrActionFailed		= -10;

    static {
        try {
			guidolayout.Init ();
			guidopageformat.Init ();
			parserError.Init();
			guidoscorebase.Init ();
			guidodate.Init ();
			guidodrawdesc.Init ();
			guidopaint.Init ();
			guidorect.Init ();
			guidoelementinfo.Init ();
			guidosegment.Init ();
			guidofactory.Init ();
			guido2midiparams.Init();
			limitparams.Init();
			guidoscoremapbase.Init();
			guidopianorollbase.Init();
			System.out.println("GUIDOEngine version " + GetVersion() + " - JNI interface version " + GetJNIVersion() + " initialized.");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("GUIDOEngine native code library initialization failed.\n" + e);
		}
    }

	/**
        Initialises the Guido Engine. 
		
		Must be called before any attempt to read a Guido file or to use the Guido Factory
		@param guidoFont a music font name (only "Guido2" supported).
		@param textFont a text font name.
        @return a Guido error code.
    */
	public static native final synchronized int Init(String guidoFont, String textFont);

	/**
	 * Guido shut down.
	 * Actually release the font allocated by the engine.
	 */
	public static native final synchronized void shutdown();
	
	/** Check the libMusicXML library availability
		@return true when available
	*/
    public static native final boolean  xml2gmn();

	/** Gives the libMusicXML library version when available
		@return a version string (empty when the library is not available)
	*/
    public static native final String  musicxmlversion();

	/** Gives the MusicXML to guido converter version
		@return a version string (empty when the library is not available)
	*/
    public static native final String  musicxml2guidoversion();

	/** Converts a MusicXML file to a GMN string

		@param filename the file name
		@return a string
	*/
    public static native final String  xml2gmn(String filename);

	/**
		Gives a textual description of a Guido error code.

		@param errCode a Guido error code.
		@return a string describing the error.
	*/
	public static native final String GetErrorString( int errCode );

	/**
        Gives the line of a Guido script where the last parse error has occured.
		@return a line number.
		@deprecated replaced by {@link guidoengine.guidoscorebase#ParserGetErrorCode()}
	*/
	@Deprecated
	public static native final int	GetParseErrorLine();

	/** Converts internal Guido units into centimeters.

		@param val the value to be converted
		@return the converted value
	*/
    public static native final float Unit2CM(float val);

	/** Converts centimeters into internal Guido units.

		@param val the value to be converted
		@return the converted value
	*/
    public static native final float CM2Unit(float val);

	/** Converts internal Guido units into inches.

		@param val the value to be converted
		@return the converted value
	*/
    public static native final float Unit2Inches(float val);

	/** Converts inches into internal Guido units.

		@param val the value to be converted
		@return the converted value
	*/
    public static native final float Inches2Unit(float val);

	/**	Gives the library version number as a string

        Version number format is  MAJOR.MINOR.SUB
	*/
	public static native final String GetVersion();

	/**	Gives the JNI library version number as a string

        Version number format is  MAJOR.MINOR.SUB
	*/
	public static native final String GetJNIVersion();

	/**	Checks a required library version number.

		@param major the major revision number.
		@param minor the minor revision number.
		@param sub the sub revision number.
		@return noErr if the library version number is greater or equal to the version number
			passed as argument.
		@return otherwise guidoErrActionFailed.
	*/
	public static native final int CheckVersionNums(int major, int minor, int sub);

	/** Gives the distance between two staff lines.

		This value is constant (= 50). It does not depend on the context, it will
		probably never change in future versions of the library.

		@return the distance between two lines of staff, in Guido internal units.
	*/
	public static native final float GetLineSpace();
}

