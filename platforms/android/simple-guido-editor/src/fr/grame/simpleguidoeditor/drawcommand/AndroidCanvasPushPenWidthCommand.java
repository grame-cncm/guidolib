package fr.grame.simpleguidoeditor.drawcommand;

import drawcommand.PushPenWidthCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

public class AndroidCanvasPushPenWidthCommand extends PushPenWidthCommand implements DrawToCanvas {

	public AndroidCanvasPushPenWidthCommand(float width) {
		super(width);
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.pushPenWidth(_width);
	}
}
