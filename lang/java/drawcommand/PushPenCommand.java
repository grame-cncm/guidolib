package drawcommand;

public class PushPenCommand implements PrintableDrawCommand {
	protected char alpha;
	protected char red;
	protected char green;
	protected char blue;
	protected float width;
	
	public PushPenCommand(char alpha, char red, char green, char blue, float width) {
		super();
		this.alpha = alpha;
		this.red = red;
		this.green = green;
		this.blue = blue;
		this.width = width;
	}

	@Override
	public String asString() {
		StringBuilder out = new StringBuilder();
		out.append("PushPenColor");
		out.append(" ");
		out.append(alpha);
		out.append(" ");
		out.append(red);
		out.append(" ");
		out.append(green);
		out.append(" ");
		out.append(blue);
		out.append(" ");
		out.append(width);
		return out.toString();
	}
}
