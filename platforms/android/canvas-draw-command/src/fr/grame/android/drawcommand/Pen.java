package fr.grame.android.drawcommand;

/**
 * A pen is a color and a width.
 *
 */
public class Pen {
	private int color;
	private float width;
	
	public Pen(int color, float width) {
		this.color = color;
		this.width = width;
	}

	public int getColor() {
		return color;
	}

	public float getWidth() {
		return width;
	}
}
