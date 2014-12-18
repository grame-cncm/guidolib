package guidoengine;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;

/**
 * 
 * @author guillaume
 * 
 */
public final class guidopianoroll {

	final static int kCLine = 1;
	final static int kCSharpLine = 1 << 1;
	final static int kDLine = 1 << 2;
	final static int kDSharpLine = 1 << 3;
	final static int kELine = 1 << 4;
	final static int kFLine = 1 << 5;
	final static int kFSharpLine = 1 << 6;
	final static int kGLine = 1 << 7;
	final static int kGSharpLine = 1 << 8;
	final static int kALine = 1 << 9;
	final static int kASharpLine = 1 << 10;
	final static int kBLine = 1 << 11;
	final static int kAutoLines = 0;
	final static int kNoLine = -1;

	final static int kSimplePianoRoll = 0;
	final static int kTrajectoryPianoRoll = 1;

	public long fPianoRoll;
	public long fARHandler;

	public void setARHandler(long arRef) {
		this.fARHandler = arRef;
	}

	public native final synchronized void AR2PianoRoll(int type);

	public native final synchronized void Midi2PianoRoll(int type, String midiFileName);

	public native final synchronized int DestroyPianoRoll();

	public native final synchronized int SetLimits(limitparams limitParams);

	public native final synchronized int EnableKeyboard(boolean enabled);

	public native final synchronized int GetKeyboardWidth(int height, float keyboardWidth);

	public native final synchronized int EnableAutoVoicesColoration(boolean enabled);

	public native final synchronized int SetRGBColorToVoice(int voiceNum, int r, int g, int b, int a);

	public native final synchronized int SetHtmlColorToVoice(int voiceNum, long color);

	public native final synchronized int EnableMeasureBars(boolean enabled);

	public native final synchronized int SetPitchLinesDisplayMode(int mode);

	public native final synchronized int GetMap(int width, int height, guidoscoremap scoremap);

	// OnDraw(PianoRoll *pr, int width, int height, VGDevice* dev); TODO GGX

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
	public native final synchronized int GetBitmap(int[] dst, int w, int h, guidopaint area,
			Color color);

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
		class foo implements ImageObserver {
			public boolean imageUpdate(Image img, int infoflags, int x, int y, int width, int height) {
				return true;
			}
		}
		BufferedImage img = new BufferedImage(w, h, BufferedImage.TYPE_4BYTE_ABGR_PRE);
		int[] outPixels = new int[w * h];
		int result = GetBitmap(outPixels, w, h, new guidopaint(), color);
		img.setRGB(0, 0, w, h, outPixels, 0, w);
		g.drawImage(img, 0, 0, new foo());
		return result;
	}

	/**
	 * Internal jni initialization method. Automatically called at package init.
	 */
	protected static native void Init();
}
