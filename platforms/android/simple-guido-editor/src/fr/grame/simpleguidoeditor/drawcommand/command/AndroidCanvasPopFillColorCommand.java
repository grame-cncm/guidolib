package fr.grame.simpleguidoeditor.drawcommand.command;

import android.graphics.Canvas;
import drawcommand.PopFillColorCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

public class AndroidCanvasPopFillColorCommand extends PopFillColorCommand implements DrawToCanvas {

	public AndroidCanvasPopFillColorCommand() {
		super();
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.popFillColor();
	}

}
