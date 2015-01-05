package guidoengine;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;

/**
 * A class to create and manipulate piano roll.
 */
public final class guidopianoroll {

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
	public guidopianoroll() {
	}

	/**
	 * Create a pino roll and add a arHandler. The ar handler is not freed be
	 * this class.
	 * 
	 * @param arHandler
	 */
	public guidopianoroll(long arHandler) {
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
	 * Draws the score into a bitmap.
	 * 
	 * Actually, draws the score to an offscreen that is next copied to the
	 * destination bitmap.
	 * 
	 * @param dst
	 *            the destination bitmap ARGB array
	 * @param w
	 *            the bitmap width
	 * @param h
	 *            the bitmap height
	 * @param area
	 *            clipping description
	 * @param color
	 *            the color used to draw the score
	 * @see guidodrawdesc
	 * @see guidopaint
	 */
	public native final synchronized int GetBitmap(int[] dst, int w, int h, guidopaint area, Color color);

	/**
	 * Draws the score.
	 * 
	 * Drawing the score should be typically called from the paint method of a
	 * Canvas.
	 * 
	 * @param g
	 *            a Graphics
	 * @param w
	 *            the desired drawing width
	 * @param h
	 *            the desired drawing heigth
	 * @param area
	 *            clipping description
	 * @see guidodrawdesc
	 * @see guidopaint
	 */
	public synchronized int Draw(Graphics g, int w, int h, guidopaint area) {
		return Draw(g, w, h, area, Color.black);
	}

	/**
	 * Draws the score.
	 * 
	 * Drawing the score should be typically called from the paint method of a
	 * Canvas.
	 * 
	 * @param g
	 *            a Graphics
	 * @param w
	 *            the desired drawing width
	 * @param h
	 *            the desired drawing heigth
	 * @param area
	 *            clipping description
	 * @param color
	 *            the color used to draw the score
	 * @see guidodrawdesc
	 * @see guidopaint
	 */
	public synchronized int Draw(Graphics g, int w, int h, guidopaint area, Color color) {
		return Draw(g, 0, 0, w, h, area, color);
	}

	/**
	 * Draws the score.
	 * 
	 * Drawing the score should be typically called from the paint method of a
	 * Canvas.
	 * 
	 * @param g
	 *            a Graphics
	 * @param top
	 *            the top coordinate to begin to draw the image
	 * 
	 * @param left
	 *            the left coordinate to begin to draw the image
	 * @param w
	 *            the desired drawing width
	 * @param h
	 *            the desired drawing height
	 * @param area
	 *            clipping description
	 * @param color
	 *            the color used to draw the score
	 * @see guidodrawdesc
	 * @see guidopaint
	 */
	public synchronized int Draw(Graphics g, int top, int left, int w, int h, guidopaint area, Color color) {
		class foo implements ImageObserver {
			public boolean imageUpdate(Image img, int infoflags, int x, int y, int width, int height) {
				return true;
			}
		}
		BufferedImage img = new BufferedImage(w, h, BufferedImage.TYPE_4BYTE_ABGR_PRE);
		int[] outPixels = new int[w * h];
		int result = GetBitmap(outPixels, w, h, new guidopaint(), color);
		img.setRGB(0, 0, w, h, outPixels, 0, w);
		g.drawImage(img, left, top, new foo());
		return result;
	}

	/**
	 * Internal jni initialization method. Automatically called at package init.
	 */
	protected static native void Init();
}
