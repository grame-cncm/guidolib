package guidoengine;

/**
 * Class of parameter for piano roll.
 * @author guillaume
 * 
 */
public final class limitparams {
	private guidodate startDate;
	private guidodate endDate;
	private int lowPitch;
	private int highPitch;

	public limitparams() {
		startDate = new guidodate();
		endDate = new guidodate();
	}

	public limitparams(guidodate startDate, guidodate endDate, int lowPitch,
			int highPitch) {
		this.startDate = startDate;
		this.endDate = endDate;
		this.lowPitch = lowPitch;
		this.highPitch = highPitch;
	}

	/**
	 * Internal jni initialization method. Automatically called at package init.
	 */
	protected static native void Init();

	public guidodate getStartDate() {
		return startDate;
	}

	public void setStartDate(guidodate startDate) {
		this.startDate = startDate;
	}

	public guidodate getEndDate() {
		return endDate;
	}

	public void setEndDate(guidodate endDate) {
		this.endDate = endDate;
	}

	public int getLowPitch() {
		return lowPitch;
	}

	public void setLowPitch(int lowPitch) {
		this.lowPitch = lowPitch;
	}

	public int getHighPitch() {
		return highPitch;
	}

	public void setHighPitch(int highPitch) {
		this.highPitch = highPitch;
	}
}
