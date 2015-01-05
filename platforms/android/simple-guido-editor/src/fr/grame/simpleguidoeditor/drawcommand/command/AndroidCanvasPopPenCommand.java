package fr.grame.simpleguidoeditor.drawcommand.command;

import android.graphics.Canvas;
import drawcommand.PopPenCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

public class AndroidCanvasPopPenCommand extends PopPenCommand implements DrawToCanvas {

	public AndroidCanvasPopPenCommand() {
		super();
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.popPen();
	}
}
