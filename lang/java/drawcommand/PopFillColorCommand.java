package drawcommand;

public class PopFillColorCommand implements PrintableDrawCommand {

	public PopFillColorCommand() {
		super();
	}
	
	@Override
	public String asString() {
		return "PopFillColor";
	}
}
