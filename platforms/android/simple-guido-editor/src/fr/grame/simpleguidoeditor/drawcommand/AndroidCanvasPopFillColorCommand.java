package fr.grame.simpleguidoeditor.drawcommand;

import android.graphics.Canvas;
import drawcommand.PopFillColorCommand;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

public class AndroidCanvasPopFillColorCommand extends PopFillColorCommand implements DrawToCanvas {

	public AndroidCanvasPopFillColorCommand() {
		super();
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.popFillColor();
	}

}
