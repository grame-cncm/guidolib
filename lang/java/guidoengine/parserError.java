package guidoengine;

/**
 * Class to get the detail of parser error.
 * 
 */
public class parserError {

	/** Ligne of the error */
	private int col;
	/** Column of the error */
	private int ligne;
	/** message of the error */
	private String message;

	public parserError(int col, int ligne, String message) {
		this.col = col;
		this.ligne = ligne;
		this.message = message;
	}

	public int getCol() {
		return col;
	}

	public int getLigne() {
		return ligne;
	}

	public String getMessage() {
		return message;
	}

	/**
	 * Internal jni initialization method. Automatically called at package init.
	 */
	protected static native void Init();
}
