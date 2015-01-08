package drawcommand;

public class PushFillColorCommand implements PrintableDrawCommand {
	protected char alpha;
	protected char red;
	protected char green;
	protected char blue;

	public PushFillColorCommand(char alpha, char red, char green, char blue) {
		super();
		this.alpha = alpha;
		this.red = red;
		this.green = green;
		this.blue = blue;
	}

	@Override
	public String asString() {
		StringBuilder out = new StringBuilder();
		out.append("PushFillColor");
		out.append(" ");
		out.append(alpha);
		out.append(" ");
		out.append(red);
		out.append(" ");
		out.append(green);
		out.append(" ");
		out.append(blue);
		return out.toString();
	}
}
