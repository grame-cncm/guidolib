package drawcommand;

public class PopPenCommand implements PrintableDrawCommand {

	public PopPenCommand() {
		super();
	}
	
	@Override
	public String asString() {
		return "PopPen";
	}

}
