package guidoengine;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;

/**
 * A class to manipulate pianoroll.
 * Add extended API to draw score on a bitmap with a native device and draw the bitmap in a java.awt.Graphics object.
 */
public final class guidopianoroll extends guidopianorollbase {
	public guidopianoroll() {
		super();
	}
	
	public guidopianoroll(long arHandler) {
		super(arHandler);
	}
	/**
	 * Draws the score into a bitmap.
	 * 
	 * Actually, draws the score to an offscreen that is next copied to the
	 * destination bitmap.
	 * 
	 * @param dst	the destination bitmap ARGB array
	 * @param w  	the bitmap width
	 * @param h  	the bitmap height
	 * @param area  clipping description
	 * @param color	the color used to draw the score
	 * @return a Guido error code.
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
	 * @return a Guido error code.
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
	 * @param g a Graphics
	 * @param w the desired drawing width
	 * @param h the desired drawing heigth
	 * @param area clipping description
	 * @param color the color used to draw the score
	 * @return a Guido error code.
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
	 * @param g  a Graphics
	 * @param top the top coordinate to begin to draw the image
	 * @param left the left coordinate to begin to draw the image
	 * @param w the desired drawing width
	 * @param h the desired drawing height
	 * @param area clipping description
	 * @param color the color used to draw the score
	 * @return a Guido error code.
	 * @see guidodrawdesc
	 * @see guidopaint
	 */
	public synchronized int Draw(Graphics g, int top, int left, int w, int h, guidopaint area, Color color) {
		class foo implements ImageObserver {
			@Override
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
}
