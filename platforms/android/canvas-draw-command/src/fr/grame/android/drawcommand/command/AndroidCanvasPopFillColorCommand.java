package fr.grame.android.drawcommand.command;

import android.graphics.Canvas;
import drawcommand.PopFillColorCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

public class AndroidCanvasPopFillColorCommand extends PopFillColorCommand implements DrawToCanvas {

	public AndroidCanvasPopFillColorCommand() {
		super();
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.popFillColor();
	}

}
