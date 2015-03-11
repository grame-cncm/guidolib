package guidoengine;

/**
 * A class to create and manipulate piano roll.
 */
public class guidopianorollbase {

	/** line pitch */
	public final static int kCLine = 1;
	public final static int kCSharpLine = 1 << 1;
	public final static int kDLine = 1 << 2;
	public final static int kDSharpLine = 1 << 3;
	public final static int kELine = 1 << 4;
	public final static int kFLine = 1 << 5;
	public final static int kFSharpLine = 1 << 6;
	public final static int kGLine = 1 << 7;
	public final static int kGSharpLine = 1 << 8;
	public final static int kALine = 1 << 9;
	public final static int kASharpLine = 1 << 10;
	public final static int kBLine = 1 << 11;
	public final static int kAutoLines = 0;
	public final static int kNoLine = -1;

	/**
	 * Piano roll type
	 */
	public final static int kSimplePianoRoll = 0;
	public final static int kTrajectoryPianoRoll = 1;

	private long fPianoRoll;
	private long fARHandler;

	/**
	 * Create an empty piano roll
	 */
	public guidopianorollbase() {
	}

	/**
	 * Create a pino roll and add a arHandler. The ar handler is not freed be
	 * this class.
	 * 
	 * @param arHandler
	 */
	public guidopianorollbase(long arHandler) {
		this.fARHandler = arHandler;
	}

	/**
	 * Set a new ArHandler.
	 * 
	 * @param arRef
	 */
	public void setARHandler(long arRef) {
		this.fARHandler = arRef;
	}

	/**
	 * Create a piano roll from the Abstract representation. The piano roll may
	 * be deleted with destroyPianoRoll. A piano Roll instance is created and
	 * have to be deleted with DestroyPianoRoll();
	 * 
	 * @param type
	 *            the type of piano roll (kSimplePianoRoll or
	 *            kTrajectoryPianoRoll)
	 */
	public native final synchronized void AR2PianoRoll(int type);

	/**
	 * Create a piano roll from a midi file. The piano roll may be deleted with
	 * destroyPianoRoll.
	 * 
	 * @param type
	 *            the type of piano roll (kSimplePianoRoll or
	 *            kTrajectoryPianoRoll)
	 * @param midiFileName
	 *            the midi file
	 */
	public native final synchronized void Midi2PianoRoll(int type, String midiFileName);

	/**
	 * Destroy a piano roll created with AR2PianoRoll or Midi2PianoRoll
	 */
	public native final synchronized int DestroyPianoRoll();

	/**
	 * Sets limits to a piano roll (start/end date, lower/higher pitch)
	 * 
	 * @param limitParams
	 *            an object contaning the limits.
	 * @return a Guido error code
	 */
	public native final synchronized int SetLimits(limitparams limitParams);

	/**
	 * Enables keyboard or not (not enabled by default)
	 * 
	 * @param enabled
	 *            a boolean corresponding to the keyboard draw state
	 * @return a Guido error code
	 */
	public native final synchronized int EnableKeyboard(boolean enabled);

	/**
	 * Gets the piano roll keyboard width
	 * 
	 * @param height
	 *            the height of the canvas (-1 to set the default height : 512)
	 * @return the keyBoardWith or -1 if error
	 */
	public native final synchronized float GetKeyboardWidth(int height);

	/**
	 * Enables or not the automatic voices coloration (not enabled by default)
	 * (not for a midi rendering). If a color is manually set with
	 * GuidoPianoRollSetColorToVoice, automatic color will not be applied for
	 * this voice.
	 * 
	 * @param enabled
	 *            a boolean corresponding to the color state
	 * @return a Guido error code
	 */
	public native final synchronized int EnableAutoVoicesColoration(boolean enabled);

	/**
	 * Sets a RGB color to a voice (first voice is number 1) (black by default)
	 * 
	 * @param voiceNum
	 *            the voice number (first voice is number 1)
	 * @param r
	 *            the red param of RGB color
	 * @param g
	 *            the green param of RGB color
	 * @param b
	 *            the blue param of RGB color
	 * @param a
	 *            the alpha param of RGB color
	 * @return a Guido error code
	 */
	public native final synchronized int SetRGBColorToVoice(int voiceNum, int r, int g, int b, int a);

	/**
	 * Sets a html color to a voice (first voice is number 1) (black by default)
	 * 
	 * @param voiceNum
	 *            the voice number (first voice is number 1)
	 * @param color
	 *            an html color
	 * @return a Guido error code
	 */
	public native final synchronized int SetHtmlColorToVoice(int voiceNum, long color);

	/**
	 * Enables or not measure bars (false by default)
	 * 
	 * @param enabled
	 *            a boolean corresponding to the measure bars draw state
	 * @return a Guido error code
	 */
	public native final synchronized int EnableMeasureBars(boolean enabled);

	/**
	 * Sets the pitch lines display mode (automatic by default). Use Pitch lines
	 * display mode constants to pick lines which will be be displayed. Example
	 * : "kCLine + kGLine" will displayed C and G line. "kNoLine" doesn't
	 * display any line. "kAutoLines" adjust line display according to piano
	 * roll pitch range (automatic possibilities : no line, C line, C and G
	 * line, chromatic scale, diatonic scale);
	 * 
	 * @param mode an int corresponding to the pitch lines display mode
	 * @return a Guido error code
	 */
	public native final synchronized int SetPitchLinesDisplayMode(int mode);

	/**
	 * Gets the piano roll map
	 * @param width the width of the piano roll (-1 to set the default width : 1024)
	 * @param height the height of the canvas (-1 to set the default height : 512)
	 * @param scoremap
	 * @return a Guido error code
	 */
	public native final synchronized int GetMap(int width, int height, guidoscoremap scoremap);

	/**
	 * Internal jni initialization method. Automatically called at package init.
	 */
	protected static native void Init();
}
