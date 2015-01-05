package fr.grame.simpleguidoeditor.drawcommand.command;

import drawcommand.PushPenWidthCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;

public class AndroidCanvasPushPenWidthCommand extends PushPenWidthCommand implements DrawToCanvas {

	public AndroidCanvasPushPenWidthCommand(float width) {
		super(width);
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.pushPenWidth(_width);
	}
}
