package fr.grame.simpleguidoeditor.drawcommand.command;

import drawcommand.PopPenColorCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;

public class AndroidCanvasPopPenColorCommand extends PopPenColorCommand implements DrawToCanvas {

	public AndroidCanvasPopPenColorCommand() {
		super();
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.popPenColor();
	}
}
