package guidoengine;

/**
 * Class of parameter for piano roll.
 * 
 */
public class rectangle {
	private int left;
    private int top;
    private int right;
    private int bottom;

	public rectangle() {
		left = 0;
        top = 0;
        right = 0;
        bottom = 0;
	}

	/**
	 * Internal jni initialization method. Automatically called at package init.
	 */
	protected static native void Init();

	public int getLeft() {
		return left;
	}

	public int getTop() {
		return top;
	}

	public int getRight() {
		return right;
	}

	public int getBottom() {
		return bottom;
	}
    
    public void setRect(double x, double y, double width, double height) {
        left = (int) x;
        top = (int) y;
        right = (int) (x + width);
        bottom = (int) (y + height);
    }
}
