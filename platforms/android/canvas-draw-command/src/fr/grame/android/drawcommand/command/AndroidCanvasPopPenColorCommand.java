package fr.grame.android.drawcommand.command;

import drawcommand.PopPenColorCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

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
